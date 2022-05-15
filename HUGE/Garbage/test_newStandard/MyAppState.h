#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
//#include "TPostProcess.h"
#include "TMeshRenderer.h"
#include "TRenderObject.h"
#include "TestShadow.h"
#include "TDepthMap.h"
#include "TEdgeDraw.h"
#include "TBloom.h"
#include "TPostProcDraw.h"
#include "TScreenRT.h"
#include "TSkyDome.h"
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
	Mesh mQuad;
	MeshBuffer mQuadBuffer;
	Mesh mCube;
	MeshBuffer mCubeBuffer;
	Model mModel;
	const wchar_t* Model_FILE_PATH = L"../../Assets/Meshes/goofyrun.model";
	ShaderEffect_Standard::SE_Context_Standard modelctx;


	TextureId mQuadDiffuse;
	TextureId mQuadNormal;
	TextureId mDiffuseMap;
	TextureId mSpongeBob;

	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;

//
 
	//TMR test
	ShaderEffect_Standard::SE_Context_Standard ctx1;
	ShaderEffect_Standard::SE_Context_Standard ctx2;


	TRenderObject tobj1;
	TRenderObject tobj2;
	//
	C_Transform mModelTf;
  
	//test post proc
	//TPostProcess mTestPostProc;
 	//TDepthMap mTestDepthMap;

	//
	TSkyDome mSkydome;
	//test edge
	TEdgeDraw mEdgeDraw;

	Camera* mCurrentCam;
	bool useLightCam = false;
 
	TScreenRT mScreenRT;
	//bloom
	TBloom mBloomEffect;
	TPostProcDraw mPPDraw;

};

