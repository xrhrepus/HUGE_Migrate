#include "Precompiled.h"
#include "ShaderEffectManager.h"
#include <StandardEffect.h>

namespace
{
	std::unique_ptr<H::Graphics::ShaderEffectManager> sShaderEffectManager;
}

using namespace H;
using namespace H::Graphics;

void ShaderEffectManager::StaticInitialize()
{
	ASSERT(sShaderEffectManager == nullptr, "[Graphic::ShaderEffectManager] system already initialized!");
	sShaderEffectManager = std::make_unique<ShaderEffectManager>();
	sShaderEffectManager->Initialize();

}
void ShaderEffectManager::StaticTerminate()
{
	ASSERT(sShaderEffectManager != nullptr, "[Graphic::ShaderEffectManager] system not initialized!");
	if (sShaderEffectManager != nullptr)
	{
		sShaderEffectManager->Terminate();
		sShaderEffectManager.reset();
	}

}
ShaderEffectManager* ShaderEffectManager::Get()
{
	ASSERT(sShaderEffectManager != nullptr, "[Graphic::ShaderEffectManager] instance not created");
	return sShaderEffectManager.get();
}
void ShaderEffectManager::Initialize()
{
	//AddEffect("Texturing", path_Texturing);
	AddEffect(std::make_unique<ShaderEffect_Standard>(),"Standard");
	//AddEffect(std::make_unique<ShaderEffect_Skinning>(),"Skinning");
	//AddEffect(std::make_unique<ShaderEffect_Toon>(), "Toon");

}
void ShaderEffectManager::Terminate()
{
	for (auto&[key, val] : mInventory)
	{
		val->Terminate();
	}
}

bool ShaderEffectManager::AddEffect(std::unique_ptr<ShaderEffect>&& shaderEffect, const std::string& name)
{
	ASSERT(shaderEffect != nullptr, "[Graphic::ShaderEffectManager::AddEffect] Don't add a nullptr for Shader effect.");
	if (!shaderEffect) {
		return false;
	}
	shaderEffect->Initialize();
	auto[iter, success] = mInventory.try_emplace(name, std::move(shaderEffect));
	ASSERT(success, "[Graphic::ShaderEffectManager::AddEffect] Shader effect name already existed.");
	return success;
}

const ShaderEffect& ShaderEffectManager::GetEffect(const std::string & name) const
{
	const auto it = mInventory.find(name);
	if (it != mInventory.cend())
	{
		return *it->second;
	}
	// TODO: add a error shader effect (texture with error tex?)
	return *mInventory.find("Standard")->second;
}

