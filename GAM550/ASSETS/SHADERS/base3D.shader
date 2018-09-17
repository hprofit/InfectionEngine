cbuffer ConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	//bool CastShadows;
	//bool ReceiveShadows;
	int IsLit;
	float4 CameraPosition;
	float4 LightPosition;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float3x3 tbn : TBN;
	float4 view : VIEW;
	float4 light : LIGHT;
	float4 color : COLOR;
	float2 texCoords : TEXCOORDS;
	//bool castShadows : CASTSHADOWS;
	//bool receiveShadows : RECEIVESHADOWS;
	//bool isLit : ISLIT;
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
	position.w = 1;
	normal.w = 0;
	tangent.w = 0;
	bitangent.w = 0;

	float4 P = mul(ModelMatrix, position);

	float4 n = mul(NormalMatrix, normal);
	float4 b = mul(NormalMatrix, bitangent);
	float4 t = mul(NormalMatrix, tangent);

	float3 T = normalize(mul(ModelMatrix, tangent)).xyz;
	float3 B = normalize(mul(ModelMatrix, bitangent)).xyz;
	float3 N = normalize(mul(ModelMatrix, normal)).xyz;


	output.position = mul(MatFinal, position);
	output.normal = mul(NormalMatrix, normal);
	output.tbn = float3x3(T, B, N);
	output.view = CameraPosition - P;
	output.light = LightPosition - P;
	output.color = color;
	output.texCoords = texCoords;
	//output.castShadows = cb.CastShadows;
	//output.receiveShadows = cb.ReceiveShadows;
	//output.isLit = cb.IsLit;

	return output;
}


float4 PShader(
	float4 position : SV_POSITION, 
	float4 normal : NORMAL, 
	float3x3 tbn : TBN,
	float4 view : VIEW,
	float4 light : LIGHT,
	float4 color : COLOR,
	float2 texCoords : TEXCOORDS
	//bool castShadows : CASTSHADOWS,
	//bool receiveShadows : RECEIVESHADOWS,
	//bool isLit : ISLIT
) : SV_TARGET
{
	float4 finalColor;
	bool isLit = true;
	if (isLit) {
		float4 m = normalize(normal);
		float4 L = normalize(light);
		float4 v = normalize(view);
		float4 H = normalize(v + L);
		float specularCoef = 100;
		float4 specularColor = float4(1, 1, 1, 1);
		float4 lightColor = float4(1, 1, 1, 1);

		float4 ambient = color * float4(0.1, 0.1, 0.1, 1);
		float4 diffuse = max(dot(m, L), 0) * Texture.Sample(ss, texCoords) * lightColor;
		float4 specular = pow(max(dot(H, m), 0), specularCoef) * specularColor * lightColor;

		finalColor = diffuse + specular + ambient;
	}
	else {
		finalColor = Texture.Sample(ss, texCoords);
	}
	finalColor.w = 1;
	return finalColor;
}