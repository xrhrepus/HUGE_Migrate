//====================================================================================================
// Filename:	Standard.fx
// Created by:	Peter Chan
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

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 materialPower;
}

cbuffer OptionalBuffer :register(b3)
{
	float4 optional;
   
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);


SamplerState textureSampler : register(s0);

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;

};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;

	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	
	float2 texCoord	: TEXCOORD2;

};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord , 0).x;
	float4 position = input.position + ( float4(input.normal, 0 ) *displacement* optional.x);
    float3 normal = mul(input.normal, (float3x3) World);
    float3 tangent = mul(normal, (float3x3) World);
    float3 binormal = cross(normal, tangent);

	output.position = mul(position, WVP);
 
	output.normal = mul(input.normal, (float3x3) World);
    output.tangent = tangent;
    output.binormal = binormal;
	output.dirToLight = -lightDirection;
	output.dirToView = normalize(ViewPosition.xyz - mul(position, World).xyz);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
     
    float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);
   

	float3 n  = normalize(input.normal);
    float3 b = normalize(input.binormal);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);

    float3 normal = normalize(mul(sampledNormal, tbnw)) * optional.y + n * (1 - optional.y);

    //float3 normal = normalize(input.normal);

	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float4 ambient = lightAmbient * materialAmbient;

 	float d = saturate(dot(dirToLight,  normal));
	float4 diffuse = d * lightDiffuse * materialDiffuse;



 	float3 r = reflect(lightDirection.xyz,  normal);
	float s = saturate(dot(dirToView, r));

	float specular = pow(s, materialPower.x) * lightSpecular * materialSpecular;
	float4 diffuseColor = diffuseMap.Sample(textureSampler,input.texCoord);

	float4 specularColor = specularMap.Sample(textureSampler, input.texCoord);


    return (ambient + diffuse) * diffuseColor  +  specular* specularColor.r;
	//return  specular;
}
