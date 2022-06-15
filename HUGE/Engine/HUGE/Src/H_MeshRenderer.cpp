#include "Precompiled.h"
#include "H_MeshRenderer.h"
#include "GameObject.h"
#include "World.h"

using namespace H;
using namespace H::Graphics;

META_DERIVED_BEGIN(MeshRenderer, Component)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::MeshRenderer::Initialize()
{
	mStandardShaderEffect =
		static_cast<const ShaderEffect_Standard*>(&ShaderEffectManager::Get()->GetEffect("Standard"));
	mMesh = &(GetOwner().GetWorld().GetService<H::MeshService>()->GetMeshEntry("cube"));
}
 
void H::MeshRenderer::Terminate()
{
	mMeshBuffer.Terminate();
}

void H::MeshRenderer::Update(float deltatime)
{
}

void H::MeshRenderer::Render()
{
	mStandardShaderEffect->Bind();
	mStandardShaderEffect->SetContextInfo(mStandardContext);
	mMeshBuffer.Render();
	mStandardShaderEffect->UnBind();
}

void H::MeshRenderer::DebugUI()
{
	if (ImGui::TreeNode("MeshRenderer"))
	{
		ImGui::PushID("hms");
		ImGui::Text("current mesh");
		ImGui::Text(mMesh->name.c_str());

		if (ImGui::BeginChild("setmesh"))
		{
			GetOwner().GetWorld().GetService<H::MeshService>()->ForEachMesh(
				[this](const H::MeshService::MeshEntry& meshEntry)
				{
					if (ImGui::Button(meshEntry.name.c_str()))
					{
						SetMesh(meshEntry);
					}
				});
			ImGui::EndChild();
		}
		ImGui::PopID();
	}

}

void H::MeshRenderer::SetMesh(const H::MeshService::MeshEntry& mesh)
{
	mMesh = &mesh;
	mMeshBuffer.Terminate();
	mMeshBuffer.Initialize(mMesh->mesh, false);
}

void H::MeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}
 