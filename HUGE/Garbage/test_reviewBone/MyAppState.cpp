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

	mCurrentCam = &mCamera;

	mTentacle = MeshBuilder::CreateFourBoneTentacle(10.0f, 2.0f, 4, Vector3::Zero());
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
}

void MyAppState::_drawGrid()
{
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

void MyAppState::RenderScene()
{
	_drawGrid();
 
}

void MyAppState::PostProcess()
{
}
