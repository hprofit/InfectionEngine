cbuffer DeferredFinalConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4 CameraPosition;
	float4 LightPosition;
	float4 LightColor;
	float4 LIDHW;	// Intensity, Distance, Height, Width
};

struct VOut
{
	float4 position : SV_POSITION;
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

	return output;
}


float4 PShader(
	float4 position : SV_POSITION
) : SV_TARGET
{
	float2 texCoords = float2(position.x / LIDHW.w, position.y / LIDHW.z);

	float4 worldPos = WorldPosTexture.Sample(ss, texCoords);
	if (length(LightPosition - worldPos) > LIDHW.y) {
		return float4(0, 0, 0, 0);
	}


	float4 normal = normalize(NormalTexture.Sample(ss, texCoords));
	float4 diffuseColor = DiffuseTexture.Sample(ss, texCoords);
	float4 specularInfo = SpecularTexture.Sample(ss, texCoords);

	float4 L = normalize(LightPosition - worldPos);
	float4 v = normalize(CameraPosition - worldPos);
	float4 H = normalize(v + L);
	float specularCoef = specularInfo.w;
	float4 specularColor = float4(specularInfo.x, specularInfo.y, specularInfo.z, 1);

	float4 diffuse = max(dot(normal, L), 0) * diffuseColor * LightColor;
	float4 specular = pow(max(dot(H, normal), 0), specularCoef) * specularColor * LightColor;
	
	return NormalTexture.Sample(ss, texCoords) * 100;// diffuse + specular;
}