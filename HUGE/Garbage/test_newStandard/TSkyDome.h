#pragma once
#include <HUGE/Inc/HUGE.h>
using namespace H::Graphics;

class TSkyDome
{
public:
	struct TSkyDome_Data
	{
		H::Math::Matrix4 padding;
		H::Math::Matrix4 wvp;
	};
	void Initialize();
	void Terminate();
	void Render(const Camera& cam);
	
	void DebugUI();

private:
	MeshPX mSkyDome;
	MeshBuffer mMeshBuffer;
	TextureId mTexture;


	//px
	const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";

	VertexShader mVS;
	PixelShader mPS;
	TSkyDome_Data mData;
	TypedConstantBuffer<TSkyDome_Data> mCB;
};

 