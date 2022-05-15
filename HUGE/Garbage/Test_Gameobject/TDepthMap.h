#pragma once
#include <HUGE/Inc/HUGE.h>

struct DepthMapData
{
	Matrix4 wvp;
};

class TDepthMap
{
public:
	void Initialize();
	void Terminate();
	void Begin();
	void End();
	void Bind();
	void Render();
	void DrawDepthMap();
 	void DebugUI();
	struct SE_Context_TDepthMap : public ShaderEffectContext
	{
		DepthMapData depthMapData;
	};
	SE_Context_TDepthMap mContext;

	Camera* mLightViewCam;
	Matrix4* world;

private:
	const std::filesystem::path path_shader = L"../../Assets/Shaders/DepthMap.fx";
	std::unique_ptr<TypedConstantBuffer<DepthMapData>> mDMBuf;
 

	VertexShader mVS;
	PixelShader mPS;
	RenderTarget mRT;

};
 