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
    float4 position :                       SV_POSITION;
    float2 texCoordsV[BlurAmount * 2 + 1] : TEXCOORDS;
    float2 texCoordsH[BlurAmount * 2 + 1] : TEXCOORDS;
    float weights[BlurAmount * 2 + 1] :     WEIGHTS;
};

Texture2D Texture : register(t0);
SamplerState ss;

float GetWeight(float width, float i)
{
    float s = width / 2.0f;
    const float e = 2.71828f;
    float power = -0.5f * pow(i / s, 2);

    return pow(e, power);
}

PixelInput VShader(VertexInput input)
{
    PixelInput output;

    output.position = mul(ModelMatrix, input.position);

    float2 centerTexCoords = output.position.xy * 0.5f + 0.5f;
    int width, height;
    Texture.GetDimensions(width, height);
    // Horizontal
    if (HorizontalOrVertical == 0)
    {
        float pixelSize = 1.f / width;
	
        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            output.texCoordsH[i + BlurAmount] = centerTexCoords + float2(pixelSize * i, 0);
            output.weights[i + BlurAmount] = GetWeight(BlurAmount, i);
        }
    }
    // Vertical
    else
    {
        float pixelSize = 1.f / height;
	
        for (int i = -BlurAmount; i <= BlurAmount; ++i)
        {
            output.texCoordsV[i + BlurAmount] = centerTexCoords + float2(0, 1.0f - (pixelSize * i));
            output.weights[i + BlurAmount] = GetWeight(BlurAmount, i);
        }
    }

    return output;
}


struct PixelOutput
{
    float4 blurredValue : SV_TARGET0;
};

PixelOutput PShader(PixelInput input)
{
    PixelOutput output;

    output.blurredValue = float4(0, 0, 0, 0);
    for (int i = -BlurAmount; i <= BlurAmount; ++i)
    {
        output.blurredValue += Texture.Sample(ss, input.texCoordsV[i + BlurAmount]) * input.weights[i + BlurAmount];
        output.blurredValue += Texture.Sample(ss, input.texCoordsH[i + BlurAmount]) * input.weights[i + BlurAmount];
    }

    return output;
}