#include "MyAppState.h"
#include "Graphics\Src\MeshIO.h"
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
using namespace H::Physics;

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

	void AddCloth(std::vector<Particle*>& ps, Physics::PhysicsWorld& pWorld, size_t xBlocks,size_t yBlocks, const Vector3& from, const Vector3& to)
	{
		ps.resize(xBlocks * yBlocks);
 		size_t idxCount = 0;
 
		//top fixed
		for (size_t i = 0; i < xBlocks; ++i)
		{
			float f = (float)i / (float)xBlocks;
			Particle*& p = ps[idxCount];
			p = new Particle();
			p->SetPosition(H::Math::Lerp(from, to, f));
			p->SetVelocity({0.0f});
			p->radius = 0.5f;
			p->invMass = 1.0f;
			pWorld.AddParticle(p);
			H::Physics::Constraint* c = new Fixed(p, p->position);
			pWorld.AddConstraint(c);
			idxCount++;
 		}
		float xDist = H::Math::Distance(ps[1]->position, ps[0]->position);
 
		//
		for (size_t j = 0; j < yBlocks - 1; j++)
		{
			for (size_t i = 0; i < xBlocks; ++i)
			{
				Vector3 vel = H::Math::RandomUnitSphere();
				vel.y = abs(vel.y);

				float f = (float)i / (float)xBlocks;
				Particle*& p = ps[idxCount];
				p = new Particle();
				const auto& rp = ps[idxCount % xBlocks]->position;
				p->SetPosition({ rp.x ,rp.y ,rp.z });
				p->SetVelocity(vel);
				p->radius = 0.5f;
				p->invMass = 1.0f;
				pWorld.AddParticle(p);
				if (0 != idxCount % xBlocks)//add constraint to left
				{
					H::Physics::Constraint* c = new Spring(ps[idxCount], ps[idxCount - 1], xDist);
					pWorld.AddConstraint(c);
				}
				//add constraint upward
				H::Physics::Constraint* c = new Spring(ps[idxCount], ps[idxCount - xBlocks], xDist);
				pWorld.AddConstraint(c);

				idxCount++;

			}
		}
 
	}

 
}



  void MyAppState::Initialize()
{
	  srand(time(0));
	//camera
	mCamera.SetPosition({ 20.0f, 20.0f, 0.0f });
	mCamera.SetDirection(H::Math::Normalize(Vector3{ 0.0f,0.0f, 0.0f }-Vector3{ 20.0f, 20.0f, 0.0f }));

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
	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);
	mPPnothingVS.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPPnothingPS.Initialize(POSTPROCESSING_FILE_PATH, "PS");
	mPPRendertarget.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	//physics
	mPhysicsWorld.Initialize({});
	mPhysicsWorld.AddPlane(mPlaneYM);
	mPhysicsWorld.AddPlane(mPlaneYP);
	mPhysicsWorld.AddPlane(mPlaneXM);
	mPhysicsWorld.AddPlane(mPlaneXP);
	mPhysicsWorld.AddPlane(mPlaneZM);
	mPhysicsWorld.AddPlane(mPlaneZP);
	//AddCloth(ps, mPhysicsWorld, 3, 5, { 10.0,10.0,0.0f }, { 40.0,10.0,0.0f });

	
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
 	mCamera.UpdateControl(deltatime);

	mPhysicsWorld.Update(deltatime);
   }

void MyAppState::DebugUI()
{
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

	ImGui::Begin("physics");
	if (ImGui::Button("no constraint ---"))
	{
		mPhysicsWorld.ClearDynamic();

		for (size_t i = 0; i < 25; i++)
		{
			//Vector3 vel = H::Math::Normalize({ (float)(rand() % 2 + 1), (float)(rand() % -1 + 2), (float)(rand() % -1 +2) });
			Vector3 vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);

			auto p = new Particle();
			p->SetPosition({ 0.0f });
			p->SetVelocity(vel);
			p->radius = 0.5f;
			p->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p);

			vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			auto p1 = new Particle();
			p1->SetPosition({ 0.0f });
			p1->SetVelocity(vel);
			p1->radius = 0.5f;
			p1->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p1);
 
		}
	}

	if (ImGui::Button("spring---"))
	{
		mPhysicsWorld.ClearDynamic();

		for (size_t i = 0; i < 25; i++)
		{
			//Vector3 vel = H::Math::Normalize({ (float)(rand() % 2 + 1), (float)(rand() % -1 + 2), (float)(rand() % -1 +2) });
			Vector3 vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			 
				auto p = new Particle();
				p->SetPosition({ 0.0f });
				p->SetVelocity(vel);
				p->radius = 0.5f;
				p->invMass = 1.0f;
				mPhysicsWorld.AddParticle(p);
			 
			vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
 				auto p1 = new Particle();
				p1->SetPosition({ 0.0f });
				p1->SetVelocity(vel);
				p1->radius = 0.5f;
				p1->invMass = 1.0f;
				mPhysicsWorld.AddParticle(p1);
 			H::Physics::Constraint* c = new Spring(p,p1,50.0f);
			mPhysicsWorld.AddConstraint(c);

		}
  	}
	if (ImGui::Button("pyrimid---"))
	{
		mPhysicsWorld.ClearDynamic();

		for (size_t i = 0; i < 25; i++)
		{
			//top 0
			Vector3 vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);

			auto p0 = new Particle();
			p0->SetPosition({ 0.0f });
			p0->SetVelocity(vel);
			p0->radius = 0.5f;
			p0->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p0);

			//1
			vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			auto p1 = new Particle();
			p1->SetPosition({ 0.0f });
			p1->SetVelocity(vel);
			p1->radius = 0.5f;
			p1->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p1);
			H::Physics::Constraint* c0 = new Spring(p0, p1, 5.0f);
			mPhysicsWorld.AddConstraint(c0);
			//2
			vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			auto p2 = new Particle();
			p2->SetPosition({ 0.0f });
			p2->SetVelocity(vel);
			p2->radius = 0.5f;
			p2->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p2);
			H::Physics::Constraint* c1 = new Spring(p0, p2, 5.0f);
			mPhysicsWorld.AddConstraint(c1);
			//3
			vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			auto p3 = new Particle();
			p3->SetPosition({ 0.0f });
			p3->SetVelocity(vel);
			p3->radius = 0.5f;
			p3->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p3);
			H::Physics::Constraint* c2 = new Spring(p0, p3, 5.0f);
			mPhysicsWorld.AddConstraint(c2);

			//add constraint for bot 3
			H::Physics::Constraint* cb0 = new Spring(p1, p3, 5.0f);
			mPhysicsWorld.AddConstraint(cb0);
			H::Physics::Constraint* cb1 = new Spring(p1, p2, 5.0f);
			mPhysicsWorld.AddConstraint(cb1);
			H::Physics::Constraint* cb2 = new Spring(p2, p3, 5.0f);
			mPhysicsWorld.AddConstraint(cb2);


		}
	}
	if (ImGui::Button("cube---"))
	{
		mPhysicsWorld.ClearDynamic();

		for (size_t i = 0; i < 1; i++)
		{
			//all 8 pts
 
			Vector3 vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
			const size_t num_p = 8;
			Particle* p[num_p] = { nullptr };

			for (size_t i = 0; i < num_p; i++)
			{
				vel = H::Math::RandomUnitSphere();
				vel.y = abs(vel.y);
				p[i] = new Particle();
				Particle& cp = *(p[i]);
 				cp.SetPosition({ 0.0f });
				cp.SetVelocity(vel);
				cp.radius = 0.5f;
				cp.invMass = 1.0f;
				mPhysicsWorld.AddParticle(&cp);

 			}
			//verticle
			for (size_t i = 0; i < 4; i++)
			{
				H::Physics::Constraint* c0 = new Spring(p[i], p[i + 4], 5.0f);
 				mPhysicsWorld.AddConstraint(c0);
			}
			//topface
			{
				H::Physics::Constraint* c0 = new Spring(p[0], p[1], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[0], p[2], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[3], p[1], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[3], p[2], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}
			// bot face
			{
				H::Physics::Constraint* c0 = new Spring(p[4], p[5], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[4], p[6], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[7], p[5], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[7], p[6], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}
			//diagno cross
			{
				H::Physics::Constraint* c0 = new Spring(p[0], p[5], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[0], p[6], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[0], p[7], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[0], p[3], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}

			{
				H::Physics::Constraint* c0 = new Spring(p[4], p[1], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[4], p[2], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[4], p[7], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[4], p[3], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}

			{
				H::Physics::Constraint* c0 = new Spring(p[2], p[1], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[2], p[5], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[2], p[7], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[2], p[4], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}
			{
				H::Physics::Constraint* c0 = new Spring(p[6], p[1], 5.0f);
				mPhysicsWorld.AddConstraint(c0);
				H::Physics::Constraint* c1 = new Spring(p[6], p[5], 5.0f);
				mPhysicsWorld.AddConstraint(c1);
				H::Physics::Constraint* c2 = new Spring(p[6], p[0], 5.0f);
				mPhysicsWorld.AddConstraint(c2);
				H::Physics::Constraint* c3 = new Spring(p[6], p[3], 5.0f);
				mPhysicsWorld.AddConstraint(c3);
			}

 		}
	}

	if (ImGui::Button("fixed---"))
	{
		mPhysicsWorld.ClearDynamic();
		auto p = new Particle();
		p->SetPosition({ 0.0f });
		p->SetVelocity(0.0f);
		p->radius = 0.5f;
		p->invMass = 1.0f;
		mPhysicsWorld.AddParticle(p);
		H::Physics::Constraint* c = new Fixed(p, { 0.0f,20.0f,0.0f });
		mPhysicsWorld.AddConstraint(c);

		//for (size_t i = 0; i < 25; i++)
		{
 			Vector3 vel = H::Math::RandomUnitSphere();
			vel.y = abs(vel.y);
 
			auto p1 = new Particle();
			p1->SetPosition({ 0.0f });
			p1->SetVelocity(vel);
			p1->radius = 0.5f;
			p1->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p1);

 			H::Physics::Constraint* c2 = new Spring(p, p1, 5.0f);
			mPhysicsWorld.AddConstraint(c2);
			//
			vel.y = abs(vel.y);

			auto p2 = new Particle();
			p2->SetPosition({ 0.0f });
			p2->SetVelocity(vel);
			p2->radius = 0.5f;
			p2->invMass = 1.0f;
			mPhysicsWorld.AddParticle(p2);

			H::Physics::Constraint* c3 = new Spring(p2, p1, 5.0f);
			mPhysicsWorld.AddConstraint(c3);

		}
	}
	if (ImGui::Button("cloth---"))
	{
		mPhysicsWorld.ClearDynamic();
		AddCloth(ps, mPhysicsWorld, 15, 15, { 10.0,40.0,0.0f }, { 40.0,40.0,0.0f });
	}

	ImGui::End();



 	CamDebugUI(mCamera);
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
	Matrix4 world = (mWorldMat);
	Matrix4 comp = world * vm * pm;

	TransformData tf;
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Set(tf);
	mTransformBuffer.BindVS(0);// b0 slot
	mTransformBuffer.BindPS(0);


	//bind===
	mMeshpnVS.Bind();
	mMeshpnPS.Bind();

	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
 	for (const auto& mpn : mMeshpnBufs)
	{
		mpn.Render();
	}
 
 
  
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

 	mPhysicsWorld.DebugDraw();

	SimpleDraw::Render(mCamera);

}
