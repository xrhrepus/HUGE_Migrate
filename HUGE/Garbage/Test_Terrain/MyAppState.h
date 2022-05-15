#pragma once
#include "TRaycast.h"
#include "TPlane.h"
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>

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
	//terrrain

	//terrrain


	Camera mCamera;
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

	Mesh mTerr;
	MeshBuffer mTerrBuffer;
	ShaderEffect_Standard::SE_Context_Standard mTerrCtx;




	TextureId mQuadDiffuse;
	TextureId mQuadNormal;
	TextureId mDiffuseMap;
	TextureId mSpongeBob;

	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;

//
	Terrain mTestTerr;
	TRaycast mTestRaycast;
	Math::Matrix4 matInvScreen;
	Vector3 coordV;

	//plane
	TPlane mTPlane;


	Camera* mCurrentCam;
	bool useLightCam = false;
 

};

