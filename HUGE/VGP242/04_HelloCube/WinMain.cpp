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
		//1
		{ {0.0f,0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.5f,0.0f,0.0f}, {1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f} },
												
		{ {0.0f,-0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {0.0f, 0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },

		//2
		{ {0.0f,-0.5f,-0.5f},{0.0f,0.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f}, {0.0f,0.0f,1.0f,1.0f} },
		{ {0.5f,0.0f,0.0f},  {0.0f,0.0f,1.0f,1.0f} },

		{ {0.5f,0.0f,-0.5f}, {1.0f,0.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{1.0f,0.0f,1.0f,1.0f} },
		{ {0.5f,0.0f,0.0f},  {1.0f,0.0f,1.0f,1.0f} },

		//3
		{ {-0.5f,0.0f,0.0f}, {0.0f,1.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f}, {0.0f,1.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{0.0f,1.0f,1.0f,1.0f} },

		{ {-0.5f,0.0f,0.0f}, {1.0f,1.0f,0.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },

		//4
		{ {-0.5f,0.0f,0.0f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },

		{ {0.0f,0.5f,0.0f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },

		//5
{ {0.5f,0.0f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
{ {0.0f,0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
{ {0.5f,0.0f,-0.5f},{0.0f,1.0f,0.0f,1.0f} },

{ {0.0f,0.5f,0.0f},{ 1.0f,1.0f,0.0f,1.0f} },
{ {0.0f,0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },
{ {0.5f,0.0f,-0.5f},{1.0f,1.0f,0.0f,1.0f} }




	};
	uint32_t indexBuf[] = { 1,2,0,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29 };
	//Mesh mesh = MeshBuilder::CreateCube();

	MeshBuffer mb;
	mb.Initialize(vertices,std::size(vertices),sizeof(Vertex),indexBuf,std::size(indexBuf));
	VertexShader vs;
	vs.Initialize();
	PixelShader ps;
	ps.Initialize();
	ConstantBuffer cs;
	cs.Initialize();

	//=============
	//constant buffer ->(compute shader) -> vertex
	//ID3D11Buffer* constantBuffer = nullptr;
	//D3D11_BUFFER_DESC cdesc = {};
	//cdesc.ByteWidth = 3 * sizeof(Matrix4);
	//cdesc.Usage = D3D11_USAGE_DEFAULT;
	//cdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//GraphicSystem::Get()->GetDevice()->CreateBuffer(&cdesc, nullptr, &constantBuffer);

	//cam
	Camera cam;
	cam.SetPosition({ 0.0f,-3.0f,-3.0f });
	cam.SetDirection({ 0.0f,1.0f,1.0f });
	//GraphicSystem::Get()->GetContext()->VSGetConstantBuffers(0, 1, &constantBuffer);

	// constan buf take mat transpose
	Matrix4 mats[3];
	//mats[0] = mats[0].RotationY(5.0f);
	////mats[0].Translation({ 0.5f,0.0f,0.0f });
	//
	//mats[1] = cam.GetViewMatrix();
	//mats[2] = cam.GetPerspectiveMatrix((float)(1280.0f / 720.0f));

	//mats[0] = mats[0].Transpose();
	//mats[1] = mats[1].Transpose();

	//mats[2] = mats[2].Transpose();

	//auto context = GraphicSystem::Get()->GetContext();



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
		//=============

		//context->UpdateSubresource(constantBuffer, 0, nullptr, mats, 0, 0);//download data to VRAM
		//context->VSSetConstantBuffers(0, 1, &constantBuffer);

		if (GetAsyncKeyState(VK_NUMPAD9)) // up
		{
			cam.Rise(0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD3)) // down
		{
			cam.Rise(-0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD8)) // forward
		{
			cam.Walk(0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD2)) // backward
		{
			cam.Walk(-0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD4)) // 
		{
			cam.Strafe(0.1f);
		}
		if (GetAsyncKeyState(VK_NUMPAD6)) // 
		{
			cam.Strafe(-0.1f);
		}


		mats[0] = mats[0].RotationX(45.0f);
		//mats[0].Translation({ 0.5f,0.0f,0.0f });

		mats[1] = cam.GetViewMatrix();
		mats[2] = cam.GetPerspectiveMatrix((float)(1280.0f / 720.0f));

		mats[0] = mats[0].Transpose();
		mats[1] = mats[1].Transpose();

		mats[2] = mats[2].Transpose();


		//=============
		cs.BindVS(mats);
		vs.Bind();
		ps.Bind();
		mb.Render();
		
		//context->Draw(std::size(vertices), 0);
		GraphicSystem::Get()->EndRender();//flip buffer
	}

	//delete
	vs.Terminate();
	ps.Terminate();
	mb.Terminate();
	cs.Terminate();

	//SafeRelease(constantBuffer);

	GraphicSystem::StaticTerminate();
	myWindow.Terminate();

	return 0;
}