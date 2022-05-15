#include "MyAppState.h"
#include "Graphics\Src\MeshIO.h"
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;

namespace
{
	float fps;
}

 void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ 0.0f, 0.0f, -30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

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
	mMeshpns.emplace_back(MeshPN{ MeshBuilder::CreateShperePN(4.0f,2,{20.0f}) });
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
	mPalad = TextureManager::Get()->LoadTexture("paladin_prop_j_nordstrom_diffuse_..png");
	//mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	//mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	//mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	//mSkyboxTex = TextureManager::Get()->LoadTexture("universe.jpg");
	//mParticleTex = TextureManager::Get()->LoadTexture("fruit42.png");
	//
 
	//postproc
	auto gSys = GraphicSystem::Get();
	mRenderTarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);
	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mPPnothingVS.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPPnothingPS.Initialize(POSTPROCESSING_FILE_PATH, "PS");
	mPPRendertarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	//meshio
	//H::Graphics::MeshIO::Load(mMeshToLoad, L"../Assets/Meshes/meshPalad.txt");
	H::Graphics::MeshIO::Load(mMeshes, L"../../Assets/Meshes/meshPalad.txt");
	mMeshBufs.resize(mMeshes.size());
	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		mMeshBufs[i].Initialize(mMeshes[i]);
	}
	
	//mMeshToLoadBuffer.Initialize(mMeshToLoad);
	mMeshTransform.SetIdentity();
	mMeshVShader.Initialize(Vertex::Format, STANDARD_FILE_PATH);
	mMeshPShader.Initialize(STANDARD_FILE_PATH,"PS");


	//model
	mModel.Initialize(L"../../Assets/Meshes/goofyrun.txt");
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

 	//MESHIO
	//mMeshToLoadBuffer.Terminate();
	for (auto& mbf : mMeshBufs)
	{
		mbf.Terminate();
	}
	mMeshVShader.Terminate();
	mMeshPShader.Terminate();

	mModel.Terminate();
}


void MyAppState::Render()
{
	mRenderTarget.BeginRender();
	RenderScene();
	mRenderTarget.EndRender();

	PostProcess();
	DrawGrid();
}

void MyAppState::Update(float deltatime)
{
	fps = static_cast<float>(1.0f / deltatime);
	mCamera.UpdateControl(deltatime);
  }

void MyAppState::DebugUI()
{
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

}

void MyAppState::RenderScene()
{
	//transform
	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();
	Matrix4 world = (mWorldMat);
	Matrix4 comp = world * vm * pm;

	TransformData tf;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(tf);
	mTransformBuffer.BindVS(0);// b0 slot
	mTransformBuffer.BindPS(0);

	//light
	mDirectionalLightBuffer.Set(mDirectionalLight);
	mDirectionalLightBuffer.BindVS(1);
	mDirectionalLightBuffer.BindPS(1);
	//material
	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	//bind===
	mMeshpnVS.Bind();
	mMeshpnPS.Bind();

	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
 	for (const auto& mpn : mMeshpnBufs)
	{
		mpn.Render();
	}

	//inported mesh
	mMeshVShader.Bind();
	mMeshPShader.Bind();

	comp = mMeshTransform * vm * pm;

	tf.world = H::Math::Transpose(mMeshTransform);
	tf.wvp = H::Math::Transpose(comp);
	mTransformBuffer.Set(tf);

	SamplerManager::Get()->GetSampler("LinearWarp")->BindPS();
	TextureManager::Get()->GetTexture(mPalad)->BindPS(0);

	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	//for (const auto& mbf : mMeshBufs)
	//{
	//	mbf.Render();
	//}
	mModel.Render();
	//mMeshToLoadBuffer.Render();
 
}

void MyAppState::PostProcess()
{
	//mPPRendertarget.BeginRender();
	//SamplerManager::Get()->GetSampler("PointClamp")->BindPS(0);

	//mRenderTarget.BindPS(0);

	//mPPRendertarget.BindPS(1);
	//mPPnothingVS.Bind();
	//mPPnothingPS.Bind();

	//mScreenMeshBuffer.Render();
	//mPPRendertarget.EndRender();

	//mRenderTarget.UnbindPS(0);

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
	for (int i = 0; i < 10; i++)
	{
		SimpleDraw::AddLine({ -100.0f, 10.0f * i,0.0f }, { 100.0f,10.0f * i,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -100.0f, -10.0f * i,0.0f }, { 100.0f,-10.0f * i,0.0f }, Colors::DarkCyan);

		SimpleDraw::AddLine({ 10.0f * i, -100.0f,0.0f }, { 10.0f * i, 100.0f,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -10.0f * i,-100.0f,0.0f }, { -10.0f * i,100.0f,0.0f }, Colors::DarkCyan);

	}
	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::Aqua);//z

	SimpleDraw::Render(mCamera);

}
