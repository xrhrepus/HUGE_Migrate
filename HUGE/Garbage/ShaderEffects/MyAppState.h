#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
 #include "TMeshRenderer.h"
#include "TRenderObject.h"
 
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
 
	Camera* mCurrentCam;
	Camera mCamera;
 	//
	DirectionalLight mLight;
	Material mMaterial;
  
 	const wchar_t* Model_FILE_PATH = L"../../Assets/Models/Penguin/penguin.model";
	const wchar_t* Model_Diff_FILE_PATH = L"../../Assets/Models/Penguin/penguin.tif";

	std::vector<TextureId> mTextures;
	Model mM;
 	TextureId mT;
	std::vector<TRenderObject> mRenderObjs;
	ShaderEffect_Standard::SE_Context_Standard mStandardCtx;

 

};

