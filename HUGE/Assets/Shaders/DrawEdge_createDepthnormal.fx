//====================================================================================================
// Filename:	DrawEdge_createDepthNormal.fx
// Author: RickH
// From : https://halisavakis.com/my-take-on-shaders-edge-detection-image-effect/
// Description:	Step1 : Create a depth normal map for edge draw
//====================================================================================================
cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
}
 
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
    float4 normalDepth : TEXCOORD0;
};

VS_OUTPUT VS (VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	float4 ndcPos = mul(float4(input.position, 1.0f), WVP);
	float depth = ndcPos.z / ndcPos.w;

    output.position = ndcPos;
  
    float3 wnormal = mul(input.normal, (float3x3)World);
    
    output.normalDepth.xyz = normalize(abs(wnormal.xyz));//save negative value to compute
    //output.normalDepth.x = wnormal.x * 2.0f - 1.0f; //save negative value to compute
    //output.normalDepth.y = wnormal.y * 2.0f - 1.0f; //save negative value to compute
    //output.normalDepth.z = wnormal.z * 2.0f - 1.0f; //save negative value to compute
    
    output.normalDepth.w = depth;
    
    return output;
}

float4 PS (VS_OUTPUT input) : SV_Target
{
     return input.normalDepth;
}
