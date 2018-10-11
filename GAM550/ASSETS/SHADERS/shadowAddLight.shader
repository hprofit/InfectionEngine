cbuffer ShadowAddLightCB
{
	float4x4 ShadowMatrix;
	float4x4 ModelMatrix;
	float4 CameraPosition;
	float4 LightPosition;	// x, y, z, A
	float4 LightColor;		// r, g, b, B
	float4 LIDHW;	// Intensity, Distance, Render Target Height, Render Target Width
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
	float2 texCoords : TEXCOORDS;
};

Texture2D WorldPosTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D DiffuseTexture : register(t2);
Texture2D SpecularTexture : register(t3);
Texture2D ShadowMapTexture : register(t5);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;
	input.position.w = 1;

	output.position = mul(ModelMatrix, input.position);
	output.texCoords.x = input.texCoords.x;
	output.texCoords.y = 1.0f - input.texCoords.y;

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

float4 PShader(PixelInput input) : SV_TARGET
{
	const float EPSILON = 0.0001f;
	float2 texCoords = float2(input.position.x / LIDHW.w, input.position.y / LIDHW.z);
	float4 worldPos = WorldPosTexture.Sample(ss, texCoords);
	worldPos.w = 1;


	float4 shadowPos = mul(ShadowMatrix, worldPos);
	float3 shadowPosDiv = (shadowPos.xyz / shadowPos.w) * 0.5f + 0.5f;
	float2 shadowMapUV = shadowPosDiv.xy;
	shadowMapUV.y = 1.0f - shadowMapUV.y;
	float shadowDepth = ShadowMapTexture.Sample(ss, shadowMapUV).x;
	float pointDepth = shadowPosDiv.z;

	//return float4(input.position.xy, 0, 1);
	//return float4(input.texCoords.x * LIDHW.w, (1.0f - input.texCoords.y) * LIDHW.z, 0, 1);
	//return float4(shadowPosDiv.z, shadowPosDiv.z, shadowPosDiv.z, 1);
	//return float4(shadowDepth, shadowDepth, shadowDepth, 1);
	//return float4(pointDepth, pointDepth, pointDepth, 1);

	//shadowMapUV.y = t;
	// In Shadow from this light
	if (shadowDepth < (pointDepth - EPSILON) || shadowPos.w <= 0 || 
		shadowMapUV.x < 0 || shadowMapUV.x > 1 || 
		shadowMapUV.y < 0 || shadowMapUV.y > 1) 
	{
		return float4(0,0,0,0);
	}
	// Else, do lighting equation as normal

	float a = LightPosition.w;
	float b = LightColor.w;
	float lightDistance = LIDHW.y;
	float lightIntensity = LIDHW.x;
	float4 LPos = float4(LightPosition.xyz, 1);
	float L_Length = length(LPos - worldPos);
	//if (L_Length > lightDistance) { return float4(0, 0, 0, 0); }
	float4 LColor = LightColor * lightIntensity;
	//LColor *= falloff(L_Length, a, b);
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

	float4 attVal = attenuation(L_Length, lightDistance);

	float4 diffuse = max(dot(normal, L), 0) * diffuseColor * LColor;
	float4 specular = pow(max(dot(H, normal), 0), specularCoef) * specularColor * LColor;

	float4 finalColor = (diffuse + specular);// *attVal;
	finalColor.a = 1;
	return finalColor;
}