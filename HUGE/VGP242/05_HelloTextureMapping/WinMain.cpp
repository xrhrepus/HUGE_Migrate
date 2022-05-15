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

	//==============
	std::vector<MeshBuffer> meshbuffers;
	std::vector<VertexShader> vertexshaders;
	std::vector<PixelShader> pixelshaders;
	std::vector<ConstantBuffer> constantshaders;

	//textures
	Texture tex_earth;	tex_earth.Initialize(L"../Assets/Images/sun.jpg");
	Texture tex_basketball;	tex_basketball.Initialize(L"../Assets/Images/earth.jpg");
	Texture tex_sponge;	tex_sponge.Initialize(L"../Assets/Images/sponge_bob.png");
	Texture tex_universe;	tex_universe.Initialize(L"../Assets/Images/universe.jpg");
	Texture tex_mercury;	tex_mercury.Initialize(L"../Assets/Images/mercury.jpg");
	Texture tex_venus;	tex_venus.Initialize(L"../Assets/Images/venus.jpg");

	//meshes
	MeshPX earth = MeshBuilder::CreateShperePX(3, 12, { 0.0f,0.0f,0.0f });
	MeshPX sun = MeshBuilder::CreateShperePX(2, 12, { 0.0f,0.0f,0.0f });
	MeshPX moon = MeshBuilder::CreateShperePX(1, 8, { 0.0f,0.0f,0.0f });
	MeshPX universe = MeshBuilder::CreateSkyBoxPX(40, 8, { 0.0f,0.0f,0.0f });
	MeshPX mercury = MeshBuilder::CreateShperePX(1, 8, { 0.0f,0.0f,0.0f });
	MeshPX venus = MeshBuilder::CreateShperePX(2, 8, { 0.0f,0.0f,0.0f });

	//==============

	//meshbuffers.push_back();

	//sun======================
	//MeshBuilder
	MeshBuffer mesh_earth;
	mesh_earth.Initialize(earth);
	ConstantBuffer cs;
	cs.Initialize( 3 * sizeof(Matrix4));
	//sampler
	Sampler sp;
	sp.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Warp);
	VertexShader vs;
	vs.Initialize(earth.vertices[0].Format, L"../Assets/Shaders/Texturing.fx");
	PixelShader ps;
	ps.Initialize();
	//sun======================
	////earth=====
	MeshBuffer mesh_sun;
	mesh_sun.Initialize(sun);
	////earth=====
	//???
	MeshBuffer mesh_moon;
	mesh_moon.Initialize(moon);
	//???
	//universe==
	MeshBuffer mesh_universe;
	mesh_universe.Initialize(universe);
	//universe==
	//mercury==
	MeshBuffer mesh_mercury;
	mesh_mercury.Initialize(mercury);
	//mercury==
	MeshBuffer mesh_venus;
	mesh_venus.Initialize(venus);

	//cam
	Camera cam;
	cam.SetPosition({23.0f,23.0f,0.0f });
	cam.SetDirection({ -1.0f,-1.0f,0.0f });
	//GraphicSystem::Get()->GetContext()->VSGetConstantBuffers(0, 1, &constantBuffer);

	// constan buf take mat transpose
	Matrix4 sun_mats[3];
	Matrix4 earth_mats[3];
	Matrix4 moon_mats[3];
	Matrix4 universe_mats[3];//universe
	Matrix4 mercury_mats[3];//merrc
	Matrix4 venus_mats[3];//venus


	Matrix4 local_rotation;
	Matrix4 rotx;
	Matrix4 roty;
	Matrix4 rotz;
	//Matrix4 translate1;
	Matrix4 earth_translate;
	Matrix4 moon_translate;
	Matrix4 mercury_translate;
	Matrix4 venus_translate;
	Matrix4 earth_rotation;

	//translate1.Translation({ -3.0f, 0.0f, 0.0f } );
	earth_rotation.RotationX(H::Math::Constants::Pi * 0.5f);
	//earth_translate.Translation({ 23.0f, 0.0f, 0.0f });

	local_rotation.SetIdentity();
	
	rotx.SetIdentity();
	roty.SetIdentity();
	rotz.SetIdentity();

	sun_mats[0].SetIdentity();
	earth_mats[0].SetIdentity();
	moon_mats[0].SetIdentity();
	universe_mats[0].SetIdentity();
	//
	//mats1[0] = translate1;

	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	const float rotstep =  2.0f *H::Math::Constants::DegToRad;
	bool done = false;

	//moon rotation rad
	const float moon_earth_radius = 6.0f;
	const float mercury_sun_radius = 8.0f;
	const float venus_sun_radius = 12.0f;
	const float earth_sun_radius = 23.0f;

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
		//=============

		//cam.UpdateControl(0.1f);

		//rotation
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

		//auto rotation
		rotY += 0.2 * rotstep;
		roty.RotationY((float)(rotY));

		local_rotation = rotz *roty *  rotx;

		//moon(sponge)
		moon_translate.Translation({ moon_earth_radius * cosf( 2.0f * rotY), 0.0f,moon_earth_radius * sinf(2.0f * rotY) });

		mercury_translate.Translation({ mercury_sun_radius * cosf(3.0f * rotY), 0.0f,mercury_sun_radius * sinf(3.0f * rotY) });

		venus_translate.Translation({ venus_sun_radius * cosf(4.0f * rotY), 0.0f,venus_sun_radius * sinf(4.0f * rotY) });

		earth_translate.Translation({ earth_sun_radius * cosf( 0.5f  * rotY), 0.0f,earth_sun_radius * sinf(0.5f *rotY) });

		sun_mats[0]  = local_rotation;//sun

		mercury_mats[0] = local_rotation * mercury_translate;

		venus_mats[0] = local_rotation * venus_translate;

		earth_mats[0] = earth_rotation * local_rotation * earth_translate ;//earth

		moon_mats[0] = earth_mats[0] * moon_translate ;//moon


		sun_mats[1] = cam.GetViewMatrix();
		sun_mats[2] = cam.GetPerspectiveMatrix();

		universe_mats[0] = universe_mats[0].Transpose();
		sun_mats[0] = sun_mats[0].Transpose();
		earth_mats[0] = earth_mats[0].Transpose();
		moon_mats[0] = moon_mats[0].Transpose();
		mercury_mats[0] = mercury_mats[0].Transpose();
		venus_mats[0] = venus_mats[0].Transpose();


		sun_mats[1] = sun_mats[1].Transpose();
		sun_mats[2] = sun_mats[2].Transpose();
		venus_mats[1] = mercury_mats[1] = universe_mats[1] = moon_mats[1] = earth_mats[1] = sun_mats[1];
		venus_mats[2] = mercury_mats[2] = universe_mats[2] = moon_mats[2] = earth_mats[2] = sun_mats[2];

		//== 


		//bind===
		vs.Bind();
		ps.Bind();
		sp.BindPS();

		//universe
		cs.Set(universe_mats);
		cs.BindVS();

		tex_universe.BindPS();
		mesh_universe.Render();

		////sun====================== 1
		cs.Set(sun_mats);
		cs.BindVS();
		tex_earth.BindPS();
		mesh_earth.Render();


		////earth====================== 2
		cs.Set(earth_mats);
		cs.BindVS();

		tex_basketball.BindPS();
		mesh_sun.Render();


		////moon======== 3
		cs.Set(moon_mats);
		cs.BindVS();

		tex_sponge.BindPS();
		mesh_moon.Render();

		//mercury 4
		cs.Set(mercury_mats);
		cs.BindVS();

		tex_mercury.BindPS();
		mesh_mercury.Render();

		//venus 5
		cs.Set(venus_mats);
		cs.BindVS();

		tex_venus.BindPS();
		mesh_venus.Render();

		GraphicSystem::Get()->EndRender();//flip buffer
	}

	//delete
	vs.Terminate();
	ps.Terminate();
	mesh_earth.Terminate();
	cs.Terminate();
	sp.Terminate();

	mesh_sun.Terminate();

	mesh_moon.Terminate();

	mesh_universe.Terminate();
	mesh_mercury.Terminate();
	mesh_venus.Terminate();


	tex_earth.Terminate();
	tex_basketball.Terminate();
	tex_sponge.Terminate();
	tex_universe.Terminate();
	tex_mercury.Terminate();
	tex_venus.Terminate();
	//SafeRelease(constantBuffer);

	GraphicSystem::StaticTerminate();
	myWindow.Terminate();

	return 0;
}