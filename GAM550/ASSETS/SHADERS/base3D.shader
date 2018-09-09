struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(
	float4 position : POSITION, 
	float4 position : NORMAL, 
	float4 tangent : TANGENT, 
	float4 bitangent : BITANGENT, 
	float2 texCoords : TEXCOORDS, 
	float4 color : COLOR)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}