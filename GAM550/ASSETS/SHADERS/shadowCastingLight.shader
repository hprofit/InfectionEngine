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
	float4 shadowPos : S_POS;
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
	output.shadowPos = output.position;
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
	float depth = input.shadowPos.z / input.shadowPos.w;

	float d2 = depth;// input.depth * input.depth;
	float d3 = d2 * input.depth;
	float d4 = d3 * input.depth;
	output.depth = float4(input.depth, d2, d3, d4); // <- Get rid of this when done testing
	//output.depth = float4(input.depth, input.depth, input.depth, 1); // <- Get rid of this when done testing
	return output;
}