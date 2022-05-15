#include "TEdgeDraw.h"

void TEdgeDraw::Initialize()
{
	auto gSys = GraphicSystem::Get();
	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);

	//origin scene
	mOriginRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	//createDN
	mCreateDN_VS.Initialize(EDGEDRAW_CREATEDN_FILE_PATH.c_str());
	mCreateDN_PS.Initialize(EDGEDRAW_CREATEDN_FILE_PATH.c_str(), "PS");
	mCreateDN_RT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	//final draw
	mVS.Initialize(EDGEDRAW_POSTPROCESS_FILE_PATH.c_str());
	mPS.Initialize(EDGEDRAW_POSTPROCESS_FILE_PATH.c_str(), "PS");
	mRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	//param
	mEdgeDrawBuffer.Initialize(mEdgeDrawData);
	mEdgeDrawData.uv_step = { 1.0f / GraphicSystem::Get()->GetBackBufferWidth(),1.0f / GraphicSystem::Get()->GetBackBufferHeight() };
	mTFBuf.Initialize(mTFData);
}

void TEdgeDraw::Terminate()
{
	mOriginRT.Terminate();
	mScreenMeshBuffer.Terminate();
	mEdgeDrawBuffer.Terminate();
	mCreateDN_VS.Terminate();
	mCreateDN_PS.Terminate();
	mCreateDN_RT.Terminate();
	mVS.Terminate();
	mPS.Terminate();
	mRT.Terminate();
	mTFBuf.Terminate();

 }

void TEdgeDraw::SetDNBuffer(const TransformData & tfdata)
{
	mTFData = tfdata;
	mTFBuf.Set(mTFData);
}
void TEdgeDraw::BeginDN()
{
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	mCreateDN_RT.BeginRender();
	mCreateDN_VS.Bind();
	mCreateDN_PS.Bind();
	mTFBuf.BindVS(0);
 
}
 
void TEdgeDraw::EndDN()
{
	mCreateDN_RT.EndRender();
}

void TEdgeDraw::Begin()
{
	mOriginRT.BeginRender();
	BlendStateManager::Get()->GetBlendState(Blend::AlphaBlend)->Set();
	SamplerManager::Get()->GetSampler(SamplerType::PointClamp)->BindPS(0);
}

void TEdgeDraw::End()
{
	mOriginRT.EndRender();
}

void TEdgeDraw::Render()
{
	//
	SamplerManager::Get()->GetSampler(SamplerType::PointClamp)->BindPS();
	mRT.BeginRender();
	mOriginRT.BindPS(0);
	mCreateDN_RT.BindPS(1);
	mVS.Bind();
	mPS.Bind();
	mEdgeDrawBuffer.Set(mEdgeDrawData);
	mEdgeDrawBuffer.BindPS(0);
	mScreenMeshBuffer.Render();
	mRT.EndRender();
 
	//
	mRT.BindPS(0);
	mScreenMeshBuffer.Render();


	//unbind all
	mOriginRT.UnbindPS(0);
	mCreateDN_RT.UnbindPS(1);
	mEdgeDrawBuffer.UnBindPS(0);
	mRT.UnbindPS(0);
}

void TEdgeDraw::DebugUI()
{
	ImGui::Begin("TEdgeDraw");
	if (ImGui::CollapsingHeader("Views##TEdgeDraw"))
	{
		ImGui::Text("[DepthNormal]");
		ImGui::Image(GetDN_SEresView(), { 320.0f,180.0f });
		ImGui::Text("[Origin]");
		ImGui::Image(GetOR_SEresView(), { 320.0f,180.0f });
		ImGui::Text("[Final]");
		ImGui::Image(GetFinal_SEresView(), { 320.0f,180.0f });
	}
	if (ImGui::CollapsingHeader("Params##TEdgeDraw"))
	{
		ImGui::ColorEdit4("EdgeColor##TEdgeDraw", &mEdgeDrawData.edgeColor.r);
		ImGui::DragFloat("threshold##TEdgeDraw", &mEdgeDrawData.threshold, 0.005f, 0.0f, 1.0f);
		ImGui::Text("Texel[u: %f ,v: %f ]", &mEdgeDrawData.uv_step.x, &mEdgeDrawData.uv_step.y);
		ImGui::DragFloat("edgeColorWeight##TEdgeDraw", &mEdgeDrawData.edgeColorWeight, 0.01f, 0.0f, 1.0f);

	}

	ImGui::End();

}

ID3D11ShaderResourceView* TEdgeDraw::GetDN_SEresView()
{
	return mCreateDN_RT.GetShaderResourceView();
}

ID3D11ShaderResourceView * TEdgeDraw::GetOR_SEresView()
{
	return mOriginRT.GetShaderResourceView();
}

ID3D11ShaderResourceView * TEdgeDraw::GetFinal_SEresView()
{
	return mRT.GetShaderResourceView();
}
