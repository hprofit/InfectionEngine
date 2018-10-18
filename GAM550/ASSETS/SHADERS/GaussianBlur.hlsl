cbuffer BlurBuffer
{
    float4 Weights[12];
    struct {
        float4 X[12];
        float4 Y[12];
    } Offsets;
    int BlurAmount;
    int HorizontalOrVertical; // 0 - Hor, 1 - Vert
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
    float2 centerTexCoords : TEXCOORDS;
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

    float2 texOffset = float2(0, 0);
    bool horizontal = HorizontalOrVertical == 0; // 0 - Horizontal, 1 - Vertical
    output.blurredValue = float4(0, 0, 0, 0);

    if (horizontal) 
    {
        int count = 0, vecIdx = 0;
        for (int i = BlurAmount; i > 0; --i)
        {
            if (horizontal)
                texOffset = float2(Offsets.X[vecIdx][count], 0) + input.centerTexCoords;
            else
                texOffset = float2(0, Offsets.Y[vecIdx][count]) + input.centerTexCoords;
            output.blurredValue += Texture.Sample(ss, texOffset) * Weights[vecIdx][count];

            if (horizontal)
                texOffset = float2(-Offsets.X[vecIdx][count], 0) + input.centerTexCoords;
            else
                texOffset = float2(0, -Offsets.Y[vecIdx][count]) + input.centerTexCoords;
            output.blurredValue += Texture.Sample(ss, texOffset) * Weights[vecIdx][count];

            ++count;
            if (count == 4)
            {
                count = 0;
                ++vecIdx;
            }
        }
        texOffset = float2(Offsets.X[vecIdx][count], 0) + input.centerTexCoords;
        output.blurredValue += Texture.Sample(ss, texOffset) * Weights[vecIdx][count];

        output.blurredValue.a = 1;
    }
    else {
        output.blurredValue = float4(0, 1, 0, 1); // Texture.Sample(ss, input.centerTexCoords);
    }

    return output;
}