#include "TDrawNormalDepth.h"

void TDrawNormalDepth::Initialize()
{
	auto gSys = GraphicSystem::Get();
	//createDN
	mCreateDN_VS.Initialize(EDGEDRAW_CREATEDN_FILE_PATH.c_str());
	mCreateDN_PS.Initialize(EDGEDRAW_CREATEDN_FILE_PATH.c_str(), "PS");
	mCreateDN_RT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mTFBuf.Initialize();

}

void TDrawNormalDepth::Terminate()
{
	mCreateDN_VS.Terminate();
	mCreateDN_PS.Terminate();
	mCreateDN_RT.Terminate();

}

void TDrawNormalDepth::SetDNBuffer(const TransformData & tfdata)
{
 	mTFBuf.Set(tfdata);
}

void TDrawNormalDepth::BeginDN()
{
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	mCreateDN_RT.BeginRender();
	mCreateDN_VS.Bind();
	mCreateDN_PS.Bind();
	mTFBuf.BindVS(0);

}

void TDrawNormalDepth::EndDN()
{
	mCreateDN_RT.EndRender();

}

void TDrawNormalDepth::DebugUI()
{
}

ID3D11ShaderResourceView * TDrawNormalDepth::GetDN_SEresView()
{
	return mCreateDN_RT.GetShaderResourceView();
}
