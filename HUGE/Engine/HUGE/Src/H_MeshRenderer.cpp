#include "Precompiled.h"
#include "H_MeshRenderer.h"
#include "GameObject.h"
#include "World.h"

using namespace H;
using namespace H::Graphics;

META_DERIVED_BEGIN(H_MeshRenderer, Component)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::H_MeshRenderer::Initialize()
{
	mStandardShaderEffect =
		static_cast<const ShaderEffect_Standard*>(&ShaderEffectManager::Get()->GetEffect("Standard"));
	mMesh = &(GetOwner().GetWorld().GetService<H::MeshService>()->GetMeshEntry("cube"));
}
 
void H::H_MeshRenderer::Terminate()
{
	mMeshBuffer.Terminate();
}

void H::H_MeshRenderer::Update(float deltatime)
{
}

void H::H_MeshRenderer::Render()
{
	mStandardShaderEffect->Bind();
	mStandardShaderEffect->SetContextInfo(mStandardContext);
	mMeshBuffer.Render();
	mStandardShaderEffect->UnBind();
}

void H::H_MeshRenderer::DebugUI()
{
	if (ImGui::CollapsingHeader("H_MeshRenderer"))
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

void H::H_MeshRenderer::SetMesh(const H::MeshService::MeshEntry& mesh)
{
	mMesh = &mesh;
	mMeshBuffer.Terminate();
	mMeshBuffer.Initialize(mMesh->mesh, false);
}

void H::H_MeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}
 