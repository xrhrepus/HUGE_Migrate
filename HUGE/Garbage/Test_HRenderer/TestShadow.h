#pragma once
#include "TRenderObject.h"
#include "TDepthMap.h"
#include "NewStandardShader.h"

class TestShadow
{
public:
 	void Initialize(Camera* cam);
	void Terminate();
	void Update(float deltatime);
	void Render();
	void DebugUI();

 
	TRenderObject mRenderObj;
	ShaderEffect_Standard::SE_Context_Standard mContex;

	TDepthMap mDepthMap;
	NewStandardShader mNSE;
	DirectionalLight mLight;
	Material mMaterial;

	//contexts
	NewStandardShader::NS_Context tankContext;
	NewStandardShader::NS_Context quadContext;

	//bot quad
	Mesh mQuad;
	MeshBuffer mQuadBuf;
	TextureId mQuadId;
 
	//
	Camera* mainCam;
	Camera* lightCam;

	bool useLightCam = false;
private:
	Mesh mMesh;
 };
 