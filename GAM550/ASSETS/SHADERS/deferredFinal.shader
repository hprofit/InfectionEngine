cbuffer DeferredFinalConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4 CameraPosition;
	float4 LightPosition;
	float4 LightColor;
	float4 LIHW;	// Intensity, Height, Width, NAN
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 view : VIEW;
};

Texture2D WorldPosTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D DiffuseTexture : register(t2);
Texture2D SpecularTexture : register(t3);
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
	position.w = 1;

	float4 P = mul(ModelMatrix, position);

	output.position = mul(MatFinal, position);
	output.view = CameraPosition - P;

	return output;
}


float4 PShader(
	float4 position : SV_POSITION,
	float4 view : VIEW
) : SV_TARGET
{
	float2 texCoords = float2(position.x / LIHW.z, position.y / LIHW.y);
	float4 color = float4(position.x / LIHW.z, position.y / LIHW.y, 0,1);
	return DiffuseTexture.Sample(ss, texCoords) * 5.0f;
}