cbuffer ConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	float4 CameraPosition;
	bool CastShadows;
	bool ReceiveShadows;
	bool IsLit;
	bool Textured;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

Texture2D Texture;
SamplerState ss;


VOut VShader(
	float4 position : POSITION,
	float4 normal : NORMAL,
	float4 tangent : TANGENT,
	float4 bitangent : BITANGENT,
	float2 texCoords : TEXCOORDS,
	float4 color : COLOR
)
{
	VOut output;

	output.position = mul(ModelMatrix, position);
	output.texCoords.x = output.position.x * 0.5 + 0.5;
	output.texCoords.y = output.position.y * -0.5 + 0.5;

	return output;
}

float4 PShader(
	float4 position : SV_POSITION,
	float2 texCoords : TEXCOORDS
) : SV_TARGET
{
	return Texture.Sample(ss, texCoords);
}