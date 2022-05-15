#include "TShadowMap.h"

void TShadowMap::Initialize()
{
	mTransformBuf = std::make_unique<H::Graphics::TransformBuffer>();
	mLightBuf = std::make_unique<H::Graphics::LightBuffer>();

	mTransformBuf->Initialize(mContext.transformData);
	mLightBuf->Initialize(mContext.directionalLight);

	mVS.Initialize(0, path_shader.c_str());
	mPS.Initialize(path_shader.c_str(), "PS");
}

void TShadowMap::Terminate()
{
	mTransformBuf->Terminate();
	mLightBuf->Terminate();

	mVS.Terminate();
	mPS.Terminate();

}

void TShadowMap::Bind()
{
	mVS.Bind();
	mPS.Bind();

	mTransformBuf->Set(mContext.transformData);
	mTransformBuf->BindVS();
	mTransformBuf->BindPS();

	//mLightBuf->BindVS();
}

void TShadowMap::DebugUI()
{
}
