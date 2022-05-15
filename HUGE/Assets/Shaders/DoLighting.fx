//====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transform n lighting.
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
	float4 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;

}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 materialPower;
}

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position, WVP);

	float4 ambient = lightAmbient * materialAmbient;

	float3 dirToLight = -lightDirection.xyz ;
	float3 normalInWorld = mul(input.normal, (float3x3)World);
	float d = saturate(dot(dirToLight, normalInWorld));
	float4 diffuse = d * lightDiffuse * materialDiffuse;

	float3 dirToView = normalize(ViewPosition.xyz - mul(input.position, World).xyz);
	float3 r = reflect(lightDirection.xyz, normalInWorld);
	float s = saturate(dot(dirToView, r));
    float specular = pow(s, materialPower.x) * lightSpecular * materialSpecular;

	output.color = ambient + diffuse + specular;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}
