#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>

using namespace H;
using namespace H::Graphics;
//class AppState;
struct TransformData
{
	Matrix4 world;
	Matrix4 wvp;// combined matrix
	Vector3 viewPosition;
	float padding;
};
struct OptionsData
{
	float displacementWeight;
	float useNormal;
	float padding[2];
};



using TransformBuffer = TypedConstantBuffer<TransformData>;
using LightBuffer = TypedConstantBuffer<DirectionalLight>;
using MaterialBuffer = TypedConstantBuffer<Material>;
using OptionsBuffer = TypedConstantBuffer<OptionsData>;

class MyAppState : public H::AppState
{
public:
	 void Initialize();
	 void Terminate();
	 void Render();
	 void Update(float deltatime);
	 void DebugUI();

private:
	const wchar_t* DO_LIGHTING_FILE_PATH = L"../Assets/Shaders/DoLighting.fx";
	const wchar_t* TEXTURING_FILE_PATH = L"../Assets/Shaders/Texturing.fx";

	const wchar_t* STANDARD_FILE_PATH = L"../Assets/Shaders/Standard.fx";


	Camera mCamera;
	//using TransformBuffer = TypedConstantBuffer<Matrix4>;
	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	OptionsBuffer mOptionsBuffer;

	Matrix4 mWorldMat1;
	Matrix4 mWorldMat2;

	Matrix4 mRotation1;
	float rot_x  ;
	float rot_y  ;
	float rot_z  ;

	MeshPX mSkyBox;
	MeshPN mMeshPN;
	Mesh mEarth_Mesh;
 
	MeshBuffer mSkyBoxMeshBuffer;
	MeshBuffer mMeshBuffer;
	MeshBuffer mMeshBufferPN;

	DirectionalLight mLight;
	Material mMaterial;
	OptionsData mOptionsData;


	VertexShader mVertexShader;
	VertexShader mVertexShaderPN;
	VertexShader mVertexShaderPX;

	PixelShader mPixelShader;
	PixelShader mPixelShaderPN;
	PixelShader mPixelShaderPX;


	//Sampler mSampler;
	//
	TextureId mDiffuseMap;
	TextureId mSpecularMap;
	TextureId mDisplacementMap;
	TextureId mNormalMap;

	//
	TextureId mSkyboxTex;




};

