#pragma once
#include <HUGE\Inc\HUGE.h>
using namespace H::Graphics;

struct BloomParamData
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
	float decay;// 0 ~ 1
	float blurIntensity;
	float offset;
	float padding;
	//// GaussianBlur
	//Vector2 sampleOffset[15];
	//float sampleWeight[15];

};
using BloomParamBuffer = TypedConstantBuffer<BloomParamData>;

class TBloom
{
  public:
	void Initialize();
	void Terminate();

	void BloomOriginBegin();
	void BloomOriginEnd();
	void Render();


 	void DebugUI();

	RenderTarget mRT_Bloom;
	RenderTarget mRT_Combine;

private:

	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
private:
	
	VertexShader mVS_Origin;
	PixelShader mPS_Origin;
	RenderTarget mRT_Origin;

	bool isBloomOn = true;
	VertexShader mVS_Bloom;
	PixelShader mPS_Bloom;

	VertexShader mVS_Combine;
	PixelShader mPS_Combine;
	
	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

	BloomParamData mData;
	BloomParamBuffer mDataBuffer;
};

 