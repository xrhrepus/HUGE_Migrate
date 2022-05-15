#include "Precompiled.h"
//#include "HUGE.h"
#include "App.h"
#include "AppState.h"
#include "MetaRegistration.h"

using namespace H;
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;


void App::ChangeState(std::string name)
{
	if (auto it = mAppStates.find(name); it != mAppStates.end())
	{
		mNextState = it->second.get();
	}
}

void  App::Run(AppConfig appConfig)
{
	LOG("App starts...");
	//init
	//run
	//shut
	H::Core::StaticMetaRegistration();
	H::Math::StaticMetaRegistration();
	H::StaticMetaRegistration();
	
	mAppConfig = std::move(appConfig);

	//init -------
	LOG("Create Window...");

	mWindow.Initialize(GetModuleHandle(NULL),
		(mAppConfig.appName.c_str()),
		mAppConfig.windowWidth,
		mAppConfig.windowHeight,
		mAppConfig.maximize);
	//
	InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	//[GraphicSystem]
	GraphicSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	

	//[SimpleDraw]
	SimpleDraw::StaticInitialize(500000);
	//[Texture manager] ==========================
	TextureManager::StaticInitialize(mAppConfig.assetDirectory / "Images");
	//TextureManager::Get()->SetRootPath(L"../Assets/Images/");

	//
	SamplerManager::StaticInitialize();
	BlendStateManager::StaticInitialize();
	RasterizerStateManager::StaticInitialize();
	ShaderEffectManager::StaticInitialize();
	DepthStencilManager::StaticInitialize();
	//[SpriteRenderer]
	SpriteRenderer::StaticInitialize();

	//[IMGui], last init first catch win msg ==========================
	DebugUI::Initialize(mWindow.GetWindowHandle(), mAppConfig.imguiDocking, true);

	//
	mCurrentState->Initialize();
	//=======================================================================
	bool done = false;
	while (!done)
	{
		done = mWindow.ProcessMessage();

		if (done)
		{
			break;
		}

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			//mCurrentState = mNextState;
			//mNextState = nullptr;
			mCurrentState->Initialize();
		}

		//input sys ===================================================
		auto inputSys = InputSystem::Get();
		inputSys->Update();
		if (inputSys->IsKeyPressed(KeyCode::ESCAPE)) { done = true; }

		//time ===================================================
		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		lastTime = currTime;
		//

		mCurrentState->Update(deltaTime);

		GraphicSystem::Get()->BeginRender();
		SpriteRenderer::Get()->BeginRender();
		mCurrentState->Render();
		SpriteRenderer::Get()->EndRender();

		GraphicSystem::Get()->ClearDepthBuffer();
		SimpleDraw::Render2D();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();
		GraphicSystem::Get()->EndRender();
	}

	//clean up current state
	mCurrentState->Terminate();

	InputSystem::StaticTerminate();
	TextureManager::StaticTerminate();
	SamplerManager::StaticTerminate();
	BlendStateManager::StaticTerminate();
	RasterizerStateManager::StaticTerminate();
	ShaderEffectManager::StaticTerminate();
	DepthStencilManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	DebugUI::Terminate();
	SimpleDraw::StaticTerminate();

	GraphicSystem::StaticTerminate();
	mWindow.Terminate();



}
