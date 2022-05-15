#include "MyAppState.h"
#include "Graphics\Src\MeshIO.h"
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;

namespace
{
	float fps;
	void CamDebugUI(Camera& cam)
	{
		ImGui::Begin("cam");

		if (ImGui::Button(" X##cam")) { cam.SetPosition({ 20.0f,0.0f,0.0f }); cam.SetDirection({ -1.0f,0.0f,0.0f }); } ImGui::SameLine();
		if (ImGui::Button(" Y##cam")) { cam.SetPosition({ 0.0f,20.0f,0.0f }); cam.SetDirection({ 0.0f,-0.98f,-0.02f }); } ImGui::SameLine();
		if (ImGui::Button(" Z##cam")) { cam.SetPosition({ 0.0f,0.0f,20.0f }); cam.SetDirection({ 0.0f,0.0f,-1.0f }); }  
		if (ImGui::Button(" 45.view##cam")) { cam.SetPosition({ 20.0f,20.0f,20.0f }); cam.SetDirection(H::Math::Normalize(Vector3{ 0.0f,0.0f, 0.0f }-Vector3{ 20.0f, 20.0f, 20.0f })); }


		ImGui::End();
	}
	void UpdateTransforms(Bone* bone, std::vector<Math::Matrix4>& bonematrix)
{
	if (bone->parent)
	{
		//bone->offsetTransform = bone->offsetTransform *  bone->toParentTransform;
 		bonematrix[bone->index] = /*bone->offsetTransform **/  bone->toParentTransform * bonematrix[bone->parentIndex];
	}
	else
	{
		//bone->offsetTransform = bone->offsetTransform *  bone->toParentTransform;
 		bonematrix[bone->index] = /*bone->offsetTransform **/ bone->toParentTransform;
	}

	for (auto child : bone->children)
		UpdateTransforms(child, bonematrix);
}

}

 void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ -100.0f, 180.0f, -100.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 0.0f,50.0f, 0.0f }-mCamera.GetPosition()));

	//lighting
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.direction = H::Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLightBuffer.Initialize();

	mMaterial.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.specular = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.power = 1.0f;
	mMaterialBuffer.Initialize();

	//Matrixs
	mWorldMat.SetIdentity();

	//meshPn
	mMeshpns.emplace_back(MeshPN{ MeshBuilder::CreateShperePN(1.0f,4,{0.0f})});
	//mMeshpns.emplace_back(MeshPN{ MeshBuilder::CreateCubePN(10.0f,5.0f,3.0f,{10.0f}) });
	for (auto& mpn : mMeshpns)
	{
		mMeshpnBufs.emplace_back(MeshBuffer());
		mMeshpnBufs.back().Initialize(mpn);
	}
	mMeshpnVS.Initialize(VertexPN::Format, DO_LIGHTING_FILE_PATH);
	mMeshpnPS.Initialize(DO_LIGHTING_FILE_PATH, "PS");
	mTransformBuffer.Initialize();

	//texture
	mEarth = TextureManager::Get()->LoadTexture("earth.jpg");
	mPalad = TextureManager::Get()->LoadTexture("sponge_bob.png");
 	//
 
	//postproc
	auto gSys = GraphicSystem::Get();
	mRenderTarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);
	mRenderTarget.SetClearColor(0.17f, 0.1f, 0.18f, 1.0f);
	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mPPnothingVS.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPPnothingPS.Initialize(POSTPROCESSING_FILE_PATH, "PS");
	mPPRendertarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

 
	//skeleton
	//mLineSkeleton.Init(L"../../Assets/Skeletons/goofyrun.skeleton");

	//mModel.Initialize(L"../../Assets/Meshes/goofyrun.model");

	mSkinningVShader.Initialize(BoneVertex::Format, SKINNING_FILE_PATH);
	mSkinningPShader.Initialize(SKINNING_FILE_PATH, "PS");
	//H::Graphics::MeshIO::LoadModel(mModel, );

	mMBAObject.Init();

  }

void MyAppState::Terminate()
{
	//meshpn
	mMeshpnVS.Terminate();
	mMeshpnPS.Terminate();
	for (auto& mpn : mMeshpnBufs)
		mpn.Terminate();


	//
	mDirectionalLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	//texture
	

	//postproc
	mPPnothingVS.Terminate();
	mPPnothingPS.Terminate();
	mPPRendertarget.Terminate();


	mMBAObject.Terminate();

   
 }


void MyAppState::Render()
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
 
	mRenderTarget.BeginRender();
	RenderScene();
	DrawGrid();
	mRenderTarget.EndRender();

	PostProcess();
	mMBAObject.GetLineSkeleton().DebugUIViewDraw(mCamera);
}

void MyAppState::Update(float deltatime)
{
	fps = static_cast<float>(1.0f / deltatime);
 	mCamera.UpdateControl(deltatime);
	mMBAObject.Update(deltatime);
}

void MyAppState::DebugUI()
{
	ImGui::ShowDemoWindow();

	//mCamera.DebugUI();
 	ImGui::Begin("Setting");
	ImGui::Text("fps: %f", fps);
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.25f, -1.0f, 1.0f);

		if (directionChanged)
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light",  &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light",  &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.r);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 0.0f, 100.0f);
	}
  	ImGui::End();
 	

	CamDebugUI(mCamera);
	//mLineSkeleton.DeBugUI();
	mMBAObject.DebugUI();
  }

void MyAppState::RenderScene()
{
	//light
	mDirectionalLightBuffer.Set(mDirectionalLight);
	mDirectionalLightBuffer.BindVS(1);
	mDirectionalLightBuffer.BindPS(1);
	//material
	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);


	//transform
	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();
 

	//model
	mMBAObject.Render(vm,pm,mCamera);

}

void MyAppState::PostProcess()
{
 

	mRenderTarget.BindPS(0);
	mPPnothingVS.Bind();
	mPPnothingPS.Bind();
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS(0);

	mScreenMeshBuffer.Render();
	mRenderTarget.UnbindPS(0);
	
}

void MyAppState::DrawGrid() const
{
	//--
	SimpleDraw::AddScreenRect({ {300.0f,200.0f},{100.0f,50.0f} }, Colors::Cyan);
	SimpleDraw::AddScreenCircle({ {300.0f,200.0f},50.0f }, Colors::Cyan);
	for (int i = 1; i < 10; i++)
	{
		SimpleDraw::AddLine({ -100.0f, 0.0f,10.0f * i }, { 100.0f,0.0f,10.0f * i }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -100.0f, 0.0f, - 10.0f * i }, { 100.0f,0.0f,-10.0f * i }, Colors::DarkCyan);

		SimpleDraw::AddLine({ 10.0f * i,0.0f, -100.0f }, { 10.0f * i,0.0f, 100.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -10.0f * i,0.0f,-100.0f }, { -10.0f * i,0.0f,100.0f }, Colors::DarkCyan);

	}
	SimpleDraw::AddLine({ 0.0f }, { 10.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,10.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,10.0f }, Colors::Aqua);//z

	//SimpleDraw::AddLine({ 0.0f }, mCamera.GetPosition() - Vector3{1.0f,1.0f,0.0f}, Colors::GreenYellow);
	//SimpleDraw::AddLine({ 0.0f }, mCamera.GetPosition() - Vector3{ -1.0f,-1.0f,0.0f }, Colors::GreenYellow);
	//SimpleDraw::AddLine({ 0.0f }, mCamera.GetPosition() - Vector3{ 1.0f,-1.0f,0.0f }, Colors::GreenYellow);
	//SimpleDraw::AddLine({ 0.0f }, mCamera.GetPosition() - Vector3{ -1.0f,1.0f,0.0f }, Colors::GreenYellow);

	SimpleDraw::Render(mCamera);

}
