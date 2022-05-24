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

	mMesh = MeshBuilder::CreateCube(10.0f, 10.0f, 10.0f, Vector3::Zero());
	mMb.Initialize(mMesh);
	vs.Initialize(OLD_STANDARD_FILE_PATH);
	ps.Initialize(OLD_STANDARD_FILE_PATH, "PS");
	tx = TextureManager::Get()->LoadTexture("sponge_bob.png");

	mDl.direction = Vector3{ 0.0f,1.0f,0.0f };
	mDl.diffuse = 1.0f;
	mDl.ambient = 1.0f;
	ltb.Initialize(mDl);
	//
	tfd.world = Matrix4::Identity();
	tfb.Initialize(tfd);
	//
	mt.ambient = Colors::Red;
	mt.diffuse = Colors::Red;
	mt.ambient = Colors::Red;
	mt.ambient = Colors::Red;
	mtb.Initialize(mt);

	//
	tMeshRd = std::make_unique<T_MeshRenderer>(mMesh);
	//
 }

void MyAppState::Terminate()
{
	mMb.Terminate();
	vs.Terminate();
	ps.Terminate();

	tfb.Terminate();
	mtb.Terminate();
	ltb.Terminate();
 }

void MyAppState::RenderScene()
{
//
	Matrix4 vm = mCurrentCam->GetViewMatrix();
	Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	Matrix4 world = Matrix4::Identity();

	tfd.world = H::Math::Transpose(world);
	tfd.viewPosition = mCurrentCam->GetPosition();
	tfd.wvp = H::Math::Transpose(world * vm * pm);
	tfb.Set(tfd);

	tMeshRd->Render();

////
//	mtb.Set(mt);
////
//	ltb.Set(mDl);
////
//	vs.Bind();
//	TextureManager::Get()->GetTexture(tx)->BindPS();
//	ps.Bind();
//
//	tfb.BindVS(0);
//	tfb.BindPS(0);
//
//	mtb.BindVS(2);
//	mtb.BindPS(2);
//
//	ltb.BindVS(1);
//	ltb.BindPS(1);
//
//	mMb.Render();

}

void MyAppState::Render()
{
	_drawGrid();

	RenderScene();
}

void MyAppState::Update(float deltatime)
{
	mCurrentCam->UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
  }

void MyAppState::DebugUI()
{
	tMeshRd->DebugUI();

	ImGui::Begin("he");
	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::SliderFloat3("dir", mDl.direction.v.data(), 0.0f, 1.0f);
	}
	ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tx)), { 100.0f,100.0f });
	ImGui::End();
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


void MyAppState::PostProcess()
{
}
