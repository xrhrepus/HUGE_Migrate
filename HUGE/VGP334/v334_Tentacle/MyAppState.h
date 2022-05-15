#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "Tentacle.h"

using namespace H;
using namespace H::Graphics;
//class AppState;
void UpdateTransforms(Bone* bone, std::vector<Math::Matrix4>& bonematrix );
struct tTransformData
{
	Matrix4 world;
	Matrix4 wvp;// combined matrix
	Vector3 viewPosition;
	float padding;
};
struct tOptionsData
{
	float displacementWeight;
	float useNormal;
	float padding[2];
};
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

struct tBoneTransformData
{
	Matrix4 boneTransform[128];
};

using tTransformBuffer = TypedConstantBuffer<tTransformData>;
using tLightBuffer = TypedConstantBuffer<DirectionalLight>;
using tMaterialBuffer = TypedConstantBuffer<Material>;
using tOptionsBuffer = TypedConstantBuffer<tOptionsData>;
using BloomParamBuffer = TypedConstantBuffer<BloomParamData>;

using ParticleBuffer = TypedConstantBuffer<GeometryData>;
using  tBoneTransformBuffer = TypedConstantBuffer<tBoneTransformData>;



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
	tLightBuffer mDirectionalLightBuffer;
	Material mMaterial;
	tMaterialBuffer mMaterialBuffer;

	tOptionsData mOptionsData;

	//texture
	TextureId mPalad;
	TextureId mEarth;

	//Mesh PN
	std::vector<MeshPN>	 mMeshpns;
	std::vector<MeshBuffer>	 mMeshpnBufs;
	VertexShader mMeshpnVS;
	PixelShader mMeshpnPS;
	tTransformBuffer mTransformBuffer;


 
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


	static const size_t mNumBones = 4;
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

	//tentacle

	BoneMesh mBonemesh;
	BoneMesh mBonemesh2;

	MeshBuffer mBoneMeshBuffer;
	MeshBuffer mBoneMeshBuffer2;

	VertexShader mBoneVShader;
	PixelShader mBonePShader;
	tBoneTransformBuffer mBoneTfBuffer;

	Tentacle mTentacle;
};

