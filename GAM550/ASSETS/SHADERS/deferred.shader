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
	float4 worldPosition : WORLD_POS;
	float4 normal : NORMAL;
	float3x3 tbn : TBN;
	float4 color : COLOR;
	float2 texCoords : TEXCOORDS;
};

Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
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
	normal.w = 0;
	tangent.w = 0;
	bitangent.w = 0;

	float4 n = mul(NormalMatrix, normal);
	float4 b = mul(NormalMatrix, bitangent);
	float4 t = mul(NormalMatrix, tangent);

	float3 T = normalize(mul(ModelMatrix, tangent)).xyz;
	float3 B = normalize(mul(ModelMatrix, bitangent)).xyz;
	float3 N = normalize(mul(ModelMatrix, normal)).xyz;

	output.worldPosition = mul(ModelMatrix, position);
	output.position = mul(MatFinal, position);
	output.normal = mul(NormalMatrix, normal);
	output.tbn = float3x3(T, B, N);
	output.color = color;
	output.texCoords = texCoords;

	return output;
}





struct POut
{
	float4 worldPos : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 diffuse : SV_TARGET2;
	float4 specular : SV_TARGET3;
};

POut PShader(
	float4 position : SV_POSITION, 
	float4 worldPosition : WORLD_POS,
	float4 normal : NORMAL, 
	float3x3 tbn : TBN,
	float4 color : COLOR,
	float2 texCoords : TEXCOORDS
)
{
	POut output;

	output.worldPos = worldPosition;
	// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
	output.worldPos.w = 1;
	output.normal.w = 0;
	output.normal = normalize(normal);
	// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
	output.normal.w = 1;
	output.diffuse = Textured ? DiffuseTexture.Sample(ss, texCoords) : color;
	output.specular = float4(0.9, 0.9, 0.9, 60);

	return output;
}