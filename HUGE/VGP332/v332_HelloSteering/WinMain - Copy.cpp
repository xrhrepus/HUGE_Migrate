//#include <Core/Inc/Core.h>
#include <HUGE\Inc\HUGE.h>

using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
using namespace H::AI;

void ShowDebugUi(TileMap& tilemap, float deltatime);
InputSystem* inputSys;
TextureId icon;
//[Steering]==================================================
class SeekBehavior : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(Agent& agent) override
	{
		auto desierdV = H::Math::Normalize (agent.destination - agent.position) * agent.maxSpeed;
		return desierdV - agent.velocity;
	  }

private:

};

 

struct Dude : public H::AI::Agent
{
	void Init()
	{
		mTextureId = TextureManager::Get()->LoadTexture(L"sheep64.png");

		maxSpeed = 300.0f;
		mSteeringModule = std::make_unique<H::AI::SteeringModule>(*this);

		mSteeringModule->AddBehavior<SeekBehavior>("Seek")->SetActive(true);
	}

	void Update(float deltatime)
	{
		auto force = mSteeringModule->Calculate();
		auto a = force / mass;
		velocity += a * deltatime;

		position += velocity * deltatime;
		if (H::Math::MagnitudeSqr(velocity) > 1.0f)
		{
			heading = H::Math::Normalize(velocity);
		}

	}
	void Render()
	{
		
		H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), position);
		//H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), {200.0f,200.0f});

	}

	H::Graphics::TextureId mTextureId;
	std::unique_ptr<H::AI::SteeringModule> mSteeringModule;

};

	//[Steering]==================================================



int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	//[set up app window] ==========================
	Window myWindow;
	myWindow.Initialize(hinstance, "Hell0 pathfinding", 1280, 720);

	//[inputsystem] ==========================
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	inputSys = InputSystem::Get();

	//[graphic system] init  ==========================
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	//[simple draw] ==========================
	SimpleDraw::StaticInitialize(1000000);

	//[IMGui], last init first catch win msg ==========================
	DebugUI::Initialize(myWindow.GetWindowHandle(), true, false);

	//[Texture manager] ==========================
	TextureManager::StaticInitialize(L"../Assets/Images/");
	//TextureManager::Get()->SetRootPath(L"../Assets/Images/");
	 //imgButton = TextureManager::Get()->LoadTexture(L"stone32.png");
	icon = TextureManager::Get()->LoadTexture(L"fruit42.png");

	//[spriterenderer] ==========================
	SpriteRenderer::StaticInitialize();

	//[camera] ==========================
	Camera cam;
	cam.SetPosition({ 0.0f,0.0f, -10.0f });
	cam.SetDirection({ 0.0f,0.0f, 1.0f });
	
	//[Steering]==================================================

	Dude dud;
	dud.Init();
	//[loop]
	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		//run logic ==========================
		if (inputSys->IsKeyPressed(KeyCode::ESCAPE)){done = true; 	}
		//time
		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		lastTime = currTime;

		//begin render ==========================
		DebugUI::BeginRender();
		SpriteRenderer::Get()->BeginRender();
		GraphicSystem::Get()->BeginRender();

		//=======================================
	//[Steering]==================================================
		dud.destination.x = static_cast<float>( inputSys->GetMouseScreenX());
		dud.destination.y = static_cast<float>(inputSys->GetMouseScreenY());



		dud.Update(deltaTime);
		
		dud.Render();

	//[Steering]==================================================


		ImGui::ShowDemoWindow();

		//=======================================

		//end render ==========================
		SpriteRenderer::Get()->EndRender();
		SimpleDraw::Render(cam);
		DebugUI::EndRender();

		GraphicSystem::Get()->EndRender();//flip buffer
		inputSys->Update();
	}

	//terminate ==========================
	GraphicSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	DebugUI::Terminate();
	SimpleDraw::StaticTerminate();
	myWindow.Terminate();

	return 0;

}

