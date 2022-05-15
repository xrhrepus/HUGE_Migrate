//====================================================================================================
// Filename:	Standard.fx
// Created by:	 
// Description: Standard Shader 
//====================================================================================================

// HLSL
cbuffer TransformBuffer : register(b0)
{
	matrix World;// combined matrix
	matrix WVP;// combined matrix
	float4 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;

}
 
SamplerState textureSampler : register(s0);

struct VSInput
{
	float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD0;

};
 
VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
    output.position = mul(input.position, WVP);
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

 //   float4 color;
 //   float v = 1.0f - (input.position.z / input.position.w);
 //   color.r = v * 10.0f;
 //   color.g = 1.0f;
 //   color.b = 1.0f;
 //   color.a = 1.0f;
 //   
 //   //return input.position.z;
	//return color;
    
    //return pout.depth;
	//return  specular;
}
