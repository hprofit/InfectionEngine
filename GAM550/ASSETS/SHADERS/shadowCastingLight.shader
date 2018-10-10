cbuffer ConstantBuffer
{
	float4x4 MatFinal;
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
	float depth : DEPTH;
};

Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;
	output.position = mul(MatFinal, input.position);
	output.depth = output.position.z / output.position.w;
	return output;
}


struct POut
{
	float4 depth : SV_TARGET0;
};

POut PShader(PixelInput input)
{
	POut output;
	output.depth = float4(input.depth, input.depth, input.depth, 1); // <- Get rid of this when done testing
	return output;
}