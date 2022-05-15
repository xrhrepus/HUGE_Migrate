#include "Precompiled.h"
#include "ShaderEffectManager.h"

namespace
{
	std::unique_ptr<H::Graphics::ShaderEffectManager> sShaderEffectManager;
}
void H::Graphics::ShaderEffectManager::StaticInitialize()
{
	ASSERT(sShaderEffectManager == nullptr, "[Graphic::ShaderEffectManager] system already initialized!");
	sShaderEffectManager = std::make_unique<ShaderEffectManager>();
	sShaderEffectManager->Initialize();

}
void H::Graphics::ShaderEffectManager::StaticTerminate()
{
	if (sShaderEffectManager != nullptr)
	{
		sShaderEffectManager->Terminate();
		sShaderEffectManager.reset();
	}

}
H::Graphics::ShaderEffectManager* H::Graphics::ShaderEffectManager::Get()
{
	ASSERT(sShaderEffectManager != nullptr, "[Graphic::ShaderEffectManager] instance not created");
	return sShaderEffectManager.get();
}
void H::Graphics::ShaderEffectManager::Initialize()
{
	//AddEffect("Texturing", path_Texturing);
	AddEffect(H::Graphics::ShaderEffectManager::EffectType::Standard,"Standard");
	AddEffect(H::Graphics::ShaderEffectManager::EffectType::Skinning,"Skinning");
	AddEffect(H::Graphics::ShaderEffectManager::EffectType::Toon, "Toon");

}
void H::Graphics::ShaderEffectManager::Terminate()
{
	for (auto&[key, val] : mInventory)
	{
		val->Terminate();
	}
}

bool H::Graphics::ShaderEffectManager::AddEffect(EffectType type, std::string name)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		std::unique_ptr< ShaderEffect> sp;
		switch (type)
		{

		//case H::Graphics::ShaderEffectManager::EffectType::Texturing:
		//	sp = std::make_unique<ShaderEffect_Standard>();
		//	break;
		case H::Graphics::ShaderEffectManager::EffectType::Standard:
			sp = std::make_unique<ShaderEffect_Standard>();
			break;
		case H::Graphics::ShaderEffectManager::EffectType::Skinning:
			sp = std::make_unique<ShaderEffect_Skinning>();
			break;
		case H::Graphics::ShaderEffectManager::EffectType::Toon:
			sp = std::make_unique<ShaderEffect_Toon>();
			break;

		default:
			return false;
			break;
		}
		sp->Initialize(sp->GetShaderFilePath());
		iter->second = std::move(sp);
	}
	return success;

}

//H::Graphics::ShaderEffect * H::Graphics::ShaderEffectManager::GetEffect(std::string name)
//{
//	auto iter = mInventory.find(name);
//	return iter != mInventory.end() ? iter->second.get() : nullptr;
//}
