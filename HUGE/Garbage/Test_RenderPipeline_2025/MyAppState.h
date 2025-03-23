#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "TRenderPipeline.h"

using namespace H;
using namespace H::Graphics;
 

class MyAppState : public H::AppState
{
public:
	 void Initialize();
	 void Terminate();
	 void Render();
	 void Update(float deltatime);
	 void DebugUI();

private:
	void RenderScene();
	void PostProcess();

private:
	const wchar_t* DO_LIGHTING_FILE_PATH = L"../../Assets/Shaders/DoLighting.fx";
	const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";
	const wchar_t* STANDARD_FILE_PATH = L"../../Assets/Shaders/Standard.fx";
	const wchar_t* STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/StandardInstanced.fx";
	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
	const wchar_t* PARTICLE_FILE_PATH = L"../../Assets/Shaders/Particle.fx";

	Camera mCamera;
	Camera* mCurrentCam;
	bool useLightCam = false;

	TSampleInstancedRendering mTInstanceDraw;
};

