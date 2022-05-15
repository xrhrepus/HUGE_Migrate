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
	const uint32_t w = GraphicSystem::Get()->GetBackBufferWidth();
	const uint32_t h = GraphicSystem::Get()->GetBackBufferHeight();

 

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

 	//
 	mCurrentCam = &mCamera;
	mTestDrawSkele.Init();
 }

void MyAppState::Terminate()
{
}


void MyAppState::Render()
{
	RenderScene();
}

void MyAppState::Update(float deltatime)
{
	mCurrentCam->UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
  }

void MyAppState::DebugUI()
{
	mCurrentCam->DebugUI();
	mTestDrawSkele.DeBugUI();

 
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
	mTestDrawSkele.Render();
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

