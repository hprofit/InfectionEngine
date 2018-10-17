cbuffer ShadowAddLightCB
{
	float4x4 ShadowMatrix;
	float4x4 ModelMatrix;
	float4 CameraPosition;
	float4 LightPosition;	// x, y, z, A
	float4 LightColor;		// r, g, b, B
	float4 LIDHW;	// Intensity, Distance, Render Target Height, Render Target Width
    float Alpha;
    float MaxDepth;
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

float attenuation(float dist, float lightDistance)
{
    float val = 1.0f - ((dist * dist) / (lightDistance * lightDistance));
	return clamp(val, 0.0f, 1.0f);
}

float4 PShader(PixelInput input) : SV_TARGET
{
    float alpha = 1.f * pow(10.f, -3);

    float halfDepth = MaxDepth / 2.0f;
    float3 halfVec = float3(halfDepth, halfDepth, halfDepth);



	const float EPSILON = 0.0001f;
	float2 texCoords = float2(input.position.x / LIDHW.w, input.position.y / LIDHW.z);
    float4 worldPos = float4(WorldPosTexture.Sample(ss, texCoords).xyz, 1);


	float4 shadowPos = mul(ShadowMatrix, worldPos);
    float2 shadowMapUV = (shadowPos.xy / shadowPos.w) * 0.5f + 0.5f;
    shadowMapUV.y = 1.0f - shadowMapUV.y;

	float shadowDepth = ShadowMapTexture.Sample(ss, shadowMapUV).x;
	float pointDepth = shadowPos.w;

	// In Shadow from this light
    if (shadowDepth < (pointDepth - EPSILON) || shadowPos.w <= 0 ||
		shadowMapUV.x < 0 || shadowMapUV.x > 1 ||
		shadowMapUV.y < 0 || shadowMapUV.y > 1)
    {
        return float4(0, 0, 0, 0);
    }
	
    // Else, do lighting equation as normal
    float lightDistance = LIDHW.y;
	float4 LPos = float4(LightPosition.xyz, 1);
	float L_Length = length(LPos - worldPos);

	// If the current point is beyond the light's reach, it's not light by this light
	if (L_Length > lightDistance)
    { 
        return float4(0, 0, 0, 0); 
    }
	
	float a = LightPosition.w;
	float b = LightColor.w;
	float lightIntensity = LIDHW.x;
    float4 LColor = float4((LightColor * lightIntensity).xyz, 1);

    float4 normal = normalize(float4(NormalTexture.Sample(ss, texCoords).xyz, 0));
	float4 diffuseColor = DiffuseTexture.Sample(ss, texCoords);
	float4 specularInfo = SpecularTexture.Sample(ss, texCoords);




	float4 L = normalize(LPos - worldPos);
	float4 v = normalize(CameraPosition - worldPos);
	float4 H = normalize(v + L);
	float specularCoef = specularInfo.w;
	float4 specularColor = float4(specularInfo.xyz, 1);

    float4 attVal = 1.0f; //attenuation(L_Length, lightDistance);
	float4 diffuse = max(dot(normal, L), 0) * diffuseColor * LColor;
	float4 specular = pow(max(dot(H, normal), 0), specularCoef) * specularColor * LColor;
    
    return float4(((diffuse + specular) * attVal).xyz, 1);
}