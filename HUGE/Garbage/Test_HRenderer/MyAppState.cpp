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
	mSphere = MeshBuilder::CreateShpere(10.0f, 4, { 0.0f });
	mSphereBUffer.Initialize(mSphere);
	//
	mQuad = MeshBuilder::CreateQuad(500.0f, 500.0f, { 0.0f });
	mQuadBuffer.Initialize(mQuad);
	//mQuadDiffuse = TextureManager::Get()->LoadTexture("fruit42_x10.png");

	//
	mCube = MeshBuilder::CreateCube(5.0f, 2.0f, 6.0f, { 0.0f });
	mCubeBuffer.Initialize(mCube);
	//
	mTestPostProc.Initialize();

	//
	ctx1.camera = mCurrentCam;
	ctx1.directionalLight = mLight;
	ctx1.diffuse = mDiffuseMap;
	ctx1.normal = mNormalMap;
	ctx1.material = mMaterial;

	ctx2.camera = mCurrentCam;
	ctx2.directionalLight = mLight;
	ctx2.diffuse = mQuadDiffuse;
	ctx2.normal = mNormalMap;
	ctx2.material = mMaterial;

	//
	tmr1.Initialize();
	tobj1.SetMeshContext(mCube, ctx1);
	tobj2.SetMeshContext(mQuad, ctx2);

	tobj1.Initialize("obj1");
	tobj2.Initialize("obj2");

	//
	{
		mCurrentCam = &mCamera;
		//mTestDepthMap.Initialize();

		mTestShadow.mQuadId = mQuadDiffuse;
		mTestShadow.Initialize(&mCamera);
 	}
 }

void MyAppState::Terminate()
{
 	mSphereBUffer.Terminate();
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();

	tmr1.Terminate();
	tobj1.Terminate();
	tobj2.Terminate();

	mTestShadow.Terminate();
	//mTestDepthMap.Terminate();
}


void MyAppState::Render()
{
	RenderScene();
	//mTestPostProc.Begin();//begin(std::function renderfuncptr)
	//mTestPostProc.End();
	//mTestPostProc.Render();
}

void MyAppState::Update(float deltatime)
{
	mCurrentCam->UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
  }

void MyAppState::DebugUI()
{
	ImGui::Begin("Cameras");
	bool camchanged = false;
	if (ImGui::Checkbox("useLightCam##Cameras", &useLightCam))
	{
		camchanged = true;
	}
	if (camchanged)
	{
		//mCurrentCam = useLightCam ? &mTestDepthMap.mLightViewCam : &mCamera;
		mCurrentCam = useLightCam ? &mTestShadow.mDepthMap.mLightViewCam : &mCamera;
		mTestShadow.mNSE.mContext.camera = mCurrentCam;

 		//mTestShadow.mRenderObj.mMeshRenderer.mStandardContext.camera = mCurrentCam;
	}

	ImGui::End();

 
	mCurrentCam->DebugUI();

	tobj1.DebugUI();
	tobj2.DebugUI();
 	mTestPostProc.DebugUI();
	mTestShadow.DebugUI();
	tmr1.DebugUI();
	//tmr2.DebugUI();


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

	mTestShadow.Render();

	//mTestDepthMap.DrawDepthMap();

#pragma region testother


////method 1
//	{
		//Matrix4 vm = mCurrentCam->GetViewMatrix();
		//Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();

		//Matrix4 world = H::Math::Translation({ 30.0f,30.0f,0.0f });
		//Matrix4 comp = world * vm * pm;

		//TransformData tf;

		//auto SEMngr = ShaderEffectManager::Get();
		//auto stdEffect = SEMngr->GetEffect<ShaderEffect_Standard>("Standard");
		//stdEffect->Bind();
//
//		world = H::Math::Translation({ 30.0f,30.0f,0.0f });
//		comp = world * vm * pm;
//		tf.world = H::Math::Transpose(world);
//		tf.wvp = H::Math::Transpose(comp);
//		tf.viewPosition = mCurrentCam.GetPosition();
//
//
//		//1
//		ctx1.camera = &mCurrentCam;
//		ctx1.directionalLight = mLight;
//		ctx1.diffuseMap = mDiffuseMap;
//		ctx1.normalMap = mNormalMap;
//		ctx1.transformData = tf;
//		ctx1.material = mMaterial;
//		stdEffect->SetContextInfo(&ctx1);
//
//		mSphereBUffer.Render();
//
		//2
		//world =  H::Math::RotationX(H::Math::Constants::Pi * 0.5f) * H::Math::Translation(Vector3{ 0.0f,-100.0f,0.0f });
		//comp = world * vm * pm;
		//tf.world = H::Math::Transpose(world);
		//tf.wvp = H::Math::Transpose(comp);
		//tf.viewPosition = mCurrentCam->GetPosition();

 	//	ctx2.camera = mCurrentCam;
		//ctx2.directionalLight = mLight;
		//ctx2.diffuseMap = mQuadDiffuse;
		//ctx2.normalMap = mNormalMap;
		//ctx2.transformData = tf;
		//ctx2.material = mMaterial;

		//stdEffect->SetContextInfo(&ctx2);
		//mQuadBuffer.Render();
		//stdEffect->UnBind();
//
//	}
//
//method 2
	{
		//tobj1.Render();
		//tobj2.Render();
	}
#pragma endregion
	/*mTestDepthMap.Begin();

	mTestDepthMap.DrawDepthMap(mTestShadow.mRenderObj.mTransform.GetTransformMatrix());
	mTestShadow.mRenderObj.mMeshRenderer.mMeshBuffer.Render();
	mTestDepthMap.End();*/

	//
	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(*mCurrentCam);
 
}

 