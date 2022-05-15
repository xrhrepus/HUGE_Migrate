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
void UpdateTransforms(Bone* bone, std::vector<Math::Matrix4>& bonematrix)
{
	if (bone->parent)
		bonematrix[bone->index] = bone->toParentTransform * bonematrix[bone->parentIndex];
	else
		bonematrix[bone->index] = bone->toParentTransform;

	for (auto child : bone->children)
		UpdateTransforms(child, bonematrix);
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
	mMeshpns.emplace_back(MeshPN{ MeshBuilder::CreateCubePN(10.0f,5.0f,3.0f,{10.0f}) });
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
	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mPPnothingVS.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPPnothingPS.Initialize(POSTPROCESSING_FILE_PATH, "PS");
	mPPRendertarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	//meshio
 	mMeshes.push_back(MeshBuilder::CreateCube( 5.0f, 1.0f, 3.0f , { 5.0f,0.0f,0.0f }));
	mMeshes.push_back(MeshBuilder::CreateCube(5.0f, 1.0f, 3.0f, { 5.0f,0.0f,0.0f }));
	mMeshes.push_back(MeshBuilder::CreateCube(5.0f, 1.0f, 3.0f, { 5.0f,0.0f,0.0f }));

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
	//mModel.Initialize(L"../../Assets/Meshes/goofyrun.txt");
	AnimationBuilder ab;
	mTestAnim = ab
		.AddRotationKey(H::Math::Quaternion({ 0.0,0.0f,0.0f,1.0f }), 0.0f)
 		.AddRotationKey(H::Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, H::Math::Constants::Pi), 5.0f)
		.AddRotationKey(H::Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, H::Math::Constants::TwoPi), 10.0f)
		.AddPositionKey({0.0f,0.0f,0.0f},0.0f)
		.AddPositionKey({ 100.0f,0.0f,0.0f }, 10.0f)
 		.AddScaleKey({ 0.1f,0.1f,0.1f }, 0.0f)
		.Build();

	//bone
 	mBoneMatrix.resize(mNumBones);

	mBones.emplace_back(Bone());
	mBones.emplace_back(Bone());
	mBones.emplace_back(Bone());

	mBones[0].name = "bone_0";
	mBones[0].index = 0;
	mBones[0].parent = nullptr;
	mBones[0].parentIndex = -1;
	mBones[0].toParentTransform = H::Math::Matrix4::Identity();

	UpdateTransforms(&mBones[0], mBoneMatrix);


	mBones[1].name = "bone_1";
	mBones[1].index = 1;
	mBones[1].parent = &mBones[0];
	mBones[1].parentIndex = 0;
	Matrix4 tm;
	mBones[1].toParentTransform = H::Math::Matrix4::Identity(); //tm.H::Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f });

	mBones[0].children.push_back(&mBones[1]);
	mBones[0].childIndices.push_back(mBones[1].index);

	UpdateTransforms(&mBones[1], mBoneMatrix);

	mBones[2].name = "bone_2";
	mBones[2].index = 2;
	mBones[2].parent = &mBones[1];
	mBones[2].parentIndex = 1;
	mBones[2].toParentTransform = H::Math::Matrix4::Identity(); //tm.H::Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f });
	
	mBones[1].children.push_back(&mBones[2]);
	mBones[1].childIndices.push_back(mBones[2].index);

	UpdateTransforms(&mBones[2], mBoneMatrix);

	//
	mBoneTransCtrls.emplace_back(H::Math::Vector3());
	mBoneTransCtrls.emplace_back(H::Math::Vector3());
	mBoneTransCtrls.emplace_back(H::Math::Vector3());
	mBoneRotCtrls.emplace_back(H::Math::Vector3());
	mBoneRotCtrls.emplace_back(H::Math::Vector3());
	mBoneRotCtrls.emplace_back(H::Math::Vector3());
	mBoneRotAngles.emplace_back(H::Math::Vector3());
	mBoneRotAngles.emplace_back(H::Math::Vector3());
	mBoneRotAngles.emplace_back(H::Math::Vector3());

	//test arm
	mTestArm.Init();

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

	mTestArm.Terminate();
}


void MyAppState::Render()
{
	mRenderTarget.BeginRender();
	RenderScene();
	DrawGrid();
	mRenderTarget.EndRender();

	PostProcess();
}

void MyAppState::Update(float deltatime)
{
	fps = static_cast<float>(1.0f / deltatime);
	mTestTimer += deltatime;
	if (mTestTimer > 10.0f)
	{
		mTestTimer = 0.0f;
	}
	mCamera.UpdateControl(deltatime);
	mTestArm.Update(deltatime);
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
	if (ImGui::CollapsingHeader("Animation"))
	{
		ImGui::Text("current bone : ");
		ImGui::SameLine();
		if (ImGui::ArrowButton("prev", 0)) mBone_currChoose -= mBone_currChoose > 0 ? 1 : 0;
		ImGui::SameLine();
		ImGui::Text(" % d ", mBone_currChoose);
		ImGui::SameLine();
		if (ImGui::ArrowButton("next", 1)) mBone_currChoose += mBone_currChoose < 2 ? 1 : 0;

		//(ImGui::DragInt("Current Bone", &mBone_currChoose, 1.0, 0, mBones.size() - 1));
		
		Vector3 prevDir = mBoneTransCtrls[mBone_currChoose];
		if (ImGui::DragFloat("translate x", &mBoneTransCtrls[mBone_currChoose].x,0.01f))
		{
			Matrix4 m;
			m.Translation({ mBoneTransCtrls[mBone_currChoose].x,mBoneTransCtrls[mBone_currChoose].y,mBoneTransCtrls[mBone_currChoose].z });
			mBones[mBone_currChoose].toParentTransform = m;

			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
		}

		if (ImGui::DragFloat("translate y", &mBoneTransCtrls[mBone_currChoose].y, 0.01f))
		{
			Matrix4 m;
			m.Translation({ mBoneTransCtrls[mBone_currChoose].x,mBoneTransCtrls[mBone_currChoose].y,mBoneTransCtrls[mBone_currChoose].z });
			mBones[mBone_currChoose].toParentTransform = m;

			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
 		}

		if (ImGui::DragFloat("translate z", &mBoneTransCtrls[mBone_currChoose].z, 0.01f))
		{
			Matrix4 m;
			m.Translation({ mBoneTransCtrls[mBone_currChoose].x,mBoneTransCtrls[mBone_currChoose].y,mBoneTransCtrls[mBone_currChoose].z });
			mBones[mBone_currChoose].toParentTransform = m;

			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
		}

		bool rotationChanged = false;
		Vector3 prevRot = mBoneRotCtrls[mBone_currChoose];
		ImGui::Text("[Rotation]: x[%f],y[%f],z[%f] ", mBoneRotAngles[mBone_currChoose].x, mBoneRotAngles[mBone_currChoose].y, mBoneRotAngles[mBone_currChoose].z);
		if (ImGui::DragFloat("rotation x", &mBoneRotCtrls[mBone_currChoose].x,0.01f/*,-H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{
			mBoneRotAngles[mBone_currChoose].x += mBoneRotCtrls[mBone_currChoose].x - prevRot.x;
 			mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 1.0f,0.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].x - prevRot.x)) * mBones[mBone_currChoose].toParentTransform;
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
			rotationChanged = true;

		}
		if (ImGui::DragFloat("rotation y", &mBoneRotCtrls[mBone_currChoose].y, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{
			mBoneRotAngles[mBone_currChoose].y += mBoneRotCtrls[mBone_currChoose].y - prevRot.y;

			mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].y - prevRot.y))* mBones[mBone_currChoose].toParentTransform;
 
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
			rotationChanged = true;

		}
		if (ImGui::DragFloat("rotation z", &mBoneRotCtrls[mBone_currChoose].z, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{
			mBoneRotAngles[mBone_currChoose].x += mBoneRotCtrls[mBone_currChoose].z - prevRot.z;

			mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,0.0f,1.0f }, mBoneRotCtrls[mBone_currChoose].z - prevRot.z))* mBones[mBone_currChoose].toParentTransform;
 
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
			rotationChanged = true;
		}
		//if (rotationChanged)
		//{
		//	mBoneRotCtrls[mBone_currChoose] = Math::Normalize(mBoneRotCtrls[mBone_currChoose]);
		//	mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionFromToRotation(prevDir,mBoneRotCtrls[mBone_currChoose])) * mBones[mBone_currChoose].toParentTransform;
		//	UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);

		//}



		if(ImGui::Button("reset bone"))
		{
			for (auto& c : mBoneTransCtrls)
			{
				c = Vector3();
			}
			for (auto& c : mBoneRotCtrls)
			{
				c = Vector3();
			}
			for (auto& a :mBoneRotAngles)
			{
				a = Vector3();
			}
			mBones[mBone_currChoose].toParentTransform = H::Math::Matrix4::Identity();
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);

		}

 


	}
 	ImGui::End();

	mTestArm.DeBugUI();
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

	//=======================================

	mMeshVShader.Bind();
	mMeshPShader.Bind();


	SamplerManager::Get()->GetSampler("LinearWarp")->BindPS();
	TextureManager::Get()->GetTexture(mPalad)->BindPS(0);

	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	//int index = 0;
	//for (const auto& mbf : mMeshBufs)
	//{
	//	
	//	comp = mBoneMatrix[index] * vm * pm;
	//	tf.world = H::Math::Transpose(mBoneMatrix[index]);
	//	tf.wvp = H::Math::Transpose(comp);
	//	mTransformBuffer.Set(tf);

	//	mbf.Render();
	//	index++;
	//}
	//test arm
	mTestArm.Render(mTransformBuffer, tf, vm, pm);

	//=======================================
	/*mMeshTransform = mTestAnim.GetTransform(mTestTimer);
	comp = mMeshTransform * vm * pm;
	tf.world = H::Math::Transpose(mMeshTransform);
	tf.wvp = H::Math::Transpose(comp);
	mTransformBuffer.Set(tf);

	mModel.Render();*/
	//mMeshToLoadBuffer.Render();
 
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
	for (int i = 0; i < 10; i++)
	{
		SimpleDraw::AddLine({ -100.0f, 10.0f * i,0.0f }, { 100.0f,10.0f * i,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -100.0f, -10.0f * i,0.0f }, { 100.0f,-10.0f * i,0.0f }, Colors::DarkCyan);

		SimpleDraw::AddLine({ 10.0f * i, -100.0f,0.0f }, { 10.0f * i, 100.0f,0.0f }, Colors::DarkCyan);
		SimpleDraw::AddLine({ -10.0f * i,-100.0f,0.0f }, { -10.0f * i,100.0f,0.0f }, Colors::DarkCyan);

	}
	SimpleDraw::AddLine({ 0.0f }, { 10.0f,0.0f,0.0f }, Colors::Magenta);//x
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,10.0f,0.0f }, Colors::LawnGreen);//y
	SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,10.0f }, Colors::Aqua);//z

	SimpleDraw::Render(mCamera);

}
