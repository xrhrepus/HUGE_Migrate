#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "LineSkeleton.h"

using namespace H;
using namespace H::Graphics;
//class AppState;
void UpdateTransforms(Bone* bone, std::vector<Math::Matrix4>& bonematrix );
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
  
};
struct GeometryData
{
	Matrix4 wvp;// combined matrix
	Vector4 camPosition;
	Vector3 camUp;

	float padding;
};

 

 using LightBuffer = TypedConstantBuffer<DirectionalLight>;
using MaterialBuffer = TypedConstantBuffer<Material>;
 using BloomParamBuffer = TypedConstantBuffer<BloomParamData>;

using ParticleBuffer = TypedConstantBuffer<GeometryData>;
 


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
	void DrawGrid() const;
private:
	//constant
	const wchar_t* DO_LIGHTING_FILE_PATH = L"../../Assets/Shaders/DoLighting.fx";
	const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";
	const wchar_t* STANDARD_FILE_PATH = L"../../Assets/Shaders/Standard.fx";
	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
	const wchar_t* PARTICLE_FILE_PATH = L"../../Assets/Shaders/Particle.fx";
	const wchar_t* SKINNING_FILE_PATH = L"../../Assets/Shaders/Skinning.fx";

	//
	H::Math::Plane mPlaneYM{ 0.0f,1.0f,0.0f , -60.0f };//y-
	H::Math::Plane mPlaneYP{ 0.0f,-1.0f,0.0f , 60.0f };//y+
	H::Math::Plane mPlaneXM{ 1.0f,0.0f,0.0f , -60.0f };//x-
	H::Math::Plane mPlaneXP{ -1.0f,0.0f,0.0f , 60.0f };//x+
	H::Math::Plane mPlaneZM{ 0.0f,0.0f,1.0f , -60.0f };//z-
	H::Math::Plane mPlaneZP{ 0.0f,0.0f,-1.0f , 60.0f };//z+

	//camera
	Camera mCamera;

	//Render targets
	RenderTarget mRenderTarget;
	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

	//Matrixs
	Matrix4 mWorldMat;

	//Lighting
	DirectionalLight mDirectionalLight;
	LightBuffer mDirectionalLightBuffer;
	Material mMaterial;
	MaterialBuffer mMaterialBuffer;

	OptionsData mOptionsData;

	//texture
	TextureId mPalad;
	TextureId mEarth;

	//Mesh PN
	std::vector<MeshPN>	 mMeshpns;
	std::vector<MeshBuffer>	 mMeshpnBufs;
	VertexShader mMeshpnVS;
	PixelShader mMeshpnPS;
	TransformBuffer mTransformBuffer;
 
	//post proc
	VertexShader mPPnothingVS;
	PixelShader mPPnothingPS;
	RenderTarget mPPRendertarget;

	//physics
	Physics::PhysicsWorld mPhysicsWorld;
 
	//cloth test
	std::vector<H::Physics::Particle*> ps;
 
 };

