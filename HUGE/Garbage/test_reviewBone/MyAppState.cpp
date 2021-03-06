#include "MyAppState.h"
#include "HUGE/Inc/TransformComponent.h"
#include "HUGE/Inc/MeshRenderer.h"
float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
	mScene.Init();
	//camera
	mCamera.SetPosition({ -121.0f, 75.0f, 100.0f });
	//mCurrentCam.SetDirection({ 0.0f,0.0f, 1.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 121.0f, -75.0f, -100.0f }));

	mCurrentCam = &mCamera;

	//mMesh = MeshBuilder::CreateCube(30.0f, 30.0f, 30.0f, Vector3::Zero());
	//mMesh = MeshBuilder::CreateShpere(6.0f, 6, Vector3::Zero());
	//ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.05f, mMesh);
	//ObjLoader::Load("../../Assets/Models/TestModel/cat.obj", 100.0f, mMesh);
	//ObjLoader::Load("../../Assets/Models/TestModel/fox.obj", 100.0f, mMesh);
	//ObjLoader::Load("../../Assets/Models/TestModel/simpleCube.obj", 100.0f, mMesh);
	mMesh = *mScene.GetMesh("tank");

	mMb.Initialize(mMesh);
	vs.Initialize(OLD_STANDARD_FILE_PATH);
	ps.Initialize(OLD_STANDARD_FILE_PATH, "PS");
	//tx = TextureManager::Get()->LoadTexture("sponge_bob.png");
	//tx = TextureManager::Get()->LoadTexture("../../Assets/Models/TestModel/tree_tex.png");

	//tx = TextureManager::Get()->LoadTexture("../../Assets/Models/TestModel/cat_tex.png");
	//tx = TextureManager::Get()->LoadTexture("../../Assets/Models/Tank/tank_diffuse.jpg");
	//tx = TextureManager::Get()->LoadTexture("../../Assets/Models/TestModel/fox_red.png");
	//tx = TextureManager::Get()->LoadTexture("../../Assets/Models/TestModel/simplecube_tex.png");
	tx = mScene.GetTexture("tankdiffuse");
	//tx = mScene.GetTexture("cat");

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
	tMeshRd.SetCamera(*mCurrentCam);
	tMeshRd.SetMesh(mMesh);
	tMeshRd.SetLight(tLight);
	tMeshRd.SetMaterial(standardMat);
	tMeshRd.SetTransform(tTransform);
	tMeshRd.SetScene(mScene);
	//
	standardMat.Init();
	standardMat.SetDiffuseTexture(tx);
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
	Matrix4 world = Matrix4::translation(Vector3{ 5.0f,3.0f,1.0f });

	tfd.world = H::Math::Transpose(world);
	tfd.viewPosition = mCurrentCam->GetPosition();
	tfd.wvp = H::Math::Transpose(world * vm * pm);
	tfb.Set(tfd);

	tMeshRd.PreRender();
	tMeshRd.Render();
	tMeshRd.PostRender();
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
	ImGui::ShowDemoWindow();
	mScene.DebugUI();
	tMeshRd.DebugUI();
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
