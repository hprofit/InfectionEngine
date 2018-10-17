cbuffer BlurBuffer
{
    float4x4 ModelMatrix;
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
    float2 centerTexCoords : TEX_COORDS;
};

Texture2D Texture : register(t0);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
    PixelInput output;

    output.position = mul(ModelMatrix, input.position);

    output.centerTexCoords = output.position.xy * 0.5f + 0.5f;
    output.centerTexCoords.y = 1.0f - output.centerTexCoords.y;

    return output;
}

float GetWeight(float width, float i)
{
    float s = width / 2.0f;
    const float e = 2.71828f;
    float power = -0.5f * pow(i / s, 2);

    return pow(e, power);
}

struct PixelOutput
{
    float4 blurredValue : SV_TARGET0;
};

PixelOutput PShader(PixelInput input)
{
    PixelOutput output;
    const int MAX_COORDS = 111;
    float2 texCoords[MAX_COORDS];
    float weights[MAX_COORDS];
    float pixelSize;
    int width, height;

    Texture.GetDimensions(width, height);
    // Horizontal
    if (HorizontalOrVertical == 0)
    {
        pixelSize = 1.f / width;

        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            texCoords[i + BlurAmount] = input.centerTexCoords + float2(0, pixelSize * i);
            weights[i + BlurAmount] = GetWeight(BlurAmount, i);
        }
    }
    // Vertical
    else
    {
        pixelSize = 1.f / height;

        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            texCoords[i + BlurAmount] = input.centerTexCoords + float2(pixelSize * i, 0);
            weights[i + BlurAmount] = GetWeight(BlurAmount, i);
        }
    }

    output.blurredValue = float4(0, 0, 0, 0);
    for (int i = -BlurAmount; i <= BlurAmount; ++i)
    {
        output.blurredValue += Texture.Sample(ss, texCoords[i + BlurAmount]) * weights[i + BlurAmount];
    }

    return output;
}