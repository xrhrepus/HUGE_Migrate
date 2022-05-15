#include "Precompiled.h"
#include "Skydome.h"

void H::Skydome::Initialize(float radius, int slice)
{
	mSkyDomeMesh = MeshBuilder::CreateSkyBoxPX(radius, slice, { 0.0f });
	mMB.Initialize(mSkyDomeMesh);
	mVS.Initialize(TEXTURING_FILE_PATH);
	mPS.Initialize(TEXTURING_FILE_PATH, "PS");
	mCB.Initialize();

	mDiffuseId = TextureManager::Get()->LoadTexture("sunset_front.tif");

}

void H::Skydome::Terminate()
{
	mVS.Terminate();
	mPS.Terminate();
	mCB.Terminate();
	mMB.Terminate();
}

void H::Skydome::Render(const H::Graphics::Camera & camera)
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
	SamplerManager::Get()->GetSampler(SamplerType::LinearWrap)->BindPS();
	TextureManager::Get()->GetTexture(mDiffuseId)->BindPS(0);
	mVS.Bind();
	mPS.Bind();
 	mData.wvp = H::Math::Transpose(mTF * camera.GetViewMatrix() * camera.GetPerspectiveMatrix());

	mCB.Set(mData);
	mCB.BindVS();

	mMB.Render();

}

void H::Skydome::DrawEditorUI()
{
	if (ImGui::CollapsingHeader("Skydome"))
	{
 
	}

}
 