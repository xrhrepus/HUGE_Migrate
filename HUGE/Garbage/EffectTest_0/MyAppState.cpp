#include "MyAppState.h"

float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ 0.0f, 0.0f, 30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, -1.0f });

	//buffer
 	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();
	mBloomBuffer.Initialize();
  
 	//lighting
	mLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mLight.specular = { 0.8f,0.8f,0.8f,1.0f };
	mLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mLight.direction = H::Math::Normalize({ 0.0f,0.0f,-1.0f });

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
 
	//test effect
	mTestEffectStandard.Init(STANDARD_FILE_PATH);

 	//mTestEffectStandard.SetValue_SamplerType("LinearWarp");

	mTestEffectStandard.SetPointer_TransformBuffer(&mTransformBuffer);
	mTestEffectStandard.SetPointer_LightBuffer(&mLightBuffer);
	mTestEffectStandard.SetPointer_MaterialBuffer(&mMaterialBuffer);
	mTestEffectStandard.SetPointer_OptionsBuffer(&mOptionsBuffer);


	//
	mTestDoTexture.Init(TEXTURING_FILE_PATH);
  
	mTestDoTexture.SetPointer_TransformBuffer(&mTransformBuffer);
 
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
}

void MyAppState::Terminate()
{
 
	//mSampler.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
 	mMaterialBuffer.Terminate();
	mOptionsBuffer.Terminate();

  
	//test effect
	mTestEffectStandard.Terminate();
	mTestDoTexture.Terminate();

	//
	mSphereBUffer.Terminate();
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();
}


void MyAppState::Render()
{
	RenderScene();
}

void MyAppState::Update(float deltatime)
{
	mCamera.UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);

}

void MyAppState::DebugUI()
{
	//ImGui::ShowDemoWindow();
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
 
	Matrix4 world = (mWorldMat1);
	Matrix4 comp = world * vm * pm;

	TransformData tf;
 
	//bind===
	//standard start
	mTestEffectStandard.Bind();
	//obj 1

	//compute trans matrix
	world = H::Math::Translation({ 0.0f,0.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	//set values
 	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	mTestEffectStandard.SetValue_SamplerType("LinearWarp");
	mTestEffectStandard.SetValue_LightBuffer(mLight);
	mTestEffectStandard.SetValue_MaterialBuffer(mMaterial);
	mTestEffectStandard.SetValue_TransformBuffer(tf);
	mTestEffectStandard.SetValue_OptionsBuffer(mOptionsData);
	mTestEffectStandard.SetValue_NormalMap(mQuadNormal);
	mTestEffectStandard.SetValue_DiffuseMap(mQuadDiffuse);

	//draw
	mCubeBuffer.Render();

	//obj2
	world = H::Math::Translation({ -30.0f,0.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);


	mTestEffectStandard.SetValue_TransformBuffer(tf);
	mQuadBuffer.Render();

	//might need unbind
	mTestEffectStandard.UnBind();

	//texturing start
	mTestDoTexture.Bind();
 	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
 
	//compute trans matrix

	world = H::Math::Translation({ 30.0f,0.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	//set value
	mTestDoTexture.SetValue_SamplerType("LinearWarp");
	mTestDoTexture.SetValue_TransformBuffer(tf);
	mTestDoTexture.SetValue_DiffuseMap(mSpongeBob);
 
	//draw
	mSphereBUffer.Render();

	//unbind
	mTestDoTexture.UnBind();


	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	//standard start again
	mTestEffectStandard.Bind();
	world = H::Math::Translation({ 30.0f,30.0f,0.0f });
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	mTestDoTexture.SetValue_TransformBuffer(tf);
	mTestEffectStandard.SetValue_DiffuseMap(mDiffuseMap);
	mTestEffectStandard.SetValue_OptionsBuffer(mOptionsData);
	mTestEffectStandard.SetValue_NormalMap(mNormalMap);
	mOptionsData.useNormal = 1.0f;


	mSphereBUffer.Render();
	mTestEffectStandard.UnBind();

 
	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(mCamera);
 
}

 