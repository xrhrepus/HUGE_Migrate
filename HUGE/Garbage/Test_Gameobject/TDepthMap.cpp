#include "TDepthMap.h"

void TDepthMap::Initialize()
{
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
	mRT.BeginRender();
}

void TDepthMap::End()
{
	mRT.EndRender();
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

void TDepthMap::DrawDepthMap()
{
	mVS.Bind();
	mPS.Bind();

	auto matlighView = mLightViewCam->GetViewMatrix();
	auto matprojlight = mLightViewCam->GetPerspectiveMatrix();

	//auto matTrans = H::Math::Translation(*mTargetPos);
	////auto matRot = H::Math::Translation(*mTargetPos);
	//auto matTrans = H::Math::Translation(*mTargetPos);
	auto wvp = H::Math::Transpose((*world) * matlighView * matprojlight);
	DepthMapData dmd; 
	dmd.wvp = wvp;
	mDMBuf->Set(dmd);
	mDMBuf->BindVS(0);


}

void TDepthMap::DebugUI()
{
	ImGui::Begin("dm");


	ImGui::Image(mRT.GetShaderResourceView(), {200.0f,200.0f});
	ImGui::End();
}
