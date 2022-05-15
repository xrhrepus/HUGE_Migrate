#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
  
using namespace H;
using namespace H::Graphics;
//class AppState;
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
	//// GaussianBlur
	//Vector2 sampleOffset[15];
	//float sampleWeight[15];
 
};
struct GeometryData
{
	Matrix4 wvp;// combined matrix
	Vector3 camPosition;
	Vector3 camUp;

	float padding[2];
};

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

private:
	const wchar_t* DO_LIGHTING_FILE_PATH = L"../../Assets/Shaders/DoLighting.fx";
	const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";
	const wchar_t* STANDARD_FILE_PATH = L"../../Assets/Shaders/OldStandard.fx";
	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	const wchar_t* POSTPROCESSING_BLOOM_FILE_PATH = L"../../Assets/Shaders/Bloom.fx";
	const wchar_t* POSTPROCESSING_COMBINE_FILE_PATH = L"../../Assets/Shaders/PostProcessingCombine.fx";
	const wchar_t* PARTICLE_FILE_PATH = L"../../Assets/Shaders/Particle.fx";


	Camera mCamera;
	//using TransformBuffer = TypedConstantBuffer<Matrix4>;
	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;
	BloomParamBuffer mBloomBuffer;

	Matrix4 mWorldMat1;
	Matrix4 mWorldMat2;

	Matrix4 mRotation1;
	float rot_x  ;
	float rot_y  ;
	float rot_z  ;

	MeshPX mSkyBox;
	MeshPN mMeshPN;
	Mesh mEarth_Mesh;
	Mesh mCube;

	MeshBuffer mSkyBoxMeshBuffer;
	MeshBuffer mMeshBuffer;
	MeshBuffer mMeshBufferPN;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptionsData;
	BloomParamData mBloomData;

	VertexShader mVertexShader;
	VertexShader mVertexShaderPN;
	VertexShader mVertexShaderPX;

	PixelShader mPixelShader;
	PixelShader mPixelShaderPN;
	PixelShader mPixelShaderPX;


	//Sampler mSampler;
	//
	TextureId mDiffuseMap;
	TextureId mSpongeBob;

	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;

	//
	TextureId mSkyboxTex;
	TextureId mParticleTex;

	//r
	RenderTarget mRenderTarget;
	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;


	//post proc
	RenderTarget mPostProc_None_RenderTargets;

	RenderTarget mPostProc_Bloom_RenderTargets;

	//combine
	VertexShader mPostProc_Combine_VertexShader;
	PixelShader mPostProc_Combine_PixelShader;


	//none
	VertexShader mPostProc_NoneVertexShader;
	PixelShader mPostProc_NonePixelShader;

	//bloom
	VertexShader mPostProc_BloomVertexShader;
	PixelShader mPostProc_BloomPixelShader;
	bool isBloomOn = true;

	//particle
	VertexShader mParticle_VertexShader;
	PixelShader mParticle_PixelShader;


	MeshPNC mParticles;
	MeshBuffer mParticle_MeshBuffer;
	GeometryShader mParticle_GeoShader;
	ParticleBuffer mParticle_ConstantBuffer;
	GeometryData mGeometryData;
	Matrix4 mParticle_worldmat1;
};

