
#include "MyAppState.h"

 
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
void ShowDebugUi(float deltatime);

void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ 0.0f, 0.0f, -30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//
	mEarth_Mesh = MeshBuilder::CreateShpere(6,11, { 0.0f,0.0f,0.0f });
	mMeshPN = MeshBuilder::CreateShperePN(5,2, { 0.0f,0.0f,0.0f });
	
	mSkyBox = MeshBuilder::CreateShperePX(20.0f, 6, { 0.0f,0.0f,0.0f });

	//
	mSkyBoxMeshBuffer.Initialize(mSkyBox);
	mVertexShaderPX.Initialize(VertexPX::Format, TEXTURING_FILE_PATH);
	mPixelShaderPX.Initialize(TEXTURING_FILE_PATH);
	//
	mMeshBuffer.Initialize(mEarth_Mesh);
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();

	//
	mVertexShader.Initialize(Vertex::Format, STANDARD_FILE_PATH);
	mPixelShader.Initialize(STANDARD_FILE_PATH);

	//===
	mMeshBufferPN.Initialize(mMeshPN);
	mVertexShaderPN.Initialize(VertexPN::Format, DO_LIGHTING_FILE_PATH);
	mPixelShaderPN.Initialize( DO_LIGHTING_FILE_PATH);

	//mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Warp);
	//lighting
	mLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mLight.specular = { 0.8f,0.8f,0.8f,1.0f };
	mLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mLight.direction = H::Math::Normalize({ 1.0f,-1.0f,1.0f });

	mMaterial.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.specular = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mMaterial.power = 1.0f;

	//
	mWorldMat1.SetIdentity();
	mWorldMat2.SetIdentity();


	mWorldMat1.Translation({-10.0f,0.0f,0.0f});
	mRotation1.RotationX(rot_x /2.0f );

	//mWorldMat2.RotationX(H::Math::Constants::Pi / 2.0f);
	mWorldMat2.Translation({ 10.0f,0.0f,0.0f });


	//
	mDiffuseMap = TextureManager::Get()->LoadTexture("earth.jpg");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mSkyboxTex = TextureManager::Get()->LoadTexture("sponge_bob.png");
	//
	//BlendStateManager::Get()->GetBlendState("Opaque")->Set();

}

void MyAppState::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mVertexShaderPN.Terminate();
	mPixelShaderPN.Terminate();


	mVertexShaderPX.Terminate();
	mPixelShaderPX.Terminate();


	//mSampler.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMeshBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mOptionsBuffer.Terminate();
}

void MyAppState::Render()
{


	//rotation
	Matrix4 rx, ry, rz;
	rx.RotationX(rot_x);
	ry.RotationY(rot_y);
	rz.RotationZ(rot_z);

	mRotation1 = rx * rz *ry;

	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();
 
	//mesh 1
	Matrix4 world = (mRotation1*mWorldMat1);
	Matrix4 comp = world * vm * pm ;

	TransformData tf;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();

	//bind===
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.Set(tf);
	mTransformBuffer.BindVS(0);// b0 slot
	mTransformBuffer.BindPS(0);

	mLightBuffer.Set(mLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Set(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);


	mOptionsBuffer.Set(mOptionsData);
	mOptionsBuffer.BindVS(3);
	mOptionsBuffer.BindPS(3);

	SamplerManager::Get()->GetSampler("LinearWarp")->BindPS();
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
	TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);
	TextureManager::Get()->GetTexture(mDisplacementMap)->BindVS(2);

	TextureManager::Get()->GetTexture(mNormalMap)->BindPS(3);// ?


	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	mMeshBuffer.Render();

	//skybox
	Matrix4 scale;
	scale.Scaling(10.0f);
	world = (scale);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	//bind===
	//mVertexShaderPX.Bind();
	//mPixelShaderPX.Bind();
	mVertexShader.Bind();
	mPixelShader.Bind();

	//mConstantBuffer.Set(tf);
	//mConstantBuffer.BindVS(0);// b0 slot
	//mConstantBuffer.BindPS(0);

 

 
	SamplerManager::Get()->GetSampler("LinearWarp")->BindPS();
	TextureManager::Get()->GetTexture(mSkyboxTex)->BindPS(0);
 	RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Set();

	mSkyBoxMeshBuffer.Render();


	//mesh 2
	mVertexShaderPN.Bind();
	mPixelShaderPN.Bind();

	comp = mWorldMat2 * vm * pm;
 
	tf.world = H::Math::Transpose(mWorldMat2);
	tf.wvp = H::Math::Transpose(comp);
	mTransformBuffer.Set(tf);

	
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
	TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);

	//RasterizerStateManager::Get()->GetRasterizerState("WireFrame")->Set();
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();



	mMeshBufferPN.Render();

	//--
	SimpleDraw::AddScreenRect({ {300.0f,200.0f},{100.0f,50.0f} }, Colors::Cyan);
	SimpleDraw::AddScreenCircle({ {300.0f,200.0f},50.0f }, Colors::Cyan);
	for (int i = 0; i < 10; i++)
	{
		SimpleDraw::AddLine({ -100.0f, 10.0f * i,0.0f }, { 100.0f,10.0f * i,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -100.0f, -10.0f * i,0.0f }, { 100.0f,-10.0f * i,0.0f }, Colors::DarkCyan);

		SimpleDraw::AddLine({  10.0f * i, -100.0f,0.0f }, { 10.0f * i, 100.0f,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -10.0f * i,-100.0f,0.0f }, { -10.0f * i,100.0f,0.0f }, Colors::DarkCyan);

	}
	SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::Aqua);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::LawnGreen);//z

	SimpleDraw::Render(mCamera);
	//RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();




}

void MyAppState::Update(float deltatime)
{
	mCamera.UpdateControl(deltatime);
 

}

void MyAppState::DebugUI()
{
	//ImGui::ShowDemoWindow();
	ImGui::Begin("Setting");
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
	if (ImGui::CollapsingHeader("Earth Transform"))
	{
		ImGui::DragFloat("RotationX##Transform", &rot_x, 0.1f, - H::Math::Constants::TwoPi, H::Math::Constants::TwoPi);
		ImGui::DragFloat("RotationY##Transform", &rot_y, 0.1f, - H::Math::Constants::TwoPi, H::Math::Constants::TwoPi);
		ImGui::DragFloat("RotationZ##Transform", &rot_z, 0.1f, - H::Math::Constants::TwoPi, H::Math::Constants::TwoPi);

		ImGui::DragFloat("bump##Transform", &mOptionsData.displacementWeight, 0.01f, 0.0f, 1.0f);
		bool normalMap = mOptionsData.useNormal > 0.0f;
		if (ImGui::Checkbox("normal map" , &normalMap))
		{
			mOptionsData.useNormal = normalMap ? 1.0f : 0.0f;
		}


		if(ImGui::Button("Reset"))
		{
			rot_x = 0.0f;
			rot_y = 0.0f;
			rot_z = 0.0f;

		}
	}
	ImGui::End();

}
