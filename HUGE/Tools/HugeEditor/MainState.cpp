#include "MainState.h"

void MainState::Initialize()
{
	auto gSys = GraphicSystem::Get();

	mEditor = std::make_unique<Editor>(mWorld);
 
	//terrain
	auto terrSvcs = mWorld.GetService<TerrainService>();

	mWorld.LoadLevel(L"../../Assets/Levels/test_level.json");

	auto camSvcs = mWorld.GetService<H::CameraService>();
	mCamera = camSvcs->AddCamera("EditorCam2");
	mCamera = camSvcs->AddCamera("EditorCam");
	camSvcs->SetActiveCamera("EditorCam");
	mCamera->SetPosition({ 0.0f,30.0f,50.0f });
	mCamera->SetDirection(H::Math::Normalize({ 0.0f,-30.0f,-50.0f }));

	
	mWorld.AddService<H::SkydomeService>();
	mWorld.AddService<H::MeshService>();
	mWorld.AddService<H::RenderMaterialService>();
	mWorld.AddService<H::LightService>();
	mWorld.Initialize(66666);

	mWorld.Create(L"../../Assets/Templates/test.json", "main123qwer");

	mSceneRenderTarget.Initialize(gSys->GetBackBufferHeight(), gSys->GetBackBufferWidth(), RenderTarget::Format::RGBA_U8);

}

void MainState::Terminate()
{
	mWorld.Terminate();
	mSceneRenderTarget.Terminate();

}

void MainState::Render()
{
	mSceneRenderTarget.BeginRender();
	RenderScene();
	mSceneRenderTarget.EndRender();

}

void MainState::Update(float deltatime)
{
	mCamera->UpdateControl(deltatime);
	mWorld.Update(deltatime);

}

void MainState::DebugUI()
{
	mEditor->ShowMenuBar();
	mEditor->ShowWindowWithDockSpace();

	mEditor->ShowSceneView(mSceneRenderTarget);
	mEditor->ShowInspectorView();
	mEditor->ShowWorldView();
	
}
void MainState::RenderScene()
{
	mWorld.Render();


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

	SimpleDraw::Render(*mCamera);

}
