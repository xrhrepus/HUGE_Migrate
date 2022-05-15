//====================================================================================================
// Filename:	PostprocessingCombine.fx
// Description:	Combine all render target for final Postprocessing.
//====================================================================================================

 
Texture2D originTex : register(t0);
Texture2D bloomTex : register(t1);

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
	float4 color = originTex.Sample(textureSampler, input.texCoord);
		   color += bloomTex.Sample(textureSampler, input.texCoord);
    return color;
	
}
