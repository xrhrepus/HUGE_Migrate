#include "MyAppState.h"
#include "HUGE/Inc/TransformComponent.h"
#include "HUGE/Inc/H_MeshRenderer.h"
float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ -121.0f, 75.0f, 100.0f });
	//mCurrentCam.SetDirection({ 0.0f,0.0f, 1.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 121.0f, -75.0f, -100.0f }));

 	//lighting
	mLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mLight.specular = { 0.8f,0.8f,0.8f,1.0f };
	mLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mLight.direction = H::Math::Normalize({ 0.0f,0.0f,1.0f });

	//material
	mMaterial.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.specular = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.power = 1.0f;

	//texture
	/*mDiffuseMap = TextureManager::Get()->LoadTexture("fruit42.png");
	mSpongeBob = TextureManager::Get()->LoadTexture("sponge_bob.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("shader-ref-fur-2.png");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mQuadDiffuse = TextureManager::Get()->LoadTexture("white.png");
	mQuadNormal = TextureManager::Get()->LoadTexture("sheep64_normalmap.png");
	mRamp = TextureManager::Get()->LoadTexture("toonShaderRamp4.png");*/
	//
	mM.Initialize(Model_FILE_PATH);
 	mT = TextureManager::Get()->LoadTexture(Model_Diff_FILE_PATH);

	
	//
	mCurrentCam = &mCamera;
	mRenderObjs.emplace_back(TRenderObject());
	ShaderEffect_Standard::SE_Context_Standard ctx1;
	ctx1.camera = mCurrentCam;
	ctx1.meshBuffer = &mM.meshData[0].meshBuffer;
 	ctx1.directionalLight = mLight;
	ctx1.diffuse = mT;
 	ctx1.material = mMaterial;
	ctx1.settings.brightness = 1.0f;
	ctx1.settings.useShadow = false;

	mStandardCtx = ctx1;
	for (auto& r : mRenderObjs)
	{
		r.Initialize("t");
	}

 }

void MyAppState::Terminate()
{
	for (auto& r : mRenderObjs)
	{
		r.Terminate();
	}

  }


void MyAppState::Render()
{
	//
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();


	//TransformData tfd;
	//Matrix4 vm = mCurrentCam->GetViewMatrix();
	//Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	//Matrix4 world = tobj1.mTransform.GetTransformMatrix();

	////obj1
	//tfd.world = H::Math::Transpose(tobj1.mTransform.GetTransformMatrix());
	//tfd.viewPosition = mCurrentCam->GetPosition();
	//tfd.wvp = H::Math::Transpose(world * vm * pm);
 //
	//mTED.BeginDN();
	//mTED.SetDNBuffer(tfd);
	//tobj1.mMeshRenderer.mToonContext.meshBuffer->Render();
	//mTED.EndDN();

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

	for (auto& r : mRenderObjs)
	{
		r.DebugUI();
	}

	mCurrentCam->DebugUI();
 
 	ImGui::Begin("Setting");
	ImGui::Text("fps: %f", fps);

 
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mLight.direction.x, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mLight.direction.y, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mLight.direction.z, 0.25f, -1.0f, 1.0f);

		if (directionChanged)
		{
			mLight.direction = Math::Normalize(mLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mLight.specular.r);


	}

	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 0.0f, 100.0f);
 		

	}
 
 	ImGui::End();

}

void MyAppState::RenderScene()
{
	for (auto& r : mRenderObjs)
	{
		for (auto & m : mM.meshData)
		{
			mStandardCtx.meshBuffer = &m.meshBuffer;
			r.SetMeshContext(mStandardCtx);
			r.Render();
		}
	}


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

 