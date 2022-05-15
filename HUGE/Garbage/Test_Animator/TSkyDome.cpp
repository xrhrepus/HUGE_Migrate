#include "TSkyDome.h"

void TSkyDome::Initialize()
{
	mSkyDome = MeshBuilder::CreateSkyBoxPX(500.0f, 8, { 0.0f });
	mMeshBuffer.Initialize(mSkyDome);
	mTexture = TextureManager::Get()->LoadTexture("c4.png");
	mVS.Initialize(TEXTURING_FILE_PATH);
	mPS.Initialize(TEXTURING_FILE_PATH, "PS");
	mCB.Initialize();
	//mData.wvp = H::Math::Matrix4::Identity();
	//mCB.Set(mData);
}

void TSkyDome::Terminate()
{
	mMeshBuffer.Terminate();
	mVS.Terminate();
	mPS.Terminate();
	mCB.Terminate();
}

void TSkyDome::Render(const Camera& cam)
{
	TextureManager::Get()->GetTexture(mTexture)->BindPS(0);
	SamplerManager::Get()->GetSampler(SamplerType::LinearWrap)->BindPS();
	mVS.Bind();
	mPS.Bind();
 	//mData.wvp = H::Math::Transpose(H::Math::Matrix4::Identity() * cam.GetViewMatrix() * cam.GetPerspectiveMatrix());
	mData.wvp = H::Math::Transpose(H::Math::Matrix4::rotationX(90.0f*H::Math::Constants::DegToRad ) * H::Math::Matrix4::rotationY(135.0f*H::Math::Constants::DegToRad) * cam.GetViewMatrix() * cam.GetPerspectiveMatrix());

	mCB.Set(mData);
	mCB.BindVS();
 
	mMeshBuffer.Render();
}

void TSkyDome::DebugUI()
{
 
}
