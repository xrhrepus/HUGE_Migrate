#include "MyAppState.h"
#include "HUGE/Inc/TransformComponent.h"

float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
	mCurrentCam = &mCamera;
	//camera
	mCamera.SetPosition({ -121.0f, 75.0f, 100.0f });
	//mCurrentCam.SetDirection({ 0.0f,0.0f, 1.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 121.0f, -75.0f, -100.0f }));

	// These should from some manager
	mDiffuseMap = TextureManager::Get()->LoadTexture("earth.jpg");
	mSpongeBob = TextureManager::Get()->LoadTexture("sheep64_normaltest_0.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mSkyboxTex = TextureManager::Get()->LoadTexture("universe1024.jpg");
	mParticleTex = TextureManager::Get()->LoadTexture("alphaSheep128.png");

	// These should be from some component ?

	//mShader = std::make_unique<TStandardShader>(STANDARD_INSTANCED_FILE_PATH);
	//mMaterial = std::make_unique<TMaterial>(*mShader);


// Hardcoded or Initialized data. These are set before game starts
#pragma region Hardcoded or Initialized data

	// 1 Shader instance = 1 Shader file
	// Resources used by shader and slots. Should be done by reflection
	// The name is the variable name in shader
	//mShader->mConstantBuffers.emplace("TransformBuffer", ShaderResource{ 1,0 });
	//mShader->mConstantBuffers.emplace( "LightBuffer",1,1 );
	//mShader->mConstantBuffers.emplace( "MaterialBuffer",1,2 );
	//mShader->mTextures.emplace( "diffuseMap",1, 0 );
	//mShader->mTextures.emplace( "specularMap",1, 1 );
	//mShader->mTextures.emplace( "displacementMap",1, 2 );
	//mShader->mTextures.emplace( "normalMap",1, 3 );

	// Materials. Share the same Shader. 
	// But need to be individual instance if using different data or texture
	//mMaterial->SetTexture("diffuseMap", mDiffuseMap);
	//mMaterial->SetTexture("specularMap", mSpecularMap);
	//mMaterial->SetTexture("displacementMap", mDisplacementMap);
	//mMaterial->SetTexture("normalMap", mNormalMap);

	// These const buffers should from some memory block/pool
	tb.Initialize();
	lb.Initialize();
	mb.Initialize();

	sbt.Initialize();
	sbm.Initialize();
	sbi.Initialize();

	// Open to set hard-code or if used in editor/Imgui
	// give it a default transform
	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();
	Matrix4 worldMat;
	worldMat.Translation({ 0.0f,0.0f,0.0f });
	Matrix4 world = worldMat;
	Matrix4 comp = world * vm * pm;

	TInstanceData insDatas[10];
	Material mats[10];
	TransformData tfs[10];
	TransformData tf;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	for (size_t i = 0; i < 10; i++)
	{
		tfs[i] = (tf);
	}


	tb.Set(tfs);
	lb.Set(H::Graphics::DirectionalLight{});
	mb.Set(mats);

	sbt.Set(tfs[0]);
	sbm.Set(mats[0]);
	sbi.Set(insDatas[0]);

	// Pass the constbuf reference to material.
	//mMaterial->SetConstantBuffer("TransformBuffer", tb);
	//mMaterial->SetConstantBuffer("LightBuffer", lb);
	//mMaterial->SetConstantBuffer("MaterialBuffer", mb);
	// for runtime data like transform, we can update the respective buffer with Set()
	// todo: Doing this makes 1 draw call per object.  Can do some optimization for batch rendering
	// Batch rendereing needs different shader, may keep this for single render, and have a different Material for batching.

	//mesh
	auto m = MeshBuilder::CreateCube(33, 33,33, { 0.0f,0.0f,0.0f });
	mMeshRenderer.mMesh = m;
	mMeshRenderer.mMeshBuffer.Initialize(mMeshRenderer.mMesh);


#pragma endregion

	mTInstanceDraw.Init();
	// end testing
}

void MyAppState::Terminate()
{
	tb.Terminate();
	lb.Terminate();
	mb.Terminate();
}


void MyAppState::Render()
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();

	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);

	//mTInstanceDraw.Draw(*mCurrentCam);
	mTInstanceDraw.DrawWithRenderPass(*mCurrentCam);
	

	RenderScene();
}

void MyAppState::Update(float deltatime)
{
	mCurrentCam->UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
  }

void MyAppState::DebugUI()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("Cam");
	mCurrentCam->DebugUI();
	ImGui::End();

 	ImGui::Begin("Setting");
	ImGui::Text("fps: %f", fps);
 	ImGui::End();
}

void MyAppState::RenderScene()
{


  	//
	for (int i = 0; i < 10; i++)
	{
		SimpleDraw::AddLine({ -100.0f,0.0f, 10.0f * i }, { 100.0f,0.0f,10.0f * i }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -100.0f,0.0f , -10.0f * i }, { 100.0f,0.0f,-10.0f * i }, Colors::DarkCyan);

		SimpleDraw::AddLine({ 10.0f * i,0.0f, -100.0f }, { 10.0f * i,0.0f, 100.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -10.0f * i,0.0f,-100.0f }, { -10.0f * i,0.0f,100.0f }, Colors::DarkCyan);

	}

	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(*mCurrentCam);
 
}

void MyAppState::PostProcess()
{
}
