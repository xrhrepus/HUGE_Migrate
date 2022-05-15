//====================================================================================================
// Filename:	Postprocessing_Nothing.fx
// Created by:	Rick H
// Description:	For Postprocessing , output the origin img of SRView.
//====================================================================================================

 
Texture2D diffuseMap : register(t0);
 
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position	: POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = input.position;
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	//float4 color = diffuseMap.Sample(textureSampler, input.texCoord);
 
    return  diffuseMap.Sample(textureSampler, input.texCoord);
	
}
