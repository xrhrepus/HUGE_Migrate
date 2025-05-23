// Description: Standard shader supports instanced rendering

// indices in structured buffer of each different materials
cbuffer RenderInstanceData : register(b10)
{
    int lightIndex;
    int materialIndex;
    int diffuseIndex;
    int padding;
};

// each mesh is drawn instanced, INSTANCE_ID is the index
struct TransformData
{
    matrix World;
    matrix WVP;
    float3 ViewPosition;
    float pad;
};
StructuredBuffer<TransformData> TransformBuffer : register(t7);

// this allows each material to use different DL. but technically they should use the same one
struct DirectionalLightData
{
    float3 LightDirection;
    float pad;
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
};
StructuredBuffer<DirectionalLightData> DirectionalLightBuffer : register(t8);

// structured buffer doesn't need to be 16 byte aligned, padding can be removed
struct MaterialData
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialPower;
    float3 pad;
};
StructuredBuffer<MaterialData> MaterialBuffer : register(t9);

struct ShadowData
{
	matrix WVPLight;
};
StructuredBuffer<ShadowData> ShadowBuffer : register(t10);

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
	float aoMapWeight : packoffset(c0.w);
	float brightness : packoffset(c1.x);
	bool useShadow : packoffset(c1.y);
	float depthBias : packoffset(c1.z);
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D aoMap : register(t4);
Texture2D depthMap : register(t5);

SamplerState textureSampler : register(s0);
	
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
	float4 positionNDC : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input, uint svInstanceId : SV_InstanceID)
{
	VS_OUTPUT output;
	
    TransformData tf = TransformBuffer[svInstanceId];
    matrix World = tf.World;
    matrix WVP = tf.WVP;
    float3 ViewPosition = tf.ViewPosition;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal * displacement * bumpMapWeight);
    float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)World);
	float3 worldTangent = mul(input.tangent, (float3x3)World);

    output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
    output.dirToLight = -DirectionalLightBuffer[lightIndex].LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	if (true)
        output.positionNDC = mul(float4(localPosition, 1.0f), ShadowBuffer[svInstanceId].WVPLight);
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	// Renormalize normals from vertex shader
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
		float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
		float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
		normal = mul(normalSampled, TBNW);
	}

    MaterialData md = MaterialBuffer[materialIndex];
    DirectionalLightData dl = DirectionalLightBuffer[lightIndex];
	
    float4 ambient = dl.LightAmbient * md.MaterialAmbient;
	if (aoMapWeight != 0.0f)
		ambient += aoMap.Sample(textureSampler, input.texCoord);

	float diffuseIntensity = saturate(dot(dirToLight, normal));
    float4 diffuse = diffuseIntensity * dl.LightDiffuse * md.MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = saturate(dot(halfAngle, normal));
    float specularIntensity = pow(specularBase, md.MaterialPower);
    float4 specular = specularIntensity * dl.LightSpecular * md.MaterialSpecular;

    float4 textureColor = diffuseMap.Sample(textureSampler,input.texCoord);
	float specularFactor = 1.0f;
	if (specularMapWeight > 0.0f)
		specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * textureColor * 1 + specular * specularFactor;

	if (dot(DirectionalLightBuffer[0].LightDirection, normal) < 0.0f)
	{
		// NDC:   +---------+        UV: +----------->
		//       /   1     /|            |(0, 0)    |
		//      +----|----+ |            |          |            
		//      |    |    | |            |          |
		//   -1 -----+----- 1            |          |
		//      |    |    |/             |----------+
		//      +----|----+              V        (1, 1)
		//          -1
		float actualDepth = 1.0f - (input.positionNDC.z / input.positionNDC.w);
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;
		if (saturate(shadowUV.x) == shadowUV.x &&
			saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
			if (savedDepth > actualDepth + 0.008f)
				color = ambient * textureColor;
                //color = float4(0.0f,0.0f,0.0f,1.0f);
		}
	}

	return color;
}