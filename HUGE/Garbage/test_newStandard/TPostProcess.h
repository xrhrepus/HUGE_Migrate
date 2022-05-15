#pragma once
#include <HUGE/Inc/HUGE.h>

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

class TPostProcess
{
public:
	void Initialize();
	void Terminate();
	void Begin();
	void End();
	void Render();

	void DebugUI() ;

private:
	const std::filesystem::path POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const std::filesystem::path POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const std::filesystem::path POSTPROCESSING_MID_FILE_PATH = L"../../Assets/Shaders/Bloom_mid.fx";
	const std::filesystem::path POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";

	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

	// origin scene
 	RenderTarget mOriginRT;
	VertexShader mOriginVS;
	PixelShader mOriginPS;
	
	//bloom
	bool isBloomOn = false;
	BloomParamBuffer mBloomBuffer;
	BloomParamData mBloomData;

	//bloom
	RenderTarget mBloomRT;// 
	VertexShader mBloomVS;
	PixelShader mBloomPS;
	//bloomMid
	RenderTarget mBloomMidRT;// 
	VertexShader mBloomMidVS;
	PixelShader mBloomMidPS;

	//combine
	RenderTarget mCombineRT;// 
	VertexShader mCombineVS;
	PixelShader mCombinePS;

};

 
