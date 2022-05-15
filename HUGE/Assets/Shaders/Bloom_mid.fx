//====================================================================================================
// Filename:	Bloom_mid.fx
// RickH
// Description:	mid quality bloom
//====================================================================================================

 
Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

cbuffer BloomParamBuffer : register(b0)
{
	//threshold
    float bloomThreshold;
	//combine
    float bloomIntensity;
    float baseIntensity;
    float bloomSaturation;
    float baseSaturation;
	//simple blur
	float u_step;
	float v_step;
    float radiation;
    float decay; // 0 ~ 1
    float blurIntensity;
    float offset;//0 ~ 10

}

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

float Luminance(float4 color)
{
    return (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);

}
//bloom function
float4 SimpleBlur(VS_OUTPUT input)
{
	//const float mid_offset = 2.9f;
	//const float mid_radiation = 0.066f;

	const float offsetXustep = 0.002265625f;
	const float offsetXvstep = 0.004027778f;

 // 
	const int pxNum_u = 29;
	const int pxNum_v = 16;
    float4 avg;
    avg.xyzw = 0.0f;
    int count = 0;
    float4 temp = 0;

    float maxIntensity = 1.0;
 
    float t_intensity;//weight accord to distance
    float d_rad = 0.066f ;//decreasing radiation
 
 
	[unroll (30)]
	for (int u = 1; u < 30; ++u)
    {
        float uXustep = u * offsetXustep;
        d_rad -= offsetXvstep;
        pxNum_v -= 1;
        float2 hl = { input.texCoord.x - uXustep, input.texCoord.y };
        
        float2 hr = { input.texCoord.x + uXustep, input.texCoord.y };

        t_intensity = maxIntensity - decay * u;

 
        avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, hl) - bloomThreshold);
 

        avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, hr) - bloomThreshold);
        
         
        count += 2;
		
        [unroll ]
        for (int v = 1; v < 7; v++)
        {
            float vXvstep = d_rad - (v * offsetXvstep);
            t_intensity = maxIntensity - decay * v;
 
 

            avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(hl.x, hl.y - vXvstep)) - bloomThreshold);
 

            avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(hl.x, hl.y + vXvstep)) - bloomThreshold);
 

            avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(hr.x, hr.y - vXvstep)) - bloomThreshold);
 

            avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(hr.x, hr.y + vXvstep)) - bloomThreshold);
           
         
            count += 4;

        }
    }
    //compute the centre
	[unroll (17)]
    for (int v = 1; v < 17; ++v)
    {
        float vXvstep = radiation - (v * offsetXvstep);
        t_intensity = maxIntensity - decay * v;
   
 

        avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(input.texCoord.x, input.texCoord.y - vXvstep)) - bloomThreshold);
 

        avg += t_intensity * saturate(diffuseMap.Sample(textureSampler, float2(input.texCoord.x, input.texCoord.y + vXvstep)) - bloomThreshold);
          
        count += 2;
    }

    avg /= count;
     
 
    float4 thispix = diffuseMap.Sample(textureSampler, input.texCoord) + blurIntensity * avg;

	return thispix;


}

//extract pixel according to threshold
float4 BloomExtract(VS_OUTPUT input)
{
	float4 color = SimpleBlur(input);
    return saturate( (color - bloomThreshold) / (1 - bloomThreshold));
	return color;
}

//adjust saturation
float4 AdjustSaturation(float4 color, float saturation)
{
    float grey = dot(color.xyz, float3(0.3, 0.59, 0.11));
    return lerp(grey, color, saturation);
}
 

VS_OUTPUT VS (VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = input.position;
    output.texCoord = input.texCoord;
    return output;
}

float4 PS (VS_OUTPUT input) : SV_Target
{
    float4 base = diffuseMap.Sample(textureSampler, input.texCoord); //origin scene
	
    float4 bloom = BloomExtract(input);
 

    //return base;
   

	
    bloom = AdjustSaturation(bloom, bloomSaturation) * bloomIntensity;
    return bloom;
 

    //base = AdjustSaturation(base, baseSaturation) * baseIntensity;
    //base *= (1 - saturate(bloom));


    // return base + bloom;


   

}
