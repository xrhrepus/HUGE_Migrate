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
	mCamera.SetPosition({ 0.0f, 0.0f, 30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, -1.0f });

	//
	mEarth_Mesh = MeshBuilder::CreateShpere(6,4, { 0.0f,0.0f,0.0f });
	mMeshPN = MeshBuilder::CreateShperePN(5,8, { 0.0f,0.0f,0.0f });
	
	mSkyBox = MeshBuilder::CreateShperePX(20.0f, 6, { 0.0f,0.0f,0.0f });
 
	//mVertexShader.Initialize(Vertex::Format, STANDARD_FILE_PATH);
 
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

	//===
	mMeshBufferPN.Initialize(mMeshPN);
	mVertexShaderPN.Initialize(VertexPN::Format, DO_LIGHTING_FILE_PATH);
	mPixelShaderPN.Initialize( DO_LIGHTING_FILE_PATH, "PS");

 	//lighting
	mLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mLight.specular = { 0.8f,0.8f,0.8f,1.0f };
	mLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mLight.direction = H::Math::Normalize({ 0.0f,0.0f,-1.0f });

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


	//texture
	mDiffuseMap = TextureManager::Get()->LoadTexture("earth.jpg");
	mSpongeBob = TextureManager::Get()->LoadTexture("sponge_bob.png");
	mSpecularMap = TextureManager::Get()->LoadTexture("earth_spec.jpg");
	mDisplacementMap = TextureManager::Get()->LoadTexture("earth_bump.jpg");
	mNormalMap = TextureManager::Get()->LoadTexture("earth_normal.jpg");
	mSkyboxTex = TextureManager::Get()->LoadTexture("universe.jpg");
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

	//test effect
	mTestEffectStandard.Init(STANDARD_FILE_PATH);

 //	mTestEffectStandard.SetValue_DiffuseMap(mDiffuseMap);
	//mTestEffectStandard.SetValue_DisplacementMap(mDisplacementMap);
	//mTestEffectStandard.SetValue_NormalMap(mNormalMap);
	//mTestEffectStandard.SetValue_SpecularMap(mSpecularMap);
	mTestEffectStandard.SetValue_SamplerType("LinearWarp");

	mTestEffectStandard.SetPointer_TransformBuffer(&mTransformBuffer);
	mTestEffectStandard.SetPointer_LightBuffer(&mLightBuffer);
	mTestEffectStandard.SetPointer_MaterialBuffer(&mMaterialBuffer);
	mTestEffectStandard.SetPointer_OptionsBuffer(&mOptionsBuffer);


	//
	mTestDoTexture.Init(TEXTURING_FILE_PATH);
  
	mTestDoTexture.SetPointer_TransformBuffer(&mTransformBuffer);
	mTestDoTexture.SetValue_DiffuseMap(mSkyboxTex);

	mTestDoTexture.SetValue_SamplerType("LinearWarp");

	//
	mQuad = MeshBuilder::CreateQuad(10.0f, 10.0f, { 0.0f });
	mQuadBuffer.Initialize(mQuad);
	//mQuadDiffuse = TextureManager::Get()->LoadTexture("fruit42_x10.png");
	mQuadDiffuse = TextureManager::Get()->LoadTexture("sheep64_normaltest_0.png");

	mQuadNormal = TextureManager::Get()->LoadTexture("sheep64_normalmap.png");

	//
	mCube = MeshBuilder::CreateCube(5.0f, 2.0f, 6.0f, { 2.0f });
	mCubeBuffer.Initialize(mCube);
}

void MyAppState::Terminate()
{
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

	//test effect
	mTestEffectStandard.Terminate();
	mTestDoTexture.Terminate();

	//
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();
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
	//RasterizerStateManager::Get()->GetRasterizerState("CullNoneSolid")->Set();
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

	mTestEffectStandard.SetValue_LightBuffer(mLight);
	mTestEffectStandard.SetValue_MaterialBuffer(mMaterial);

	world.SetIdentity();
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);

	//standard start
	mTestEffectStandard.Bind();
	//set values
	mTestEffectStandard.SetValue_TransformBuffer(tf);
	mTestEffectStandard.SetValue_OptionsBuffer(mOptionsData);
	mTestEffectStandard.SetValue_DiffuseMap(mQuadDiffuse);
	mTestEffectStandard.SetValue_NormalMap(mQuadNormal);
	mCubeBuffer.Render();
	mQuadBuffer.Render();

 	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
 
	//mesh 2
	mWorldMat1.Translation({ 0.0f,0.0f,-20.0f });
	world = (mRotation1*mWorldMat1);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	mTestEffectStandard.SetValue_TransformBuffer(tf);
	mTestEffectStandard.SetValue_DiffuseMap(mSpongeBob);
	//mTestEffectStandard.Bind();
	//mTestEffectStandard.Bind();

	mMeshBuffer.Render();

	//quad


	////skybox
	Matrix4 scale;
	scale.Scaling(10.0f);
	world = (scale);
	comp = world * vm * pm;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	//bind===
	RasterizerStateManager::Get()->GetRasterizerState("CullFrontSolid")->Set();
	
	mTestDoTexture.SetValue_TransformBuffer(tf);
	mTestDoTexture.Bind();
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

 
	//
	//p
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
