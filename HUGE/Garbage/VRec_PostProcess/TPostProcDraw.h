#pragma once
#include <HUGE\Inc\HUGE.h>
using namespace H::Graphics;

class TPostProcDraw
{
public:
	void Initialize();
	void Terminate();

	void Begin();
 	void Render();
	void End();

	void DebugUI();

private:
	const wchar_t* POSTPROCESSING_FILE_PATH = L"../../Assets/Shaders/PostProcessing.fx";
	VertexShader mVS;
	PixelShader mPS;
	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

};

 