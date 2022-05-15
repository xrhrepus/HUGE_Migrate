#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "TPostProcess.h"
#include "TMeshRenderer.h"
#include "TRenderObject.h"
#include "TestShadow.h"
#include "TDepthMap.h"

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
	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
	const wchar_t* PARTICLE_FILE_PATH = L"../../Assets/Shaders/Particle.fx";


	Camera mCamera;
  	BloomParamBuffer mBloomBuffer;
	//
	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptionsData;
 
//
	Mesh mSphere;
	MeshBuffer mSphereBUffer;
	TextureId mDiffuseMap;
	TextureId mSpongeBob;

	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;
//
	Mesh mQuad;
	MeshBuffer mQuadBuffer;

	TextureId mQuadDiffuse;
	TextureId mQuadNormal;

//
	Mesh mCube;
	MeshBuffer mCubeBuffer;
 
	//TMR test
	ShaderEffect_Standard::SE_Context_Standard ctx1;
	ShaderEffect_Standard::SE_Context_Standard ctx2;

	TMeshRenderer tmr1;

	TRenderObject tobj1;
	TRenderObject tobj2;

 
	//
	

	//test post proc
	TPostProcess mTestPostProc;



	//test shadow
	TestShadow mTestShadow;

	//TDepthMap mTestDepthMap;

	Camera* mCurrentCam;
	bool useLightCam = false;
 

};

