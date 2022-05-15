#include "Precompiled.h"
#include "BlendStateManager.h"

using namespace H::Graphics;

namespace
{
	std::unique_ptr<BlendStateManager> sBlendStateManager;
}
const std::string Blend::Additive = "Additive";
const std::string Blend::Opaque = "Opaque";
const std::string Blend::AlphaBlend = "AlphaBlend";
const std::string Blend::AlphaPremultiplied = "AlphaPremultiplied";


void BlendStateManager::StaticInitialize()
{
	ASSERT(sBlendStateManager == nullptr, "[Graphic::BlendStateManager] system already initialized!");
	sBlendStateManager = std::make_unique<BlendStateManager>();
	sBlendStateManager->Initialize();

}
void BlendStateManager::StaticTerminate()
{
	if (sBlendStateManager != nullptr)
	{
		sBlendStateManager->Terminate();
		sBlendStateManager.reset();
	}

}
BlendStateManager* BlendStateManager::Get()
{
	ASSERT(sBlendStateManager != nullptr, "[Graphic::BlendStateManager] instance not created");
	return sBlendStateManager.get();
}
void BlendStateManager::Initialize()
{
	AddBlendState("Opaque", BlendState::Mode::Opaque);
	AddBlendState("AlphaBlend", BlendState::Mode::AlphaBlend);
	AddBlendState("AlphaPremultiplied", BlendState::Mode::AlphaPremultiplied);
	AddBlendState("Additive", BlendState::Mode::Additive);
}
void BlendStateManager::Terminate()
{
	for (auto&[key, val] : mInventory)
	{
		val->Terminate();
	}
}


bool H::Graphics::BlendStateManager::AddBlendState(std::string name, H::Graphics::BlendState::Mode mode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sp = std::make_unique<BlendState>();
		sp->Initialize(mode);
		iter->second = std::move(sp);
	}
	return success;

}

BlendState* BlendStateManager::GetBlendState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
