#include "MyAppState.h"

float fps;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
//void ShowDebugUi(float deltatime);
//float ComputeGaussian(float n)
//{
//	// 高斯参数计算公式
//	float theta = m_pParam->_BlurAmount;
//	return (float)((1.0 / sqrt(2 * H::Math::Constants::Pi * theta)) *
//		exp(-(n * n) / (2 * theta * theta)));
//}
void MyAppState::Initialize()
{
	//camera
	mCamera.SetPosition({ 0.0f, 0.0f, -30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//
	mEarth_Mesh = MeshBuilder::CreateShpere(7,8, { 0.0f,0.0f,0.0f });
	mMeshPN = MeshBuilder::CreateShperePN(5,8, { 0.0f,0.0f,0.0f });
	
	mSkyBox = MeshBuilder::CreateShperePX(20.0f, 6, { 0.0f,0.0f,0.0f });

	//
	mSkyBoxMeshBuffer.Initialize(mSkyBox);
	mVertexShaderPX.Initialize(VertexPX::Format, TEXTURING_FILE_PATH);
	mPixelShaderPX.Initialize(TEXTURING_FILE_PATH,"PS");
	//
	mMeshBuffer.Initialize(mEarth_Mesh);
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mOptionsBuffer.Initialize();
	mBloomBuffer.Initialize();
	//
	mVertexShader.Initialize(Vertex::Format, STANDARD_FILE_PATH);
	mPixelShader.Initialize(STANDARD_FILE_PATH, "PS");

	//===
	mMeshBufferPN.Initialize(mMeshPN);
	mVertexShaderPN.Initialize(VertexPN::Format, DO_LIGHTING_FILE_PATH);
	mPixelShaderPN.Initialize( DO_LIGHTING_FILE_PATH, "PS");

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


	mWorldMat1.Translation({-10.0f,20.0f,0.0f});
	mRotation1.RotationX(rot_x /2.0f );

	//mWorldMat2.RotationX(H::Math::Constants::Pi / 2.0f);
	mWorldMat2.Translation({ 10.0f,-10.0f,0.0f });


	//texture
	mDiffuseMap = TextureManager::Get()->LoadTexture("earth.jpg");
	mSpongeBob = TextureManager::Get()->LoadTexture("sheep64_normaltest_0.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mSkyboxTex = TextureManager::Get()->LoadTexture("universe1024.jpg");
	mParticleTex = TextureManager::Get()->LoadTexture("alphaSheep128.png");
	//
 

	//postproc
	auto gSys = GraphicSystem::Get();
	mRenderTarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	mPostProc_Bloom_RenderTargets.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
			RenderTarget::Format::RGBA_U8);
 
	mPostProc_None_RenderTargets.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	mScreenMesh = MeshBuilder::CreateNDCQuadPX();//create screen quad;

	//
	mPostProc_Combine_VertexShader.Initialize(VertexPX::Format, POSTPROCESSING_COMBINE_FILE_PATH);
	mPostProc_Combine_PixelShader.Initialize(POSTPROCESSING_COMBINE_FILE_PATH, "PS");

	//
	mPostProc_NoneVertexShader.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPostProc_NonePixelShader.Initialize(POSTPROCESSING_FILE_PATH, "PS");

	mScreenMeshBuffer.Initialize(mScreenMesh);

	//bloom params
	mPostProc_BloomVertexShader.Initialize(VertexPX::Format, POSTPROCESSING_BLOOM_FILE_PATH);
	mPostProc_BloomPixelShader.Initialize(POSTPROCESSING_BLOOM_FILE_PATH, "PS");

	mBloomData.baseIntensity = 1.0f;
	mBloomData.baseSaturation = 1.0f;
	mBloomData.bloomIntensity = 1.0f;
	mBloomData.bloomSaturation = 1.0f;


	mBloomData.u_step = 1.0f / GraphicSystem::Get()->GetBackBufferWidth();
	mBloomData.v_step = 1.0f / GraphicSystem::Get()->GetBackBufferHeight();
	mBloomData.decay = 0.01f;
	mBloomData.blurIntensity = 0.8f;
	mBloomData.radiation = 0.08f; 
	mBloomData.offset = 5.0f;
 
	//particle
	mParticle_VertexShader.Initialize(Graphics::VertexPNC::Format, PARTICLE_FILE_PATH);
	mParticle_PixelShader.Initialize(PARTICLE_FILE_PATH,"PS");

	mParticles.vertices.push_back(H::Graphics::VertexPNC(H::Math::Vector3(0.0f), H::Math::Vector3(1.0f, 1.0f, 1.0f), Colors::Aqua));
	mParticles.indices.push_back(0);

	mParticle_MeshBuffer.Initialize(mParticles);
	mParticle_ConstantBuffer.Initialize();

	mParticle_GeoShader.Initialize(PARTICLE_FILE_PATH);
	mParticle_worldmat1.SetIdentity();
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

	//
	mRenderTarget.Terminate();
	mPostProc_Bloom_RenderTargets.Terminate();
	mPostProc_None_RenderTargets.Terminate();

	mPostProc_BloomVertexShader.Terminate();
	mPostProc_BloomPixelShader.Terminate();
	mPostProc_NoneVertexShader.Terminate();
	mPostProc_NonePixelShader.Terminate();
	mPostProc_Combine_VertexShader.Terminate();
	mPostProc_Combine_PixelShader.Terminate();

	//particlesys
	mParticle_GeoShader.Terminate();
	mParticle_MeshBuffer.Terminate();
	mParticle_ConstantBuffer.Terminate();
	mParticle_VertexShader.Terminate();
	mParticle_PixelShader.Terminate();
}


void MyAppState::Render()
{
	mRenderTarget.BeginRender();
	RenderScene();
	mRenderTarget.EndRender();

	PostProcess();
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
	if (ImGui::CollapsingHeader("Post processing"))
	{
		ImGui::TextDisabled("[------Bloom---------]");
		ImGui::Checkbox("On/Off##Bloom", &isBloomOn);
		ImGui::DragFloat("threshold##Bloom", &mBloomData.bloomThreshold, 0.01f, 0.0f, 0.999f);
		ImGui::DragFloat("bloom_intensity##Bloom", &mBloomData.bloomIntensity, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("bloom_saturation##Bloom", &mBloomData.bloomSaturation, 0.01f, 0.0f, 1.0f);

		ImGui::TextDisabled("[------SimpleBlur---------]");
		if(ImGui::DragFloat("blur_radiation##Bloom", &mBloomData.radiation, 0.001f, 0.01f, 1.0f))
		{
			float r = mBloomData.radiation / (60.0f * mBloomData.u_step);
			float r2 = mBloomData.radiation / (20.0f * mBloomData.u_step);

			if (mBloomData.offset < r)
			{
				mBloomData.offset = r;
			}
			else if (mBloomData.offset > r2)
			{
				mBloomData.offset = r2;
			}
		}

		ImGui::DragFloat("blur_dacay##Bloom", &mBloomData.decay, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("blur_intensity##Bloom", &mBloomData.blurIntensity, 0.01f, 0.0f, 1.0f);
		if(ImGui::DragFloat("blur_offset##Bloom", &mBloomData.offset, 0.1f, 1.0f, 10.0f))
		{
			if (mBloomData.radiation > 60.0f * mBloomData.u_step * mBloomData.offset)
			{
				mBloomData.radiation = 60.0f * mBloomData.u_step * mBloomData.offset;
			}
			else if (mBloomData.radiation < 20.0f * mBloomData.u_step * mBloomData.offset)
			{
				mBloomData.radiation = 20.0f * mBloomData.u_step * mBloomData.offset;
			}
		}

		//ImGui::TextDisabled("[------BaseScene---------]");
		//ImGui::DragFloat("base_intensity##Bloom", &mBloomData.baseIntensity, 0.01f, 0.0f, 1.0f);
		//ImGui::DragFloat("base_saturation##Bloom", &mBloomData.baseSaturation, 0.01f, 0.0f, 1.0f);
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

void MyAppState::RenderScene()
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
	//rotation
	Matrix4 rx, ry, rz;
	rx.RotationX(rot_x);
	ry.RotationY(rot_y);
	rz.RotationZ(rot_z);

	mRotation1 = rx * rz *ry;

	Matrix4 vm = mCamera.GetViewMatrix();
	Matrix4 pm = mCamera.GetPerspectiveMatrix();

	//mesh 1
	mWorldMat1.Translation({ -10.0f,0.0f,0.0f });

	Matrix4 world = (mRotation1*mWorldMat1);
	Matrix4 comp = world * vm * pm;

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

	SamplerManager::Get()->GetSampler(SamplerType::LinearWrap)->BindPS();
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
	TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);
	TextureManager::Get()->GetTexture(mDisplacementMap)->BindVS(2);

	TextureManager::Get()->GetTexture(mNormalMap)->BindPS(3);// ?


	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	mMeshBuffer.Render();

	//mesh 2
	mWorldMat1.Translation({ 0.0f,0.0f,-20.0f });
	world = (mRotation1*mWorldMat1);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	mTransformBuffer.Set(tf);
	TextureManager::Get()->GetTexture(mSpongeBob)->BindPS(0);

	mMeshBuffer.Render();


	////skybox
	Matrix4 scale;
	scale.Scaling(10.0f);
	world = (scale);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	//bind===
	mVertexShaderPX.Bind();
	mPixelShaderPX.Bind();
 
	mTransformBuffer.Set(tf);

	SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
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
  
 	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
 
	mMeshBufferPN.Render();

	//particle
	//comp = mParticle_worldmat1 * vm * pm;
	//mGeometryData.wvp = H::Math::Transpose(comp);
	//mGeometryData.camPosition = mCamera.GetPosition();
	//mGeometryData.camUp = mCamera.GetCameraUp();

	//mParticle_ConstantBuffer.Set(mGeometryData);
	//mParticle_ConstantBuffer.BindVS(0);
	//mParticle_ConstantBuffer.BindGS(0);
	//mParticle_ConstantBuffer.BindPS(0);


	////update vertices
	//mParticle_VertexShader.Bind();
	//mParticle_GeoShader.Bind();
	//mParticle_PixelShader.Bind();


	//TextureManager::Get()->GetTexture(mParticleTex)->BindPS(0);
	//SamplerManager::Get()->GetSampler(SamplerType::LinearWrap)->BindPS();
 //	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	//mParticle_MeshBuffer.SetTopology(MeshBuffer::Topology::Points);
	//mParticle_MeshBuffer.Render();

	//mParticle_GeoShader.UnBind();
	//
	//p
	//--
	//for (int i = 1; i < 10; i++)
	//{
	//	SimpleDraw::AddLine({ -100.0f, 0.0f,10.0f * i }, { 100.0f,0.0f,10.0f * i }, Colors::DarkCyan);
	//	SimpleDraw::AddLine({ -100.0f, 0.0f, -10.0f * i }, { 100.0f,0.0f,-10.0f * i }, Colors::DarkCyan);

	//	SimpleDraw::AddLine({ 10.0f * i,0.0f, -100.0f }, { 10.0f * i,0.0f, 100.0f }, Colors::DarkCyan);
	//	SimpleDraw::AddLine({ -10.0f * i,0.0f,-100.0f }, { -10.0f * i,0.0f,100.0f }, Colors::DarkCyan);

	//}
	//SimpleDraw::AddLine({ 0.0f }, { 10.0f,0.0f,0.0f }, Colors::Magenta);//x
	//SimpleDraw::AddLine({ 0.0f }, { 0.0f,10.0f,0.0f }, Colors::LawnGreen);//y
	//SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,10.0f }, Colors::Aqua);//z
	//SimpleDraw::Render(mCamera);
 
}

void MyAppState::PostProcess()
{
	mRenderTarget.BindPS(0);//orgin

	if (isBloomOn)
	{
		mPostProc_Bloom_RenderTargets.BeginRender();

		mPostProc_BloomVertexShader.Bind();
		mPostProc_BloomPixelShader.Bind();
		SamplerManager::Get()->GetSampler("PointClamp")->BindPS(0);
  
		mBloomBuffer.Set(mBloomData);
		mBloomBuffer.BindVS(0);
		mBloomBuffer.BindPS(0);
		mScreenMeshBuffer.Render();
		mPostProc_Bloom_RenderTargets.EndRender();
	}
 
	mPostProc_Bloom_RenderTargets.BindPS(1);

	mPostProc_Combine_VertexShader.Bind();
	mPostProc_Combine_PixelShader.Bind();
 
	
	mScreenMeshBuffer.Render();//draw

	mRenderTarget.UnbindPS(0);
	mPostProc_Bloom_RenderTargets.UnbindPS(1);

	//reset shader res view
 	mPostProc_NoneVertexShader.Bind();
	mPostProc_NonePixelShader.Bind();

	mPostProc_Bloom_RenderTargets.BeginRender();
	mScreenMeshBuffer.Render();
	mPostProc_Bloom_RenderTargets.EndRender();

 
}
