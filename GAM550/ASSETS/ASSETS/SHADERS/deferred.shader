cbuffer ConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	float4 CameraPosition;
	bool CastShadows;
	bool ReceiveShadows;
	bool IsLit;
	unsigned int TextureFlags;
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

	float3 T = normalize(mul(NormalMatrix, tangent)).xyz;
	float3 B = normalize(mul(NormalMatrix, bitangent)).xyz;
	float3 N = normalize(mul(NormalMatrix, normal)).xyz;

	output.worldPosition = mul(ModelMatrix, position);
	output.position = mul(MatFinal, position);
	output.normal = mul(NormalMatrix, normal);
	output.tbn = transpose(float3x3(T, B, N));
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
	unsigned int DIFFUSE_TEXTURE = 1;
	unsigned int NORMAL_MAPPED = 2;
	unsigned int SPECULAR_MAPPED = 4;
	bool hasDiffuseTexture = TextureFlags & DIFFUSE_TEXTURE;
	bool hasNormalMap = (TextureFlags & NORMAL_MAPPED)>>1;
	bool hasSpecularMap = (TextureFlags & SPECULAR_MAPPED)>>2;

	if (hasNormalMap) {
		float3 normalMapValue = NormalMap.Sample(ss, texCoords).rgb;
		float3 nMapVal = normalize(
			float3(
				normalMapValue.r * 2.0 - 1.0,
				normalMapValue.g * 2.0 - 1.0,
				normalMapValue.b * 2.0 - 1.0
			)
		);
		float3 m = normalize(mul(tbn, nMapVal));

		output.normal = float4(m.x, m.y, m.z, 0);
		// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
		output.normal.w = 1;
	}
	else {
		normal.w = 0;
		output.normal = normalize(normal);// float4(m.x, m.y, m.z, 0);
		// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
		output.normal.w = 1;
	}


	output.worldPos = worldPosition;
	// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
	output.worldPos.w = 1;
	output.diffuse = hasDiffuseTexture ? DiffuseTexture.Sample(ss, texCoords) : color;
	output.specular = float4(0.9, 0.9, 0.9, 100);

	return output;
}