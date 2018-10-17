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
};

Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;
	output.position = mul(MatFinal, input.position);
	return output;
}


struct PixelOutput
{
	float4 depth : SV_TARGET0;
};

PixelOutput PShader(PixelInput input)
{
    PixelOutput output;
    float d2 = input.position.w * input.position.w;
    float d3 = d2 * input.position.w;
    float d4 = d3 * input.position.w;
    //output.depth = float4(input.position.w, d2, d3, d4);
    output.depth = float4(input.position.w + 0.5f, input.position.w, input.position.w, 1);
	return output;
}