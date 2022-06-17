#include "Precompiled.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "World.h"
#include "CameraService.h"

using namespace H;
using namespace H::Graphics;

META_DERIVED_BEGIN(MeshRenderer, Component)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::MeshRenderer::Initialize()
{
	auto& world = GetOwner().GetWorld();
	mStandardShaderEffect =
		static_cast<const ShaderEffect_Standard*>(&ShaderEffectManager::Get()->GetEffect("Standard"));
	SetMesh(world.GetService<H::MeshService>()->GetMeshEntry("cube"));
	SetRenderMaterial(world.GetService<H::RenderMaterialService>()->GetRenderMaterialEntry("Standard"));
	mCamera = &world.GetService<H::CameraService>()->GetActiveCamera();
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mLightServicePtr = world.GetService<H::LightService>();
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
	Matrix4 vm = mCamera->GetViewMatrix();
	Matrix4 pm = mCamera->GetPerspectiveMatrix();
	Matrix4 world = mTransformComponent->computeTransform();
	mRenderMaterial->material.SetTransform(TransformData{ H::Math::Transpose(world) ,H::Math::Transpose(world * vm * pm) ,mCamera->GetPosition() });
	mRenderMaterial->material.SetCamera(*mCamera);
	mRenderMaterial->material.SetLight(mLightServicePtr->GetDirectionalLightEntry("Directional Light").dirLight);

	mRenderMaterial->material.Bind();
	mMeshBuffer.Render();
	mRenderMaterial->material.UnBind();
}

void H::MeshRenderer::DebugUI()
{
	if (ImGui::TreeNode("MeshRenderer"))
	{
		ImGui::PushID("hms##");
		ImGui::Text("Mesh: "); 
		ImGui::SameLine();
		ImGui::Text(mMesh->name.c_str());
		ImGui::SameLine();
		if (ImGui::ArrowButton("select",ImGuiDir{ ImGuiDir_::ImGuiDir_Right}))
		{
			ImGui::OpenPopup("MeshSelect");
		}

		ImGui::Separator();
		if (ImGui::BeginPopup("MeshSelect"))
		{
			if (ImGui::BeginChild("setmesh", ImVec2{ 50.0f,200.0f }, true))
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
			ImGui::EndPopup();
		}
		mRenderMaterial->material.DebugUI();
		
		ImGui::PopID();
		ImGui::TreePop();
	}

}

void H::MeshRenderer::SetMesh(const H::MeshService::MeshEntry& mesh)
{
	mMesh = &mesh;
	mMeshBuffer.Terminate();
	mMeshBuffer.Initialize(mMesh->mesh, false);
}

void H::MeshRenderer::SetRenderMaterial(H::RenderMaterialService::RenderMaterialEntry& material)
{
	mRenderMaterial = &material;
}

void H::MeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}
 