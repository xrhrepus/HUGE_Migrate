#include "TestShadow.h"

 

void TestShadow::Initialize(Camera* cam)
{
	auto txMngr = TextureManager::Get();
	mDepthMap.Initialize();
 
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

	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.01f, mMesh);
	mContex.diffuse = txMngr->LoadTexture("../../Assets/Models/Tank/tank_diffuse.jpg");
	mContex.normal = txMngr->LoadTexture("../../Assets/Models/Tank/tank_normal.jpg");
	mContex.camera = cam;
	mContex.directionalLight = mLight;
	mContex.material = mMaterial;

	//
	mRenderObj.Initialize(mContex, "TestShadowObj");
	mNSE.Initialize();
	
	{
		tankContext.camera = cam;
		tankContext.diffuse = mContex.diffuse;
		tankContext.normal = mContex.normal;
		tankContext.light = mLight;
		tankContext.material = mMaterial;
		tankContext.setting.useShadow = true;
		tankContext.setting.normalMapWeight = 1.0f;

	}
 
 	//
	{
		mQuad = MeshBuilder::CreateQuad(250.0f, 250.0f, { 0.0f });
		mQuadBuf.Initialize(mQuad);
		quadContext.camera = cam;
		quadContext.diffuse = mQuadId;
		quadContext.normal = mContex.normal;
		quadContext.light = mLight;
		quadContext.material = mMaterial;
		quadContext.setting.useShadow = true;
		quadContext.setting.normalMapWeight = 1.0f;

	}
	{
		mainCam = cam;
		lightCam = &(mDepthMap.mLightViewCam);
	}
}

void TestShadow::Terminate()
{
	mRenderObj.Terminate();
	mDepthMap.Terminate();
	mNSE.Terminate();

	mQuadBuf.Terminate();
}

void TestShadow::Update(float deltatime)
{
}

void TestShadow::Render()
{
	//draw depthmap
	mDepthMap.Begin();
	mDepthMap.DrawDepthMap(mRenderObj.mTransform.GetTransformMatrix());
	mRenderObj.mMeshRenderer.mStandardContext.meshBuffer->Render();
 
	mDepthMap.End();

	//draw mesh
		//set values
	SamplerManager::Get()->GetSampler(SamplerType::PointClamp)->BindPS(0);


	//mNSE.mContext = tankContext;
	mLight.direction = mDepthMap.mLightViewCam.GetDirection();
	tankContext.light.direction = mLight.direction;
	quadContext.light.direction = mLight.direction;

	//
	mNSE.mContext.camera = tankContext.camera;
	mNSE.mContext.diffuse = tankContext.diffuse;
	mNSE.mContext.normal = tankContext.normal;
	mNSE.mContext.material = tankContext.material;
	mNSE.mContext.light = tankContext.light;
	mNSE.mContext.setting = tankContext.setting;

	auto world = mRenderObj.mTransform.GetTransformMatrix();
	//auto comp = world * mNSE.mContext.camera->GetViewMatrix() * mNSE.mContext.camera->GetPerspectiveMatrix();
	auto comp = world * mNSE.mContext.camera->GetViewMatrix() * mNSE.mContext.camera->GetPerspectiveMatrix();

	//auto lightWVP = H::Math::Transpose(H::Math::Translation(mDepthMap.mLightViewCam.GetPosition()) * mDepthMap.mLightViewCam.GetViewMatrix() * mDepthMap.mLightViewCam.GetPerspectiveMatrix());
	mNSE.mContext.shadow.WVPLight = H::Math::Transpose(world* mDepthMap.mLightViewCam.GetViewMatrix() * mDepthMap.mLightViewCam.GetPerspectiveMatrix());
	mNSE.mContext.transform.world = H::Math::Transpose(world);
	mNSE.mContext.transform.wvp = H::Math::Transpose(comp);
	mNSE.mContext.transform.viewPosition = mNSE.mContext.camera->GetPosition();
	//
	
	mDepthMap.GetRenderTarget()->BindPS(5);
	mNSE.Bind();
	mRenderObj.mMeshRenderer.mStandardContext.meshBuffer->Render();
 
	//set context for quad
	//mNSE.mContext = quadContext;
	mNSE.mContext.camera =  quadContext.camera;
	mNSE.mContext.diffuse = quadContext.diffuse;
	mNSE.mContext.normal =  quadContext.normal;
	mNSE.mContext.material = quadContext.material;
	mNSE.mContext.light = quadContext.light;
	mNSE.mContext.setting = quadContext.setting;


	world =(H::Math::RotationX(H::Math::Constants::Pi * 0.5f))* H::Math::Translation(Vector3{ 0.0f,-100.0f,0.0f });
	mNSE.mContext.shadow.WVPLight = H::Math::Transpose(world* mDepthMap.mLightViewCam.GetViewMatrix() * mDepthMap.mLightViewCam.GetPerspectiveMatrix());
	mNSE.mContext.transform.world = H::Math::Transpose(world);
	comp = world * mNSE.mContext.camera->GetViewMatrix() * mNSE.mContext.camera->GetPerspectiveMatrix();
	mNSE.mContext.transform.wvp = H::Math::Transpose(comp);
	mNSE.mContext.transform.viewPosition = mNSE.mContext.camera->GetPosition();

	mNSE.Bind();
	//mRenderObj.mMeshRenderer.mMeshBuffer.Render();

	mQuadBuf.Render();
	mNSE.UnBind();


	//old standard
	//mRenderObj.Render();
}

void TestShadow::DebugUI()
{
	ImGui::Begin("TestShadowTank");

	bool ulcchanged = false;
	if (ImGui::Checkbox("look from light cam##TestShadowTank", &useLightCam))
		ulcchanged = true;
	
	if (ulcchanged)
	{
		tankContext.camera = useLightCam ? lightCam : mainCam;
		quadContext.camera = tankContext.camera;

	}
 
	bool lightChanged = false;
	if (ImGui::DragFloat3("TankContext_lightDir##TestShadowTank", tankContext.light.direction.v.data(),0.1f,-1.0f,1.0f))
	{
		lightChanged = true;
	}
	if (lightChanged)
	{
		tankContext.light.direction = H::Math::Normalize(tankContext.light.direction);
	}

	ImGui::DragFloat("tankbrightness##TestShadowTank", &tankContext.setting.brightness);
	ImGui::DragFloat("tankdepthBias##TestShadowTank", &tankContext.setting.depthBias,0.0001f);
	ImGui::Checkbox("tankuseShadow##TestShadowTank", &tankContext.setting.useShadow);

	ImGui::DragFloat("quadbrightness##TestShadowTank", &quadContext.setting.brightness);
	ImGui::DragFloat("quaddepthBias##TestShadowTank", &quadContext.setting.depthBias, 0.0001f);
	ImGui::Checkbox("quaduseShadow##TestShadowTank", &quadContext.setting.useShadow);

	mRenderObj.DebugUI();
	mDepthMap.DebugUI();
	mNSE.DebugUI();

	ImGui::End();

	//ImGui::Begin("TestShadow");

}
