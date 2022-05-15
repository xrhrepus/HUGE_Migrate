#include "Precompiled.h"
#include "RasterizerStateManager.h"


using namespace H::Graphics;

namespace
{
	std::unique_ptr<RasterizerStateManager> sRasterizerStateManager;
}
void RasterizerStateManager::StaticInitialize()
{
	ASSERT(sRasterizerStateManager == nullptr, "[Graphic::RasterizerStateManager] system already initialized!");
	sRasterizerStateManager = std::make_unique<RasterizerStateManager>();
	sRasterizerStateManager->Initialize();

}
void RasterizerStateManager::StaticTerminate()
{
	if (sRasterizerStateManager != nullptr)
	{
		sRasterizerStateManager->Terminate();
		sRasterizerStateManager.reset();
	}

}
RasterizerStateManager* RasterizerStateManager::Get()
{
	ASSERT(sRasterizerStateManager != nullptr, "[Graphic::RasterizerStateManager] instance not created");
	return sRasterizerStateManager.get();
}
void RasterizerStateManager::Initialize()
{
	AddRasterizerState("Solid", RasterizerState::CullMode::Back, RasterizerState::FillMode::Solid);
	AddRasterizerState("WireFrame", RasterizerState::CullMode::Back, RasterizerState::FillMode::WireFrame);
	AddRasterizerState("CullFrontSolid", RasterizerState::CullMode::Front,RasterizerState::FillMode::Solid);
	AddRasterizerState("CullFrontWireFrame", RasterizerState::CullMode::Front, RasterizerState::FillMode::WireFrame);
	AddRasterizerState("CullNoneSolid", RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	AddRasterizerState("CullNoneWireFrame", RasterizerState::CullMode::None, RasterizerState::FillMode::WireFrame);

 }
void RasterizerStateManager::Terminate()
{
	for (auto&[key, val] : mInventory)
	{
		val->Terminate();
	}
}


bool H::Graphics::RasterizerStateManager::AddRasterizerState(std::string name, H::Graphics::RasterizerState::CullMode cmode, H::Graphics::RasterizerState::FillMode fmode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sp = std::make_unique<RasterizerState>();
		sp->Initialize(cmode,fmode);
		iter->second = std::move(sp);
	}
	return success;

}

RasterizerState* RasterizerStateManager::GetRasterizerState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
