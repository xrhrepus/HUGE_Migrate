#include "Precompiled.h"
#include <RenderMaterial.h>

void H::Graphics::RenderMaterial::SetShaderContextDebugUIFunc(std::function<void()> func)
{
	mDebugUICallBack = std::move(func);
}

void H::Graphics::RenderMaterial::Bind() const
{
}

void H::Graphics::RenderMaterial::UnBind() const
{
}

void H::Graphics::RenderMaterial::SetTransform(const TransformData& data)
{
}

void H::Graphics::RenderMaterial::SetCamera(const Camera& cam)
{
}

void H::Graphics::RenderMaterial::SetLight(const DirectionalLight& light)
{
}
