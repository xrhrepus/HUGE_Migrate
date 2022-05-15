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
	const uint32_t w = GraphicSystem::Get()->GetBackBufferWidth();
	const uint32_t h = GraphicSystem::Get()->GetBackBufferHeight();

	matInvScreen = Matrix4
	(
		2.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	);

	//camera
	mCamera.SetPosition({ -121.0f, 75.0f, 100.0f });
	//mCurrentCam.SetDirection({ 0.0f,0.0f, 1.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 121.0f, -75.0f, -100.0f }));
 
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
	mQuad = MeshBuilder::CreateQuad(100.0f, 100.0f, { 0.0f });
	mQuadBuffer.Initialize(mQuad);
	//mQuadDiffuse = TextureManager::Get()->LoadTexture("fruit42_x10.png");

	//
	mCube = MeshBuilder::CreateCube(20.0f, 20.0f, 20.0f, { 0.0f });
	mCubeBuffer.Initialize(mCube);

	mModel.Initialize(Model_FILE_PATH);

	mTerr = MeshBuilder::CreateTerrain(15, 15,1.0f);
	mTerrBuffer.Initialize(mTerr);

	//
	mTerrCtx.diffuse = mQuadDiffuse;
	mTerrCtx.normal = mNormalMap;
	mTerrCtx.material = mMaterial;
	mTerrCtx.camera = &mCamera;
	mTerrCtx.directionalLight = mLight;
	
	//
 	mCurrentCam = &mCamera;

	//
	mTestTerr.Initialize(15, 15, 10.0f);
	mTestRaycast.Init();
	mTPlane.Init();
 }

void MyAppState::Terminate()
{
 	mSphereBUffer.Terminate();
	mQuadBuffer.Terminate();
	mCubeBuffer.Terminate();
	mTerrBuffer.Terminate();

	mTestTerr.Terminate();
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
	mTestTerr.DrawEditorUI();
	mTestRaycast.DebugUI();
	ImGui::Begin("raycastTest");
	auto inputSys = Input::InputSystem::Get();
	Vector2 mousep(inputSys->GetMouseScreenX(), inputSys->GetMouseScreenY());

	ImGui::Text("Mouse pos: [ %f,%f ]", mousep.x, mousep.y);

 
	if (ImGui::IsMouseDown(0))
	{
		Matrix4 viewmat = mCamera.GetViewMatrix();

		Matrix4 perspectmat = mCamera.GetPerspectiveMatrix();

		Matrix4 invMat = H::Math::Inverse(viewmat * perspectmat);
		Ray r = mTestRaycast.CastRay(mousep.x, mousep.y, 3000.0f, mCamera.GetPosition(), invMat);

		//Matrix4 invMat = H::Math::Inverse(perspectmat * viewmat);

		//Vector3 testR(60.0f, 60.0f, 60.0f);
		//Ray r;
		//r.dir = H::Math::Normalize(Vector3{ 0.1,-0.8,0.1f });
		//r.org = testR;
		//SimpleDraw::AddBox({ testR , 5.0f }, Colors::Cyan);

		SimpleDraw::AddScreenRect(Rect{ {mousep.x,mousep.y},5.0f }, Colors::LawnGreen);
		mTPlane.CheckIntersec(r);
		static float dist_to_terrain = 0.0f;
		if (mTestTerr.Intersect(r,dist_to_terrain))
		{
			Vector3 hitPos = mCurrentCam->GetPosition() + r.dir * dist_to_terrain;
			//SimpleDraw::AddLine(testR, hitPos, Colors::Pink);
			SimpleDraw::AddBox({ hitPos , 5.0f}, Colors::Pink);

			
		}
		//mTPlane.CheckIntersec(Ray{ {0.0f,10.0f,10.0f},{0.0f,-1.0f,0.0f} });

	}
	ImGui::End();
	mTPlane.DebugUI();
	ImGui::Begin("StandardEffectContext");
	if (ImGui::CollapsingHeader("Light##StandardEffectContext"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light##StandardEffectContext", &mTerrCtx.directionalLight.direction.x, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light##StandardEffectContext", &mTerrCtx.directionalLight.direction.y, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light##StandardEffectContext", &mTerrCtx.directionalLight.direction.z, 0.25f, -1.0f, 1.0f);

		if (directionChanged)
		{
			mTerrCtx.directionalLight.direction = Math::Normalize(mTerrCtx.directionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light##StandardEffectContext", &mTerrCtx.directionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light##StandardEffectContext", &mTerrCtx.directionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light##StandardEffectContext",&mTerrCtx.directionalLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material##StandardEffectContext"))
	{
		ImGui::ColorEdit4("Ambient##Material##StandardEffectContext", &mTerrCtx.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material##StandardEffectContext", &mTerrCtx.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material##StandardEffectContext", &mTerrCtx.material.specular.r);
		ImGui::DragFloat("Power##Material##StandardEffectContext", &mTerrCtx.material.power, 0.1f, 0.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings##StandardEffectContext"))
	{
 		ImGui::DragFloat("brightness##Settings##StandardEffectContext", &mTerrCtx.settings.brightness, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("normalMapWeight##Settings##StandardEffectContext", &mTerrCtx.settings.normalMapWeight, 0.1f, 0.0f, 1.0f);
	}

	ImGui::End();
 
	mCurrentCam->DebugUI();

 
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
	auto stdEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
	SamplerManager::Get()->GetSampler(SamplerType::PointWrap)->BindPS();
	BlendStateManager::Get()->GetBlendState(Blend::AlphaBlend)->Set();
	TransformData tfd;
	Matrix4 vm = mCurrentCam->GetViewMatrix();
	Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	Matrix4 world = Matrix4::Identity();
	tfd.world = H::Math::Transpose(world);
	tfd.viewPosition = mCurrentCam->GetPosition();
	tfd.wvp = H::Math::Transpose(world * vm * pm);
	mTerrCtx.transformData = tfd;
 
	stdEffect->SetContextInfo(&mTerrCtx);
	stdEffect->Bind();

	//mCubeBuffer.Render();
	//mTerrBuffer.Render();

	stdEffect->UnBind();

	mTestTerr.Render(*mCurrentCam);
 //
	mTPlane.Render();
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

