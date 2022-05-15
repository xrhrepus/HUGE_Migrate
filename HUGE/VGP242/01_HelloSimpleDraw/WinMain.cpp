#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/HUGEMath.h>
#include <Input/Inc/Input.h>

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

	myWindow.Initialize(hinstance, "Hell0 simpledraw", WINDOW_WIDTH, WINDOW_HEIGHT);

	//
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());


	//init graphic system
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);
	//simple draw
	SimpleDraw::StaticInitialize(100000);

	Camera cam;
	cam.SetPosition({ 0.0f,0.0f, -30.0f });
	cam.SetDirection({ 0.0f,0.0f, 1.0f });

	AABB testAABB;
	testAABB.mExtend = { 11.0f,5.5f, 0.5f };
	Sphere testSphere;
	testSphere.mRadius = 6.0f;
	H::Math::Rect testRect;
	testRect.mCentre = { 600.0f,600.0f };
	testRect.mExtend = { 130.0f,50.0f };
	Circle testCircle;
	testCircle.centre = { 800.0f, 500.0f };
	testCircle.radius = 40.0f;

	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();

		//input sys
		auto inputSys = InputSystem::Get();
		inputSys->Update();

		if (inputSys->IsKeyPressed(KeyCode::ESCAPE)){done = true;}

		//time
		static auto lastTime = std::chrono::high_resolution_clock::now();

		auto currTime = std::chrono::high_resolution_clock::now();

		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		//
		lastTime = currTime;

		//run logic
		GraphicSystem::Get()->BeginRender();
		cam.UpdateControl(deltaTime);

		SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Magenta);//x
		SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::Aqua);//y
		SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::LawnGreen);//z

		SimpleDraw::AddAABB(testAABB, Colors::GreenYellow);
		SimpleDraw::AddShpere(testSphere, Colors::Cyan,8);

		//2d
		SimpleDraw::AddScreenLine({ 0.0f }, { 300.0f,200.0f }, Colors::Purple);
		SimpleDraw::AddScreenRect(testRect, Colors::RoyalBlue);
		SimpleDraw::AddScreenCircle(testCircle, Colors::Beige,8);
		SimpleDraw::Render(cam);

		GraphicSystem::Get()->EndRender();//flip buffer
	}

	GraphicSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	myWindow.Terminate();

	return 0;
}