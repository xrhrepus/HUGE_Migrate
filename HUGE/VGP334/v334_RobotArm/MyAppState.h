#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "Arm.h"

using namespace H;
using namespace H::Graphics;
//class AppState;
void UpdateTransforms(Bone* bone, std::vector<Math::Matrix4>& bonematrix);
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

	//

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

 

	//mesh IO
	Mesh mMeshToLoad;
	MeshBuffer mMeshToLoadBuffer;


	Matrix4 mMeshTransform;
	VertexShader mMeshVShader;
	PixelShader mMeshPShader;

	//animation
	
	Model mModel;
	float mTestTimer = 0.0f;
	Animation mTestAnim;


	static const size_t mNumBones = 3;
	std::vector<Mesh> mMeshes;
	std::vector<MeshBuffer> mMeshBufs;

	std::vector<Bone> mBones;
	std::vector<Math::Matrix4> mBoneMatrix;

	int mBone_currChoose;
	std::vector<H::Math::Vector3>mBoneTransCtrls;
	std::vector<H::Math::Vector3>mBoneRotCtrls;
	std::vector<H::Math::Vector3>mBoneRotAngles;

	float mBoneTransCtrl[3];
	float mBoneRotCtrl[3];

	float mArmAnim_Timer = 0.0f;
	float mArmAnim_Time = 10.0f;
	Animation mArmAnim;

	//test arm
	Arm mTestArm;

};

