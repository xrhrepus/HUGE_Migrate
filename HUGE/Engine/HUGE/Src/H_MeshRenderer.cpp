#include "Precompiled.h"
#include "H_MeshRenderer.h"
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
	mMeshBuffer.Initialize(mMesh, true);
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
	//if (ImGui::CollapsingHeader("H_MeshRenderer"))
	//{
	//	if (ImGui::TreeNode("Material##H_MeshRenderer"))
	//	{
	//		ImGui::ColorEdit4("Ambient##Material", &mStandardContext->material.ambient.r);
	//		ImGui::ColorEdit4("Diffuse##Material", &mStandardContext->material.diffuse.r);
	//		ImGui::ColorEdit4("Specular##Material", &mStandardContext.material.specular.r);
	//		ImGui::DragFloat("Power##Material", &mStandardContext.material.power, 1.0f, 0.0f, 100.0f);

	//		ImGui::TreePop();
	//	}

 //
	//}

}

void H::H_MeshRenderer::SetMesh(const Mesh & mesh)
{
	mMesh = mesh;
}

void H::H_MeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}
 