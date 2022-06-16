#include "Precompiled.h"
#include "RenderMaterialService.h"
using namespace H;
META_DERIVED_BEGIN(RenderMaterialService, Service)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::RenderMaterialService::Initialize()
{
	RenderMaterial mat;
	mat.Init();
	AddRenderMaterial({ {std::move(mat)},"Standard"});

}

void H::RenderMaterialService::DebugUI()
{
	if (ImGui::TreeNode("RenderMaterialService"))
	{
		for (const auto& e : mRenderMaterialList)
		{
			ImGui::Button(e.name.c_str());
		}
		ImGui::TreePop();
	}
}

void H::RenderMaterialService::AddRenderMaterial(RenderMaterialEntry&& entry)
{
	mRenderMaterialList.push_back(std::move(entry));
}

const RenderMaterialService::RenderMaterialEntry& H::RenderMaterialService::GetRenderMaterialEntry(const std::string& name) const
{
	for (const auto& e : mRenderMaterialList)
	{
		if (e.name == name)
		{
			return e;
		}
	}
}

RenderMaterialService::RenderMaterialEntry& H::RenderMaterialService::GetRenderMaterialEntry(const std::string& name)
{
	for (auto& e : mRenderMaterialList)
	{
		if (e.name == name)
		{
			return e;
		}
	}
}

void H::RenderMaterialService::ForEachRenderMaterial(std::function<void(const RenderMaterialEntry&)> func) const
{
	for (const auto& e : mRenderMaterialList)
	{
		func(e);
	}
}
