#include "TDepthMap.h"

void TDepthMap::Initialize()
{
	mLightViewCam.SetPosition({ 121.0f,100.0,100.0f });
	mLightViewCam.SetDirection(H::Math::Normalize( { -121.0f,-100.0,-100.0f }));

	auto gSys = GraphicSystem::Get();

	mDMBuf = std::make_unique<H::Graphics::TypedConstantBuffer<DepthMapData>>();
 
	mDMBuf->Initialize(mContext.depthMapData);
 
	mVS.Initialize(0, path_shader.c_str());
	mPS.Initialize(path_shader.c_str(), "PS");

	constexpr uint32_t depthMapsize = 1024;
	mRT.Initialize(depthMapsize, depthMapsize, RenderTarget::Format::RGBA_U8);
}

void TDepthMap::Terminate()
{
	mDMBuf->Terminate();
 
	mVS.Terminate();
	mPS.Terminate();
	mRT.Terminate();
}

void TDepthMap::Begin()
{
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	mRT.BeginRender();
}

void TDepthMap::End()
{
	mRT.EndRender();
	mVS.UnBind();
	mPS.UnBind();
	mDMBuf->UnBindVS(0);

}

void TDepthMap::Bind()
{
	mVS.Bind();
	mPS.Bind();

	mDMBuf->Set(mContext.depthMapData);
	mDMBuf->BindVS();
 
 }

void TDepthMap::Render()
{
	
}

void TDepthMap::DrawDepthMap(const Matrix4& world)
{
	mVS.Bind();
	mPS.Bind();

	//auto world = H::Math::Translation({0.0f});
	//auto matlighView = mLightViewCam.GetViewMatrix();
	//auto matprojlight = mLightViewCam.GetPerspectiveMatrix();

	//auto matTrans = H::Math::Translation(*mTargetPos);
	////auto matRot = H::Math::Translation(*mTargetPos);
	//auto matTrans = H::Math::Translation(*mTargetPos);
	Matrix4 comp = (world * mLightViewCam.GetViewMatrix() * mLightViewCam.GetPerspectiveMatrix());
	Matrix4 wvp = H::Math::Transpose(comp);
	
	mContext.depthMapData.wvp = wvp;
	mDMBuf->Set(mContext.depthMapData);
	mDMBuf->BindVS(0);
}

void TDepthMap::DebugUI()
{
	if (ImGui::CollapsingHeader("TDepthMap"))
	{
		ImGui::Image(mRT.GetShaderResourceView(), { 200.0f,200.0f });
		

	}
 
 }

RenderTarget * TDepthMap::GetRenderTarget() 
{
	return &mRT;
}

void TDepthMap::ComputeSth()
{


}

 