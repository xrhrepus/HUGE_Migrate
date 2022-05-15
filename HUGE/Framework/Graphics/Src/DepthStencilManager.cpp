#include "Precompiled.h"
#include "DepthStencilManager.h"
using namespace H::Graphics;

namespace
{
	std::unique_ptr<DepthStencilManager> sDepthStencilManager;
}
void DepthStencilManager::StaticInitialize()
{
	ASSERT(sDepthStencilManager == nullptr, "[Graphic::DepthStencilManager] system already initialized!");
	sDepthStencilManager = std::make_unique<DepthStencilManager>();
	sDepthStencilManager->Initialize();

}
void DepthStencilManager::StaticTerminate()
{
	if (sDepthStencilManager != nullptr)
	{
		sDepthStencilManager->Terminate();
		sDepthStencilManager.reset();
	}

}
DepthStencilManager* DepthStencilManager::Get()
{
	ASSERT(sDepthStencilManager != nullptr, "[Graphic::DepthStencilManager] instance not created");
	return sDepthStencilManager.get();
}
void DepthStencilManager::Initialize()
{
	//ztest
	AddDepthStencilState("ZTest",true, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Less,
		false, DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);

	AddDepthStencilState("ZTestAlways", true, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Always,
		false, DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);
	AddDepthStencilState("ZTestOff", false, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Never,
		false, DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);


	//layer masking
	AddDepthStencilState("DrawMask", true, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Less,
		true, DepthStencilState::Comparison::Always, DepthStencilState::StencilOperation::Increment_Saturate, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);

	AddDepthStencilState("ApplyMask", true, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Less,
		true, DepthStencilState::Comparison::Less, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);

}
void DepthStencilManager::Terminate()
{
	for (auto&[key, val] : mInventory)
	{
		val->Terminate();
	}
}


bool H::Graphics::DepthStencilManager::AddDepthStencilState(std::string name, bool depthEnable, DepthStencilState::DepthWriteMask depthWriteMask, DepthStencilState::Comparison depthFunc,
	bool stencilEnable, DepthStencilState::Comparison frontStencilFunc, DepthStencilState::StencilOperation frontStencilPass, DepthStencilState::StencilOperation frontStencilFail, DepthStencilState::StencilOperation frontDepthFail,
	DepthStencilState::Comparison backStencilFunc, DepthStencilState::StencilOperation backStencilPass, DepthStencilState::StencilOperation backStencilFail, DepthStencilState::StencilOperation backDepthFail)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sp = std::make_unique<DepthStencilState>();
		sp->Initialize(depthEnable, depthWriteMask,  depthFunc,
			  stencilEnable,  frontStencilFunc, frontStencilPass,  frontStencilFail, frontDepthFail,
			 backStencilFunc,  backStencilPass,   backStencilFail,  backDepthFail);
		iter->second = std::move(sp);
	}
	return success;

}

DepthStencilState* DepthStencilManager::GetDepthStencilState(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
