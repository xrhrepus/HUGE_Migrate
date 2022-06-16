#include "Precompiled.h"
#include <RenderMaterial.h>
#include <ShaderEffectManager.h>

void H::Graphics::RenderMaterial::SetShaderContextDebugUIFunc(std::function<void()> func)
{
	mDebugUICallBack = std::move(func);
}

void H::Graphics::RenderMaterial::Init()
{
	mShaderEffect = (&ShaderEffectManager::Get()->GetEffect(mShaderEffectName));
	mShaderEffectContext = mShaderEffect->CreateShaderEffectContext();
	SetShaderContextDebugUIFunc(mShaderEffectContext->CreateDebugUI());
}

void H::Graphics::RenderMaterial::Bind() const
{
	mShaderEffect->SetContextInfo(*mShaderEffectContext);
	mShaderEffect->Bind();
}

void H::Graphics::RenderMaterial::UnBind() const
{
	mShaderEffect->UnBind();
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

void H::Graphics::RenderMaterial::DebugUI()
{
	const char* name = mShaderEffectName.c_str();
	ImGui::PushID(name);
	if (ImGui::TreeNode(name))
	{
		mDebugUICallBack();
		ImGui::TreePop();
	}
	ImGui::PopID();
}
