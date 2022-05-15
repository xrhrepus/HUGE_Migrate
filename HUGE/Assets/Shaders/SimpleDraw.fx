//====================================================================================================
// Filename:	DoNothing.fx
// Created by:	Peter Chan
// Description: Shader for simple geo
//====================================================================================================

//====================================================================================================
// Vertex Shader
//====================================================================================================
cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

//float4 VS( float4 Pos : POSITION ) : SV_POSITION
//{
//	return Pos;
//}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, wvp);
	output.color = input.color;
	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input ) : SV_Target
{
	return input.color;	// Yellow, with Alpha = 1
}
