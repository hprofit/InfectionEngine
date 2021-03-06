cbuffer ConstantBuffer
{
	float4x4 MatFinal;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	float4 CameraPosition;
	float4 OverrideColor;
	float4 SpecularValues;
	bool CastShadows;
	bool ReceiveShadows;
	bool IsLit;
	unsigned int TextureFlags;
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
	float3 worldPosition : TEXCOORD0;
	float4 normal : NORMAL;
	float3x3 tbn : TBN;
	float4 color : COLOR;
	float2 texCoords : TEXCOORD1;
	float depth : DEPTH;

	bool hasDiffuseTexture : HAS_DIFFUSE_TEXTURE;
	bool hasNormalMap : HAS_NORMAL_MAP;
	bool hasSpecularMap : HAS_SPECULAR_MAP;
	bool hasColorOverride : HAS_COLOR_OVERRIDE;
	bool hasTint : HAS_TINT;
};

Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
SamplerState ss;


PixelInput VShader(VertexInput input)
{
	PixelInput output;
	input.normal.w = 0;
	input.tangent.w = 0;
	input.bitangent.w = 0;

	float3 T = normalize(mul(NormalMatrix, input.tangent)).xyz;
	float3 B = normalize(mul(NormalMatrix, input.bitangent)).xyz;
	float3 N = normalize(mul(NormalMatrix, input.normal)).xyz;

	float4 worldPosition = mul(ModelMatrix, input.position);
	output.worldPosition = worldPosition.xyz;
	output.position = mul(MatFinal, input.position);
	output.normal = mul(NormalMatrix, input.normal);
	output.tbn = transpose(float3x3(T, B, N));
	output.color = input.color;
	output.texCoords = input.texCoords;

	output.depth = output.position.z / output.position.w;

	unsigned int DIFFUSE_TEXTURE = 1;
	unsigned int NORMAL_MAPPED = 2;
	unsigned int SPECULAR_MAPPED = 4;
	unsigned int COLOR_OVERRIDE = 8;
	unsigned int TINT_COLOR = 16;
	output.hasDiffuseTexture = TextureFlags & DIFFUSE_TEXTURE;
	output.hasNormalMap = (TextureFlags & NORMAL_MAPPED) >> 1;
	output.hasSpecularMap = (TextureFlags & SPECULAR_MAPPED) >> 2;
	output.hasColorOverride = (TextureFlags & COLOR_OVERRIDE) >> 3;
	output.hasTint = (TextureFlags & TINT_COLOR) >> 4;

	return output;
}

[maxvertexcount(4)]
void GeoShader(point PixelInput input[1], inout TriangleStream<PixelInput> SpriteStream)
{
	PixelInput output;
}

struct POut
{
	float4 worldPos : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 diffuse : SV_TARGET2;
	float4 specular : SV_TARGET3;
	float4 depth : SV_TARGET4;  // <- Get rid of this when done testing
};

POut PShader(PixelInput input)
{
	POut output;

	if (input.hasNormalMap) {
		float3 normalMapValue = NormalMap.Sample(ss, input.texCoords).rgb;
		float3 nMapVal = normalize(
			float3(
				normalMapValue.r * 2.0 - 1.0,
				normalMapValue.g * 2.0 - 1.0,
				normalMapValue.b * 2.0 - 1.0
			)
		);
		float3 m = normalize(mul(input.tbn, nMapVal));

		output.normal = float4(m.x, m.y, m.z, 0);
		// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
		output.normal.w = 1;
	}
	else {
		input.normal.w = 0;
		output.normal = normalize(input.normal);
		// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
		output.normal.w = 1;
	}

	output.worldPos = float4(input.worldPosition.xyz, 1);
	// Set the alpha channel to 1 so we can see it when GBuffer is rendered 
	//output.worldPos.w = 1; // input.depth; <- put this back when done testing
	output.depth = float4(input.position.w, input.position.w, input.position.w, 1); // <- Get rid of this when done testing

	output.diffuse = input.hasDiffuseTexture ? DiffuseTexture.Sample(ss, input.texCoords) : (input.hasColorOverride ? OverrideColor : input.color);
	//if (input.hasTint)
	//	output.diffuse *= TintColor;
	output.specular = SpecularValues;

	return output;
}