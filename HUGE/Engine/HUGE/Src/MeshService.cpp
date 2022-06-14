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
	if (ImGui::CollapsingHeader("MeshService"))
	{
		for (const auto& e : mMeshList)
		{
			ImGui::Button(e.name.c_str());
		}
	}
}

void H::MeshService::AddMesh(MeshEntry&& entry)
{
	mMeshList.push_back(std::move(entry));
}
