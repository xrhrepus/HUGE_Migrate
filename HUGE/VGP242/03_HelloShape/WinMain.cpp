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

	//vertices
	float k169 = 1.778f;
	//draw heart
	Vertex v0 = { {0.0f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v1 = { {0.0f,0.15f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v2 = { {0.05f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v3 = { {0.1f,0.23f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v4 = { {0.2f,0.15f,0.0f},1.0f,0.0f,0.0f,1.0f };

	Vertex v5 = { {0.22f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v6 = { {0.0f,-0.3f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v7= { {-0.22f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v8 = { {-0.05f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v9 = { {-0.2f,0.15f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v10 = { {-0.1f,0.23f,0.0f},1.0f,0.0f,0.0f,1.0f };
	//draw triforce
	Vertex v00 = { {-0.2f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v01 = { {0.0f,0.2f*k169,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v02 = { {0.2f,0.0f,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v03 = { {0.0f,-0.2f*k169,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v04 = { {0.4f,-0.2f*k169,0.0f},1.0f,0.0f,0.0f,1.0f };
	Vertex v05 = { {-0.4f,-0.2f*k169,0.0f},1.0f,0.0f,0.0f,1.0f };
	//
	Vertex v20 = { { 0.2f,0.2f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v21 = { { 0.22f,0.07f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v22 = { { 0.25f,0.15f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v23 = { { 0.25f,-0.15f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v24 = { { 0.15f,-0.2f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v25 = { { 0.11f,-0.35f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v26 = { { 0.0f,-0.35f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v27 = { { 0.1f,-0.27f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v28 = { { -0.2f,-0.15f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v29 = { { -0.23f,-0.25f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v210 = { { -0.27f,-0.25f,0.0f },1.0f,0.0f,0.0f,1.0f };

	Vertex v211 = { { -0.16f,0.07f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v212 = { { -0.1f,0.0f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v213 = { { -0.05f,0.27f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v214 = { { 0.01f,0.2f,0.0f },1.0f,0.0f,0.0f,1.0f };

	Vertex v215 = { { -0.07f,0.1f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v216 = { { -0.05f,0.15f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v217 = { { 0.4f,-0.57f,0.0f },1.0f,0.0f,0.0f,1.0f };
	Vertex v218 = { { 0.46f,-0.5f,0.0f },1.0f,0.0f,0.0f,1.0f };


	const Vertex vertices[] =
	{
		//draw from 0 ~ 36
		//draw heart 36,0
		v0,v1,v2,
		v1,v3,v2,
		v3,v4,v2,
		v4,v5,v2,
		v2,v5,v6,
		v0,v2,v6,

		v7,v8,v6,
		v8,v0,v6,
		v9,v8,v7,
		v9,v10,v8,
		v10,v1,v8,
		v1,v0,v8,
		//draw triforce 9,36
		//draw from 36 ~ 45
		v00,v01,v02,
		v03,v02,v04,
		v05,v00,v03,
		//draw triforce
		//draw from 36 ~ 45

		//33,45
		//45-
		v20,v22,v21,
		v21,v22,v23,
		v21,v23,v24,
		v24,v23,v25,
		v26,v24,v25,
		v28,v27,v26,
		v29,v210,v28,
		//
		v211,v213,v212,
		v213,v214,v212,
		//
		v215,v216,v217,
		v217,v216,v218
	};
	UINT sz = 36;
	UINT startIndex = 0;


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
	
		if (GetAsyncKeyState(VK_NUMPAD1))//heart
		{
			sz = 36;
			startIndex = 0;
		}
		else if (GetAsyncKeyState(VK_NUMPAD2))//triforce
		{
			sz = 9;
			startIndex = 36;

		}
		else if (GetAsyncKeyState(VK_NUMPAD3))//
		{
			sz = 33;
			startIndex = 45;

		}

		auto context = GraphicSystem::Get()->GetContext();
		vs.Bind();
		ps.Bind();
		mb.Render();
		
		context->Draw(sz, startIndex);
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