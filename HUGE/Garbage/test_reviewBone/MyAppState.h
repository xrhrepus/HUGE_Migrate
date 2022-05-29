#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "T_MeshRenderer.h"
#include "T_Scene.h"
#include "T_Components.h"

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
	void _drawGrid();
	void RenderScene();
	void PostProcess();

private:
	// use scene
	T_Scene mScene;

private:
	//use t mesh rd
	T_MeshRenderer tMeshRd;
	T_Light tLight;
	T_Transform tTransform;
	Standard_Material standardMat;
private:

	DirectionalLight mDl;
	LightBuffer ltb;

	Mesh mMesh;
	MeshBuffer mMb;

	VertexShader vs;
	PixelShader ps;
	TextureId tx;

	TransformData tfd;
	TransformBuffer tfb;

	Material mt;
	MaterialBuffer mtb;


private:
	const wchar_t* DO_LIGHTING_FILE_PATH = L"../../Assets/Shaders/DoLighting.fx";
	const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";
	const wchar_t* STANDARD_FILE_PATH = L"../../Assets/Shaders/Standard.fx";
	const wchar_t* OLD_STANDARD_FILE_PATH = L"../../Assets/Shaders/OldStandard.fx";

	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
	const wchar_t* PARTICLE_FILE_PATH = L"../../Assets/Shaders/Particle.fx";


	Camera mCamera;
	Camera* mCurrentCam;
	bool useLightCam = false;
 


};

