//====================================================================================================
// Filename:	DrawEdge_postprocess.fx
// Author: RickH
// From : https://halisavakis.com/my-take-on-shaders-edge-detection-image-effect/
// Description:	Step2 :Postprocessing step of edge draw.
//====================================================================================================

 
Texture2D originSceneTex : register(t0);
Texture2D depthNormalTex : register(t1);

SamplerState textureSampler : register(s0);

cbuffer EdgeParamBuffer : register(b0)
{
	//threshold
    float4 edgeColor;
    float threshold;
	float2 uv_step;
    float edgeColorWeight;
 
}

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};
 
VS_OUTPUT VS (VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}
float4 ReadDepthNormal(float2 uv)
{
 	return depthNormalTex.Sample(textureSampler, uv);
}


float4 PS (VS_OUTPUT input) : SV_Target
{
	float4 thisColor = originSceneTex.Sample(textureSampler, input.texCoord);
    //float4 thisColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 thisDN = ReadDepthNormal(input.texCoord);

	float2 pixAround[8] =
	{
		float2(-1,1), float2(0,1), float2(1,1),
		float2(-1,0),			   float2(1,1),
		float2(-1,-1),float2(0,-1),float2(1,-1)
	};
	float4 dnAround = float4(0,0,0,0);

	for (int i = 0; i < 8; ++i)
	{
        dnAround += ReadDepthNormal(input.texCoord + pixAround[i] * uv_step.xy);
     }
	dnAround /= 8;
         
    float4 colorAround = mul((1.0f - edgeColorWeight), mul(thisColor, 0.8f)) + mul(edgeColorWeight, edgeColor);
    colorAround.w = 1.0f;
    //prev
    //float4 finalCol = lerp(thisColor,edgeColor, step(threshold, length(thisDN - dnAround)));

 	//========
    float4 finalCol = lerp(thisColor, colorAround, step(threshold, length(thisDN - dnAround)));
 
    return finalCol;
 

}
