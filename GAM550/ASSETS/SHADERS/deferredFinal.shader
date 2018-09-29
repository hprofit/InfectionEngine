cbuffer DeferredFinalConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4 CameraPosition;
	float4 LightPosition;	// x, y, z, A
	float4 LightColor;		// r, g, b, B
	float4 LIDHW;	// Intensity, Distance, Height, Width
	float4 Ambient;
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

float falloff(float dist, float a, float b) 
{
	return 1.f / (1.f + 0.1f * dist + 0.01f * dist * dist);
}

float attenuation(float dist, float lightRadius)
{
	float val = 1.0f - ((dist*dist) / (lightRadius*lightRadius));
	return clamp(val, 0.0f, 1.0f);
}

float4 PShader(
	float4 position : SV_POSITION
) : SV_TARGET
{
	float2 texCoords = float2(position.x / LIDHW.w, position.y / LIDHW.z);
	float4 worldPos = WorldPosTexture.Sample(ss, texCoords);
	float a = LightPosition.w;
	float b = LightColor.a;
	float4 LPos = LightPosition;
	LPos.w = 1;

	float L_Length = length(LPos - worldPos);
	if (L_Length > LIDHW.y) {
		return float4(0, 0, 0, 0);
	}
	float4 LColor = LightColor;
	LColor *= falloff(L_Length, a, b) * LIDHW.x;
	LColor.a = 1;

	float4 normal = NormalTexture.Sample(ss, texCoords);
	// Return the w component to 0
	normal.w = 0;
	normal = normalize(normal);
	float4 diffuseColor = DiffuseTexture.Sample(ss, texCoords);
	float4 specularInfo = SpecularTexture.Sample(ss, texCoords);




	float4 L = normalize(LPos - worldPos);
	float4 v = normalize(CameraPosition - worldPos);
	float4 H = normalize(v + L);
	float specularCoef = specularInfo.w;
	float4 specularColor = float4(specularInfo.x, specularInfo.y, specularInfo.z, 1);

	float4 attVal = attenuation(L_Length, LIDHW.y);

	float4 ambient = diffuseColor * Ambient;
	float4 diffuse = max(dot(normal, L), 0) * diffuseColor * LColor;
	float4 specular = pow(max(dot(H, normal), 0), specularCoef) * specularColor * LColor;
	
	float4 finalColor = (diffuse + specular) * LIDHW.x * attVal;
	finalColor.a = 1;
	return finalColor;
}