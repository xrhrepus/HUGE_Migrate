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
	void DrawDepthMap(const Matrix4& world);
 	void DebugUI();
	RenderTarget* GetRenderTarget();

	void ComputeSth();

	struct SE_Context_TDepthMap : public ShaderEffectContext
	{
		DepthMapData depthMapData;
	};
	SE_Context_TDepthMap mContext;

	Camera mLightViewCam;

private:
	const std::filesystem::path path_shader = L"../../Assets/Shaders/DepthMap.fx";
	//const std::filesystem::path path_shader = L"../../../v330_shaders/Shaders/DepthMap.fx";

	std::unique_ptr<TypedConstantBuffer<DepthMapData>> mDMBuf;
 
 
	VertexShader mVS;
	PixelShader mPS;
	RenderTarget mRT;

	//ortho view frustum

	Matrix4 mLightProjMatrix;//resultr
	std::vector<Vector3> mViewFrustumVtx;

};
 