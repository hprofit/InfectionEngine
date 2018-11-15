cbuffer BlurBuffer2
{
    float4 Weights[32];
};

cbuffer CBFixed {
    static const int BlurRadius = 20;
}

Texture2D gInput;
RWTexture2D<float4> gOutput;

#define N 256
#define CacheSize (N + 2 * BlurRadius)
groupshared float4 gCache[CacheSize];

struct ComputeInput {
    int3 groupThreadID : SV_GroupThreadID;
    int3 dispatchThreadID : SV_DispatchThreadID;
};

[numthreads(N, 1, 1)]
void HorizontalBlurCS(ComputeInput input)
{
    // Fill local thread storage to reduce bandwidth. To blur 
    // N pixels, we need to load N + 2 * BlurRadius pixels due
    // to the blur radius

    int2 length;
    gInput.GetDimensions(length.x, length.y);
    // This thread group runs N threads. To get the extra 2 * BlurRadius
    // pixels, have 2 * BlurRadius threads sample extra pixel.
    if (input.groupThreadID.x < BlurRadius)
    {
        // Clamp out of bound samples that occur at image borders.
        int x = max(input.dispatchThreadID.x - BlurRadius, 0);
        gCache[input.groupThreadID.x] = gInput[int2(x, input.dispatchThreadID.y)];
    }
    if (input.groupThreadID.x >= N - BlurRadius)
    {
        // Clamp out of bound samples that occur at image borders.
        int x = min(input.dispatchThreadID.x + BlurRadius, length.x - 1);
        gCache[input.groupThreadID.x + 2 * BlurRadius] = gInput[int2(x, input.dispatchThreadID.y)];
    }
    // Clamp out of bound samples that occur at image borders.
    gCache[input.groupThreadID.x + BlurRadius] = gInput[min(input.dispatchThreadID.xy, length.xy - 1)];

    // Wait for all threads to finish
    GroupMemoryBarrierWithGroupSync();

    // Now blur each pixel

    float4 blurColor = float4(0, 0, 0, 0);

    int count = 0, vecIdx = 0;
    [unroll]
    for (int i = -BlurRadius; i <= BlurRadius; ++i)
    {
        int k = input.groupThreadID.x + BlurRadius + i;
        blurColor += Weights[vecIdx][count] * gCache[k];
        if (++count >= 4)
        {
            count = 0;
            ++vecIdx;
        }
    }

    gOutput[input.dispatchThreadID.xy] = blurColor;
}


[numthreads(1, N, 1)]
void VerticalBlurCS(ComputeInput input)
{
    // Fill local thread storage to reduce bandwidth. To blur 
    // N pixels, we need to load N + 2 * BlurRadius pixels due
    // to the blur radius

    int2 length;
    gInput.GetDimensions(length.x, length.y);
    // This thread group runs N threads. To get the extra 2 * BlurRadius
    // pixels, have 2 * BlurRadius threads sample extra pixel.
    if (input.groupThreadID.y < BlurRadius)
    {
        // Clamp out of bound samples that occur at image borders.
        int y = max(input.dispatchThreadID.y - BlurRadius, 0);
        gCache[input.groupThreadID.y] = gInput[int2(input.dispatchThreadID.x, y)];
    }
    if (input.groupThreadID.y >= N - BlurRadius)
    {
        // Clamp out of bound samples that occur at image borders.
        int y = min(input.dispatchThreadID.y + BlurRadius, length.y - 1);
        gCache[input.groupThreadID.y + 2 * BlurRadius] = gInput[int2(input.dispatchThreadID.x, y)];
    }
    // Clamp out of bound samples that occur at image borders.
    gCache[input.groupThreadID.y + BlurRadius] = gInput[min(input.dispatchThreadID.xy, length.xy - 1)];

    // Wait for all threads to finish
    GroupMemoryBarrierWithGroupSync();

    // Now blur each pixel

    float4 blurColor = float4(0, 0, 0, 0);

    int count = 0, vecIdx = 0;
    [unroll]
    for (int i = -BlurRadius; i <= BlurRadius; ++i)
    {
        int k = input.groupThreadID.y + BlurRadius + i;
        blurColor += Weights[vecIdx][count] * gCache[k];
        if (++count >= 4)
        {
            count = 0;
            ++vecIdx;
        }
    }

    gOutput[input.dispatchThreadID.xy] = blurColor;
}

technique11 HorizontalBlur
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, HorizontalBlurCS()));
    }
}

technique11 VerticalBlur
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, VerticalBlurCS()));
    }
}
