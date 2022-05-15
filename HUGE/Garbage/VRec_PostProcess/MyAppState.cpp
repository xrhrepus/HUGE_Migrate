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
	mDiffuseMap = TextureManager::Get()->LoadTexture("fruit42.png");
	mSpongeBob = TextureManager::Get()->LoadTexture("water_32.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mQuadDiffuse = TextureManager::Get()->LoadTexture("yellow_32.png");
	mQuadNormal = TextureManager::Get()->LoadTexture("lawngreen_32.png");
	
	//
	mSphere = MeshBuilder::CreateShpere(10.0f, 4, { 0.0f });
	mSphereBUffer.Initialize(mSphere);
	//
	mQuad = MeshBuilder::CreateQuad(100.0f, 100.0f, { 0.0f });
	mQuadBuffer.Initialize(mQuad);
	//mQuadDiffuse = TextureManager::Get()->LoadTexture("fruit42_x10.png");

	//
	mCube = MeshBuilder::CreateCube(20.0f, 20.0f, 20.0f, { 0.0f });
	mCubeBuffer.Initialize(mCube);

	mModel.Initialize(Model_FILE_PATH);
	//
	//mTestPostProc.Initialize();

	//
	mCurrentCam = &mCamera;

	ctx1.camera = mCurrentCam;
	ctx1.directionalLight = mLight;
	ctx1.diffuse = mDiffuseMap;
	ctx1.normal = mNormalMap;
	ctx1.material = mMaterial;
	ctx1.settings.brightness = 1.0f;
	ctx1.settings.useShadow = false;
 

	ctx2.camera = mCurrentCam;
	ctx2.directionalLight = mLight;
	ctx2.diffuse = mSpongeBob;
	ctx2.normal = mNormalMap;
	ctx2.material = mMaterial;
	ctx2.settings.brightness = 1.0f;
	ctx2.settings.useShadow = false;

	ctx3 = ctx2;
	ctx3.diffuse = mQuadDiffuse;
	ctx4 = ctx2;
	ctx4.diffuse = mQuadNormal;



	modelctx = ctx2;
	modelctx.diffuse = mModel.materialData[0].diffuseMapId;
	modelctx.normal = mModel.materialData[0].normalMapId;
	modelctx.specular = mModel.materialData[0].specularMapId;
	modelctx.settings.specularMapWeight = 1.0f;
	modelctx.settings.normalMapWeight = 1.0f;
	modelctx.settings.brightness = 1.5f;

	//
 	tobj1.SetMeshContext(mCube, ctx1);
	tobj2.SetMeshContext(mSphere, ctx2);
	tobj3.SetMeshContext(mCube, ctx3);
	tobj4.SetMeshContext(mSphere, ctx4);

	tobj1.Initialize("obj1");
	tobj2.Initialize("obj2");
	tobj3.Initialize("obj3");
	tobj4.Initialize("obj4");

	//
	mEdgeDraw.Initialize();
 
	//
	mSkydome.Initialize();
	//
	mBloomEffect.Initialize();
	mPPDraw.Initialize();
	mScreenRT.Initialize();

	mScreenRT.mRT.SetClearColor( 1.0f,0.0f,0.0f,1.0f );
	//GraphicSystem::Get()->SetClearColor({ 1.0f, 0.0f, 0.0f, 1.0f });
 }

void MyAppState::Terminate()
{
 	mSphereBUffer.Terminate();
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();

 	tobj1.Terminate();
	tobj2.Terminate();
	tobj3.Terminate();
	tobj4.Terminate();

	mEdgeDraw.Terminate();

	mBloomEffect.Terminate();
	mPPDraw.Terminate();
	mScreenRT.Terminate();

	mSkydome.Terminate();

 }


void MyAppState::Render()
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();

	//
	TransformData tfd;
	Matrix4 vm = mCurrentCam->GetViewMatrix();
	Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	Matrix4 world = tobj1.mTransform.GetTransformMatrix();

	//
	tfd.world = H::Math::Transpose(world);
	tfd.viewPosition = mCurrentCam->GetPosition();
	tfd.wvp = H::Math::Transpose(world * vm * pm);

	//======== DN START
	//{
	//	mEdgeDraw.BeginDN();

	//	//obj1
	//	tfd.world = H::Math::Transpose(world);
	//	tfd.viewPosition = mCurrentCam->GetPosition();
	//	tfd.wvp = H::Math::Transpose(world * vm * pm);
	//	mEdgeDraw.SetDNBuffer(tfd);
	//	tobj1.mMeshRenderer.mMeshBuffer.Render();

	//	//obj2
	//	world = tobj2.mTransform.GetTransformMatrix();
	//	tfd.world = H::Math::Transpose(world);
	//	tfd.viewPosition = mCurrentCam->GetPosition();
	//	tfd.wvp = H::Math::Transpose(world * vm * pm);

	//	mEdgeDraw.SetDNBuffer(tfd);
	//	tobj2.mMeshRenderer.mMeshBuffer.Render();

	//	//
	//	world = mModelTf.GetTransformMatrix();
	//	tfd.world = H::Math::Transpose(world);
	//	tfd.viewPosition = mCurrentCam->GetPosition();
	//	tfd.wvp = H::Math::Transpose(world * vm * pm);

	//	mEdgeDraw.SetDNBuffer(tfd);

	//	//model
	//	for (auto& m : mModel.meshData)
	//	{
	//		m.meshBuffer.Render();
	//	}


	//	mEdgeDraw.EndDN();

	//}
	//======== DN END

	//==== ORIGINAL SCENE START
	mScreenRT.Begin();
	auto SE_STD = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
 	/*modelctx.transformData = tfd;
	modelctx.directionalLight = mLight;
	modelctx.material = mMaterial;
 
	SE_STD->SetContextInfo(&modelctx);
	SE_STD->Bind();
 	for (auto& m : mModel.meshData)
	{
		m.meshBuffer.Render();
	}*/
 
	SE_STD->SetContextInfo(&ctx1);
	SE_STD->Bind();

	BlendStateManager::Get()->GetBlendState(Blend::Opaque)->Set();
	//
	tobj1.mMeshRenderer.mStandardContext.directionalLight = mLight;
	tobj2.mMeshRenderer.mStandardContext.directionalLight = mLight;
	tobj3.mMeshRenderer.mStandardContext.directionalLight = mLight;
	tobj4.mMeshRenderer.mStandardContext.directionalLight = mLight;

	tobj1.Render();

	SE_STD->SetContextInfo(&ctx2);
	tobj2.Render();
	SE_STD->SetContextInfo(&ctx3);
	tobj3.Render();
	SE_STD->SetContextInfo(&ctx4);
	tobj4.Render();

	SE_STD->UnBind();

	mSkydome.Render(*mCurrentCam);
	mScreenRT.End();
	//==== ORIGINAL SCENE END
	// ==bloom
	mBloomEffect.BloomOriginBegin();

	{
		mPPDraw.Begin();
		mScreenRT.mRT.BindPS(0);
		//mEdgeDraw.mRT.BindPS(0);
		mPPDraw.Render();
		mPPDraw.End();
	}
	mBloomEffect.BloomOriginEnd();
	mBloomEffect.Render();
	// ==bloom

 
	//combine
	{

		mPPDraw.Begin();
		mBloomEffect.mRT_Combine.BindPS(0);
 		mPPDraw.Render();
		mPPDraw.End();

	}
	//combine
	RenderScene();
}

void MyAppState::Update(float deltatime)
{
	mCurrentCam->UpdateControl(deltatime);
	fps = static_cast<float>(1.0f / deltatime);
  }

void MyAppState::DebugUI()
{
	//ImGui::ShowDemoWindow();

	mEdgeDraw.DebugUI();
	ImGui::Begin("Cam");
	mCurrentCam->DebugUI();
	ImGui::End();

	tobj1.DebugUI();
	tobj2.DebugUI();
	tobj3.DebugUI();
	tobj4.DebugUI();

	ImGui::Begin("modelTF");
	mModelTf.DebugUI();
	ImGui::End();

 	//mTestPostProc.DebugUI();
 

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

	ImGui::Begin("Bloom");
	mBloomEffect.DebugUI();
	ImGui::End();


}

void MyAppState::RenderScene()
{


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

void MyAppState::PostProcess()
{
	mBloomEffect.Render();
}
