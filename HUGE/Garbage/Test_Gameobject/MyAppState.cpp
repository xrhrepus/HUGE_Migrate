#include "MyAppState.h"
#include "TComponents.h"
#include "C_Transform.h"
#include "C_Render.h"
float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ 0.0f, 50.0f, -90.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	mLightViewCam.SetPosition({ 0.0f, 50.0f, -90.0f });
	mLightViewCam.SetDirection({ 0.0f,0.0f, 1.0f });

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
 
	//texture
	mDiffuseMap = TextureManager::Get()->LoadTexture("earth.jpg");
	mSpongeBob = TextureManager::Get()->LoadTexture("sponge_bob.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mQuadDiffuse = TextureManager::Get()->LoadTexture("sheep64_normaltest_0.png");
 	mQuadNormal = TextureManager::Get()->LoadTexture("sheep64_normalmap.png");

	//
	mWorldMat1.SetIdentity();
 
 
 
	//
	mSphere = MeshBuilder::CreateShpere(10.0f, 4, {  0.0f });
	mSphereBUffer.Initialize(mSphere);
	//
	mQuad = MeshBuilder::CreateQuad(10.0f, 10.0f, { 0.0f });
	mQuadBuffer.Initialize(mQuad);
	//mQuadDiffuse = TextureManager::Get()->LoadTexture("fruit42_x10.png");

	//
	mCube = MeshBuilder::CreateCube(5.0f, 2.0f, 6.0f, { 0.0f });
	mCubeBuffer.Initialize(mCube);



	//GO test
	mTestGO.AddComponent<C_Transform>();
	mTestGO.AddComponent<C_Render>();
	

	mTestGO.Initialize();
	auto& ctx = mTestGO.GetComponent<C_Render>()->mSEContext;
	ctx.camera = &mCamera;
	ctx.directionalLight = mLight;

	//
	mTestPostProc.Initialize();
	mTestShadowMap.Initialize();

	//
	mTestDepthMap.Initialize();
 }

void MyAppState::Terminate()
{
 	mSphereBUffer.Terminate();
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();
	mTestGO.Terminate();
	mTestShadowMap.Terminate();
	mTestDepthMap.Terminate();

}


void MyAppState::Render()
{
	mTestPostProc.Begin();//begin(std::function renderfuncptr)
	RenderScene();
	mTestPostProc.End();
	mTestPostProc.Render();
}

void MyAppState::Update(float deltatime)
{
	mCamera.UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
	mTestGO.Update(deltatime);
 }

void MyAppState::DebugUI()
{
 
	mTestDepthMap.DebugUI();
	mTestPostProc.DebugUI();

	ImGui::Begin("Scene");
	if (ImGui::Button("add test comp"))
	{
		mTestGO.AddComponent<TestTimerComp>();
	}

	ImGui::End();
	mTestGO.DebugUI();


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
 
	Matrix4 world = H::Math::Translation({ 30.0f,30.0f,0.0f });
	Matrix4 comp = world * vm * pm;

	TransformData tf;
 

	mTestDepthMap.Begin();
	mTestDepthMap.mLightViewCam = &mCamera;
	mTestDepthMap.world = &world;

	mTestDepthMap.DrawDepthMap();

	mSphereBUffer.Render();

	mTestDepthMap.End();



	//bind===


	//standard start
	auto SEMngr = ShaderEffectManager::Get();
	auto standardEffect = SEMngr->GetEffect<ShaderEffect_Standard>("Standard");
	standardEffect->Bind();
	//obj 1
 
	//set values
 	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	standardEffect->SetValue_SamplerType(SamplerType::PointClamp);
	standardEffect->SetValue_LightBuffer(mLight);
	standardEffect->SetValue_MaterialBuffer(mMaterial);
  
 
	world = H::Math::Translation({ 30.0f,30.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	standardEffect->SetValue_TransformBuffer(tf);
	standardEffect->SetValue_DiffuseMap(mDiffuseMap);
	standardEffect->SetValue_OptionsBuffer(mOptionsData);
	standardEffect->SetValue_NormalMap(mNormalMap);
	mOptionsData.useNormal = 1.0f;


	mSphereBUffer.Render();
	standardEffect->UnBind();


	/*world = H::Math::Translation({ 30.0f,30.0f,60.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	mTestShadowMap.mContext.transformData = tf;
	mTestShadowMap.mContext.directionalLight = mLight;

	Vector3 ts = { 30.0f,30.0f,60.0f };
	ts = comp * ts;

	mTestShadowMap.Bind();
	mSphereBUffer.Render();

	world = H::Math::Translation({ 0.0f,60.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	mTestShadowMap.mContext.transformData = tf;
	mTestShadowMap.mContext.directionalLight = mLight;
	mTestShadowMap.Bind();

	mSphereBUffer.Render();*/


	//test go
	/*standardEffect->Bind();
 
	world = mTestGO.GetComponent<C_Transform>()->GetTransformMatrix();
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	auto& ctx = mTestGO.GetComponent<C_Render>()->mSEContext;
 	ctx.transformData = tf;

	mTestGO.Render();
	standardEffect->UnBind();*/
  
	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(mCamera);
 
}

 