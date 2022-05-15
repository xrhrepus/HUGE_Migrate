#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "TPostProcess.h"
#include "TMeshRenderer.h"
#include "TRenderObject.h"
#include "TestShadow.h"
#include "TDepthMap.h"
#include "TEdgeDraw.h"

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
	//const wchar_t* Model_FILE_PATH = L"../../Assets/Meshes/goofyrun.model";
	const wchar_t* Model_FILE_PATH = L"../../Assets/Models/BH3model/debugtestd2.model";


	TextureId mQuadDiffuse;
	TextureId mQuadNormal;
	TextureId mDiffuseMap;
	TextureId mSpongeBob;

	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;

//test toon
	TextureId mRamp;
	ShaderEffect_Toon::SE_Context_Toon toonctx;

//
 
	//TMR test
	ShaderEffect_Standard::SE_Context_Standard ctx1;
   
	TRenderObject tobj1;
  
	//test post proc
	TPostProcess mTestPostProc;
 	//TDepthMap mTestDepthMap;

	//test edge
	TEdgeDraw mTED;
	Camera* mCurrentCam;
	bool useLightCam = false;
 

};

