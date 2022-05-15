#include <HUGE\Inc\HUGE.h>

using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	//set up app window
	Window myWindow;
	myWindow.Initialize(hinstance, "Hell0 IMGUI", WINDOW_WIDTH, WINDOW_HEIGHT);

	//
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());

	//init graphic system
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	//simple draw
	SimpleDraw::StaticInitialize(100000);

	//Texture manager
	TextureManager::StaticInitialize();
	TextureManager::Get()->SetRootPath(L"../Assets/Images/");

	//spriterenderer
	SpriteRenderer::StaticInitialize();

	//IMGui, last init first catch win msg
	DebugUI::Initialize(myWindow.GetWindowHandle(),true,false);


	Camera cam;
	cam.SetPosition({ 0.0f,0.0f, -10.0f });
	cam.SetDirection({ 0.0f,0.0f, 1.0f });

	//=============
	//Texture testTexture;
	//testTexture.Initialize(L"../Assets/Images/sponge_bob.png");
	float tx = 0.0f;
	float ty = 0.0f;

	//std::wstring spongeBob_path = L"../Assets/Images/sponge_bob.png";
	TextureId spongeBob = TextureManager::Get()->LoadTexture(L"sponge_bob.png");
	//=============


	bool done = false;
	while (!done)
	{
		//window msg
		done = myWindow.ProcessMessage();

		//input sys
		auto inputSys = InputSystem::Get();
		inputSys->Update();
		if (inputSys->IsKeyPressed(KeyCode::ESCAPE)){done = true;}

		//time
		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		lastTime = currTime;
		//

		//run logic
		GraphicSystem::Get()->BeginRender();
		cam.UpdateControl(deltaTime);

		SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
		SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::Aqua);//y
		SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::LawnGreen);//z
		//sprite
		SpriteRenderer::Get()->BeginRender();
		SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(spongeBob), { tx,ty });
		SpriteRenderer::Get()->EndRender();
		SimpleDraw::Render(cam);


		//

		//imgui
		DebugUI::BeginRender();
		//loop
		//ImGui::ShowDemoWindow();
		//ImGui::SliderFloat2("circle centre: ", testCircle.mCentre);
		ImGui::SliderFloat("x:", &tx,0.0f,500.0f);
		ImGui::SliderFloat("y:", &ty, 0.0f, 500.0f);

		DebugUI::EndRender();

		GraphicSystem::Get()->EndRender();//flip buffer
	}

	GraphicSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	TextureManager::StaticTerminate();

	DebugUI::Terminate();
	myWindow.Terminate();

	return 0;
}