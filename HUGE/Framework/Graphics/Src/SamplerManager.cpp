#include "Precompiled.h"
#include "SamplerManager.h"
using namespace H;
using namespace H::Graphics;
namespace
{
	std::unique_ptr<SamplerManager> sSamplerManager;
}
const std::string  SamplerType::PointClamp = "PointClamp";
const std::string  SamplerType::PointWrap = "PointWrap";
const std::string  SamplerType::LinearClamp = "LinearClamp";
const std::string  SamplerType::LinearWrap = "LinearWrap";
const std::string  SamplerType::AnisotropicClamp = "AnisotropicClamp";
const std::string  SamplerType::AnisotropicWrap = "AnisotropicWrap";

void SamplerManager::StaticInitialize(  )
{
	ASSERT(sSamplerManager == nullptr, "[Graphic::SamplerManager] system already initialized!");
	sSamplerManager = std::make_unique<SamplerManager>();
	sSamplerManager->Initialize();
 
}
void SamplerManager::StaticTerminate()
{
	if (sSamplerManager != nullptr)
	{
		sSamplerManager->Terminate();
		sSamplerManager.reset();
	}

}
SamplerManager* SamplerManager::Get()
{
	ASSERT(sSamplerManager != nullptr, "[Graphic::SamplerManager] instance not created");
	return sSamplerManager.get();
}

//SamplerManager::~SamplerManager()
//{
//	for (auto&[key, value] : mInventory)
//	{
//		value->Terminate();
//	}
//}
void SamplerManager::Initialize()
{
	AddSampler("PointClamp", Sampler::Filter::Point, Sampler::AddressMode::Clamp);
	AddSampler("PointWrap", Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	AddSampler("LinearClamp", Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	AddSampler("LinearWrap", Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	AddSampler("AnisotropicClamp", Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	AddSampler("AnisotropicWrap", Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

 
 }
void SamplerManager::Terminate()
{
	for (auto& [key,val] : mInventory)
	{
		val->Terminate();
	}
}


bool H::Graphics::SamplerManager::AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	auto[iter, success] = mInventory.try_emplace(name, nullptr);
	if (success)
	{
		auto sp = std::make_unique<Sampler>();
		sp->Initialize(filter, addressMode);
		iter->second = std::move(sp);
	}
	return success;

}

Sampler* SamplerManager::GetSampler(std::string name)
{
	auto iter = mInventory.find(name);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}
