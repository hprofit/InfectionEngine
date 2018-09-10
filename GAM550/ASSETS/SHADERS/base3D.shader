cbuffer ConstantBuffer
{
	float4x4 PerspectiveMatrix;
	float4x4 ViewMatrix;
	float4x4 ModelMatrix;
	float4x4 NormalMatrix;
	float4 CameraPosition;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(
	float4 position : POSITION,
	float4 normal : NORMAL,
	float4 tangent : TANGENT,
	float4 bitangent : BITANGENT,
	float2 texCoords : TEXCOORDS,
	float4 color : COLOR)
{
	VOut output;
	float4 P = ModelMatrix * position;
	output.position = PerspectiveMatrix * ViewMatrix * P;
	output.color = color;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}