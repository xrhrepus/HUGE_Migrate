#include "Precompiled.h"
#include "LightService.h"
using namespace H;
META_DERIVED_BEGIN(LightService, Service)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::LightService::Initialize()
{
	AddDirectionalLight({ "Directional Light" });
}

void H::LightService::DebugUI()
{
	if (ImGui::TreeNode("LightService"))
	{
		for (auto& e : mDirectionalLight)
		{
			const char* name = e.name.c_str();
			ImGui::PushID(name);
			if (ImGui::TreeNode(name))
			{
				auto& dirLight = e.dirLight;
				bool dirChange = false;
				if (ImGui::DragFloat3("dir", dirLight.direction.v.data(), 0.1f))
				{
					dirChange = true;
				}
				ImGui::ColorPicker4("ambient", &dirLight.ambient.x, 0.01f);
				ImGui::ColorPicker4("diffuse", &dirLight.diffuse.x, 0.01f);
				ImGui::ColorPicker4("specular", &dirLight.specular.x, 0.01f);
				if (dirChange)
				{
					dirLight.direction = H::Math::Normalize(dirLight.direction);
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}

void H::LightService::AddDirectionalLight(DirectionalLightEntry&& entry)
{
	mDirectionalLight.push_back(std::move(entry));
}

const LightService::DirectionalLightEntry& H::LightService::GetDirectionalLightEntry(const std::string& name) const
{
	for (const auto& e : mDirectionalLight)
	{
		if (e.name == name)
		{
			return e;
		}
	}
}

LightService::DirectionalLightEntry& H::LightService::GetDirectionalLightEntry(const std::string& name)
{
	for (auto& e : mDirectionalLight)
	{
		if (e.name == name)
		{
			return e;
		}
	}
}

void H::LightService::ForEachDirectionalLightEntry(std::function<void(const DirectionalLightEntry&)> func) const
{
	for (const auto& e : mDirectionalLight)
	{
		func(e);
	}
}
