#include "MyAppState.h"
#include "TComponents.h"
#include "C_Transform.h"
#include "C_Render.h"
#include "C_MeshRenderer.h"

float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
 	//camera
	mCamera.SetPosition({ 0.0f, 250.0f, -90.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//==
	mLightCamera.SetPosition(Vector3{ 0.0f} - mLightCamera.GetDirection() * 100.0f);
	mLightCamera.SetDirection({ 0.0f,0.0f, 1.0f });
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFOV(1.0f);
	mCurrentCamera = &mLightCamera;
	//==


	//buffer
 	mBloomBuffer.Initialize();
  
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
  	auto txMngr = TextureManager::Get();
 
	//GO test
	mTestGO.AddComponent<C_Transform>();
	mTestGO.AddComponent<C_Render>();
	

	mTestGO.Initialize();
	
	//
   
	//
	mtestShadowPlane.AddComponent<C_Transform>();
	auto mr = mtestShadowPlane.AddComponent<C_MeshRenderer>();
	mr->mesh = MeshBuilder::CreateQuad(350.0f, 350.0f, { 0.0f });
	mr->diffuse = txMngr->LoadTexture("../../Assets/Images/sheep64.png");
	mr->normal = txMngr->LoadTexture("../../Assets/Images/sheep64_normalmap.png");
	Material m;
	m.ambient = { 0.8f,0.8f,0.8f,1.0f };
	m.specular = { 0.8f,0.8f,0.8f,1.0f };
	m.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	m.power = 1.0f;

	mr->material = m;
	mtestShadowPlane.Initialize();

	//
	Mesh tk; 
	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.01f, tk);

	mtestShadowTank.AddComponent<C_Transform>();
	mr = mtestShadowTank.AddComponent<C_MeshRenderer>();
	mr->mesh = tk;
	mr->diffuse = txMngr->LoadTexture("../../Assets/Models/Tank/tank_diffuse.jpg");
	mr->normal = txMngr->LoadTexture("../../Assets/Models/Tank/tank_normal.jpg");
 

	mr->material = m;
	mtestShadowTank.Initialize();

	BlendStateManager::Get()->GetBlendState("AlphaBlend")->Set();;
 }

void MyAppState::Terminate()
{
 	mTestGO.Terminate();
	mtestShadowPlane.Terminate();
	mtestShadowTank.Terminate();
 }


void MyAppState::Render()
{
 	RenderScene();
 	
}

void MyAppState::Update(float deltatime)
{
	mCamera.UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
	mTestGO.Update(deltatime);
	mtestShadowPlane.Update(deltatime);
 }

void MyAppState::DebugUI()
{
	ImGui::Begin("Scene");
	if (ImGui::Button("add test comp"))
	{
		mTestGO.AddComponent<TestTimerComp>();
	}

	ImGui::End();
	mTestGO.DebugUI();
	mtestShadowPlane.DebugUI();

 	ImGui::Begin("Setting");
	ImGui::Text("fps: %f", fps);

	//ImGui::Text("%f,%f,%f", mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z);

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
  
	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();
 
	Matrix4 world = Matrix4::Identity();
	Matrix4 comp = world * vm * pm;

	TransformData tf;
 
	//bind===
	//standard start
	auto SEMngr = ShaderEffectManager::Get();
	auto standardEffect = SEMngr->GetEffect<ShaderEffect_Standard>("Standard");
	standardEffect->Bind();
	standardEffect->SetValue_LightBuffer(mLight);


	world = Matrix4::Identity() * Matrix4::translation({0.0f,50.0f,0.0f});
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	//tank
	mtestShadowTank.GetComponent<C_MeshRenderer>()->transformData = tf;
	mtestShadowTank.Render();

	standardEffect->UnBind();
 
	//test go
	standardEffect->Bind();
 
	//
	world = Matrix4::Identity() * Matrix4::rotationX(90.0f * Math::Constants::DegToRad);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	mtestShadowPlane.GetComponent<C_MeshRenderer>()->transformData = tf;
	mtestShadowPlane.Render();
	standardEffect->UnBind();

	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(mCamera);
 
}

 