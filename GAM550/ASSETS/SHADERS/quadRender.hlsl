cbuffer ConstantBuffer
{
	float4x4 ModelMatrix;
	float4 Ambient;
};

struct VertexInput {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 bitangent : BITANGENT;
	float2 texCoords : TEXCOORDS;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

Texture2D Texture;
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;

	output.position = mul(ModelMatrix, input.position);
	output.texCoords.x = input.texCoords.x;
	output.texCoords.y = 1.0f - input.texCoords.y;

	return output;
}

float4 PShader(PixelInput input) : SV_TARGET
{
    return Texture.Sample(ss, input.texCoords) * Ambient;
}