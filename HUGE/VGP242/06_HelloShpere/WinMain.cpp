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

	myWindow.Initialize(hinstance, "Hell0 cube 2", 1280, 720);

	//init graphic system
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);


//	MeshPC mesh = MeshBuilder::CreateCube();
	//MeshPC mesh = MeshBuilder::CreateQuad(1, {0.0f,0.0f,0.0f},Colors::BlueViolet);
	MeshPC mesh = MeshBuilder::CreateCircle(2,2, { 0.0f,0.0f,0.0f }, Colors::BlueViolet);
	MeshPC mesh4 = MeshBuilder::CreateCylinder(4,2, 2, { 0.0f,0.0f,0.0f }, Colors::DodgerBlue);
	MeshPC mesh5 = MeshBuilder::CreateShperePC(2,3,{ 0.0f,0.0f,0.0f }, Colors::Olive);
	//MeshPC mesh = MeshBuilder::CreateCone(4, 2, 2, { 0.0f,0.0f,0.0f }, Colors::Coral);

	//MeshBuilder


	MeshBuffer mb;
	MeshBuffer mb4;

	MeshBuffer mb5;

	//mb.Initialize(vertices,std::size(vertices),sizeof(VertexPC),indexBuf,std::size(indexBuf));
	mb.Initialize(mesh);
	mb4 .Initialize(mesh4);
	mb5 .Initialize(mesh5);

	VertexShader vs;
	vs.Initialize(mesh.vertices[0].Format, L"../Assets/Shaders/DoTransform.fx");
	PixelShader ps;
	ps.Initialize();
	ConstantBuffer cs;
	cs.Initialize();


	//cam
	Camera cam;
	cam.SetPosition({ 0.0f,0.0f,3.0f });
	cam.SetDirection({ 0.0f,0.0f,-1.0f });
	//GraphicSystem::Get()->GetContext()->VSGetConstantBuffers(0, 1, &constantBuffer);

	// constan buf take mat transpose
	Matrix4 mats[3];
	Matrix4 local;
	Matrix4 rotx;
	Matrix4 roty;
	Matrix4 rotz;

	local.SetIdentity();
	rotx.SetIdentity();
	roty.SetIdentity();
	rotz.SetIdentity();

	mats[0].SetIdentity();
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	const float rotstep =  5.0f *H::Math::Constants::DegToRad;
	bool done = false;

	int mshs = 0;

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

		cam.UpdateControl();

		//
		if (GetAsyncKeyState(0x57))//W
		{ 
			rotX -= rotstep;
			rotx.RotationX((float)(rotX));

		}
		if (GetAsyncKeyState(0x53))//S
		{
			rotX += rotstep;
			rotx.RotationX((float)(rotX));

		}
		if (GetAsyncKeyState(0x52))//R
		{
			rotZ -= rotstep;
			rotz.RotationZ((float)(rotZ));

		}
		if (GetAsyncKeyState(0x46))//F
		{
			rotZ += rotstep;
			rotz.RotationZ((float)(rotZ));

		}


		if (GetAsyncKeyState(0x41))//A 
		{
			//deg -= (deg >= rotstep) ? rotstep : 0.0f;
			rotY -= rotstep;
			roty.RotationY((float)(rotY));
		}
		if (GetAsyncKeyState(0x44))//D 
		{
			//deg += (deg < H::Math::Constants::TwoPi - rotstep) ? rotstep : 0.0f;
			rotY += rotstep;
			roty.RotationY((float)(rotY));
		}

		local = rotz *roty *  rotx;
		//

		mats[0] = local;
		//mats[0].Translation({ 0.5f,0.0f,0.0f });

		mats[1] = cam.GetViewMatrix();
		mats[2] = cam.GetPerspectiveMatrix((float)(1280.0f / 720.0f));

		mats[0] = mats[0].Transpose();
		mats[1] = mats[1].Transpose();

		mats[2] = mats[2].Transpose();

		//== 
		if (GetAsyncKeyState(0x30))//0 
		{
			mshs =  0;
		}

		if (GetAsyncKeyState(0x31))//1 
		{
			mshs = 1;
		}
		if (GetAsyncKeyState(0x32))//2
		{
			mshs = 2;
		}

		//=============
		cs.BindVS(mats);
		vs.Bind();
		ps.Bind();
		switch (mshs)
		{
		case 0:		mb.Render();
			break;
		case 1:		mb4.Render();
			break;
		case 2:		mb5.Render();
			break;

		default:
			break;
		}
		//mb4.Render();
		//mb5.Render();
		//context->Draw(std::size(vertices), 0);
		GraphicSystem::Get()->EndRender();//flip buffer
	}

	//delete
	vs.Terminate();
	ps.Terminate();
	mb.Terminate();
	cs.Terminate();
	mb4.Terminate();
	mb5.Terminate();

	//SafeRelease(constantBuffer);

	GraphicSystem::StaticTerminate();
	myWindow.Terminate();

	return 0;
}