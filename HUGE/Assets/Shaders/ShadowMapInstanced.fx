//====================================================================================================
// Filename:	Standard.fx
// Created by:	 
// Description: Standard Shader 
//====================================================================================================

struct TransformData
{
    matrix World;
    matrix WVP;
    float3 ViewPosition;
    float pad;
};
StructuredBuffer<TransformData> TransformBuffer : register(t7);

StructuredBuffer<TransformData> LightSourceTransformBuffer : register(t8);


struct VSInput
{
	float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD0;

};
 
VSOutput VS(VSInput input, uint svInstanceId : SV_InstanceID)
{
	VSOutput output = (VSOutput)0;
    //output.position = mul(float4(input.position,1.0f), LightSourceTransformBuffer[0].WVP);
	output.position = mul(float4(input.position,1.0f), TransformBuffer[svInstanceId].WVP);
 	output.positionNDC = output.position;

  
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 color;
	float depth = 1.0f - (input.positionNDC.z / input.positionNDC.w);
	color.r = depth;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	return float4(depth,1.0f, 1.0f, 1.0f);
	//return float4(1.0f,1.0f, 1.0f, 1.0f);
}
