#include "Precompiled.h"
#include "MeshService.h"
using namespace H;
META_DERIVED_BEGIN(MeshService, Service)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::MeshService::Initialize()
{
	AddMesh({ H::Graphics::MeshBuilder::CreateCube(1.0f,1.0f,1.0f,Vector3::Zero()),"cube" });
	AddMesh({ H::Graphics::MeshBuilder::CreateQuad(1.0f,1.0f,Vector3::Zero()),"quad" });
	AddMesh({ H::Graphics::MeshBuilder::CreateShpere(1.0f,32.0f,Vector3::Zero()),"sphere" });
}

void H::MeshService::DebugUI()
{
	if (ImGui::TreeNode("MeshService"))
	{
		for (const auto& e : mMeshList)
		{
			ImGui::Button(e.name.c_str());
		}
		ImGui::TreePop();
	}
}

void H::MeshService::AddMesh(MeshEntry&& entry)
{
	mMeshList.push_back(std::move(entry));
}

const H::MeshService::MeshEntry& H::MeshService::GetMeshEntry(const std::string& name) const
{
	for (const auto& e : mMeshList)
	{
		if (e.name == name)
		{
			return e;
		}
	}
}

void H::MeshService::ForEachMesh(std::function<void(const H::MeshService::MeshEntry&)> func) const
{
	for (const auto& e : mMeshList)
	{
		func(e);
	}

}
