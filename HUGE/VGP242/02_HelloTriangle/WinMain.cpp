#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/HUGEMath.h>

using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	//set up app window
	Window myWindow;

	myWindow.Initialize(hinstance, "Hell0 Triangle", 1280, 720);

	//init graphic system
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	//NOTE:0514
	//winding order , back facing culling
	//define vertex data
	const Vertex vertices[] =
	{
		{ {0.0f,0.5f,0.0f},1.0f,0.0f,0.0f,1.0f },
		{ {0.5f,0.0f,0.0f},0.0f,1.0f,0.0f,1.0f },
		{ {0.0f,-0.5f,0.0f},0.0f,0.0f,1.0f,1.0f }
	};

	MeshBuffer mb;
//	mb.Initialize(vertices,std::size(vertices));
	VertexShader vs;
	vs.Initialize();
	PixelShader ps;
	ps.Initialize();
		
	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		//run logic
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		GraphicSystem::Get()->BeginRender();
		//draw
		
		auto context = GraphicSystem::Get()->GetContext();
		vs.Bind();
		ps.Bind();
//		mb.Bind();
		
		context->Draw(std::size(vertices), 0);
		GraphicSystem::Get()->EndRender();//flip buffer
	}

	//delete
	vs.Terminate();
	ps.Terminate();
	mb.Terminate();


	GraphicSystem::StaticTerminate();
	myWindow.Terminate();

	return 0;
}