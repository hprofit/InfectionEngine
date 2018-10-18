cbuffer BlurBuffer
{
    float4 Weights[4];
    int BlurAmount;
    int HorizontalOrVertical; // 0 - Hor, 1 - Vert
    struct {
        float4 X[4];
        float4 Y[4];
    } Offsets;
};

struct VertexInput
{
    float4 position :   POSITION;
    float4 normal :     NORMAL;
    float4 tangent :    TANGENT;
    float4 bitangent :  BITANGENT;
    float2 texCoords :  TEXCOORDS;
    float4 color :      COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 centerTexCoords : TEX_COORDS;
};

Texture2D Texture : register(t0);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
    PixelInput output;

    output.position = float4(mul(input.position, 2.f).xyz, 1);

    output.centerTexCoords = output.position.xy * 0.5f + 0.5f;
    output.centerTexCoords.y = 1.0f - output.centerTexCoords.y;

    return output;
}

struct PixelOutput
{
    float4 blurredValue : SV_TARGET0;
};

PixelOutput PShader(PixelInput input)
{
    PixelOutput output;
    const uint MAX_COORDS = 111;
    float2 texCoords[MAX_COORDS];
    float pixelSize;
    float width, height;
    Texture.GetDimensions(width, height);

    // Horizontal
    if (HorizontalOrVertical == 0)
    {
        pixelSize = 1.f / width;

        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            texCoords[i + BlurAmount] = input.centerTexCoords + float2(pixelSize * i, 0);
        }
    }
    // Vertical
    else
    {
        pixelSize = 1.f / height;

        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            texCoords[i + BlurAmount] = input.centerTexCoords + float2(0, pixelSize * i);
        }
    }
    float2 texOffset = float2(0, 0);
    bool horizontal = HorizontalOrVertical == 0; // 0 - Horizontal, 1 - Vertical
    output.blurredValue = float4(0, 0, 0, 0);

    //int curInputVecIdx = 0;
    //int count = 0;
    //for (int i = -BlurAmount; i <= BlurAmount; ++i)
    //{
    //    //int offset = i <= 0 ? i + BlurAmount : BlurAmount - i;
    //    //texOffset = float2(Offsets.X[i + BlurAmount], 0) + input.centerTexCoords;
    //                      //!horizontal ? OffsetsY[i + BlurAmount] : 0.f);
    //    //texOffset = texCoords[i + BlurAmount] + input.centerTexCoords;
    //    //output.blurredValue += Texture.Sample(ss, texOffset) * Weights[curInputVecIdx][offset];
        
    //    uint offset = i + BlurAmount <= BlurAmount ?
    //                    uint(i + BlurAmount) % 4 :
    //                    3 - (uint(i + BlurAmount) % 4);
        

    //    output.blurredValue.r += Weights[curInputVecIdx][offset];

    //    //output.blurredValue += Texture.Sample(ss, texCoords[i + BlurAmount]) * Weights[offset];
    //    //output.blurredValue.r += weights[offset];//Weights.WeightsArr[offset];
    //}
    //output.blurredValue.r += Weights[0][0]; // -3
    //output.blurredValue.r += Weights[0][1]; // -2
    //output.blurredValue.r += Weights[0][2]; // -1
    //output.blurredValue.r += Weights[0][3]; // 0
    //output.blurredValue.r += Weights[0][2]; // 1
    //output.blurredValue.r += Weights[0][1]; // 2
    //output.blurredValue.r += Weights[0][0]; // 3

    int count = 0, vecIdx = 0;
    for (int i = BlurAmount; i > 0; --i)
    {
        output.blurredValue.r += Weights[vecIdx][count];
        output.blurredValue.r += Weights[vecIdx][count];
        ++count;
        if (count == 4)
        {
            count = 0;
            ++vecIdx;
        }
    }
    output.blurredValue.r += Weights[vecIdx][count];

    output.blurredValue.a = 1;
    return output;
}