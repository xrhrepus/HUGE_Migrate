#include "TMeshRenderer.h"
using namespace H;

 
void H::TMeshRenderer::Initialize()
{
	mStandardShaderEffect =
		ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
	if (mMesh.vertices.size() == 0)
	{
		mMesh = MeshBuilder::CreateShpere(1.0f, 4, { 0.0f });
	}
	mMeshBuffer.Initialize(mMesh, true);
}
 
void H::TMeshRenderer::Terminate()
{
	mMeshBuffer.Terminate();
}

void H::TMeshRenderer::Update(float deltatime)
{
}

void H::TMeshRenderer::Render()
{
	mStandardShaderEffect->Bind();
	mStandardShaderEffect->SetContextInfo(&mStandardContext);
	mMeshBuffer.Render();
	mStandardShaderEffect->UnBind();
}

void H::TMeshRenderer::DebugUI()
{
	if (ImGui::CollapsingHeader("H_MeshRenderer"))
	{
		if (ImGui::TreeNode("Light##H_MeshRenderer"))
		{
			bool directionChanged = false;
			directionChanged |= ImGui::DragFloat("Direction X##Light##H_MeshRenderer", &mStandardContext.directionalLight.direction.x, 0.25f, -1.0f, 1.0f);
			directionChanged |= ImGui::DragFloat("Direction Y##Light##H_MeshRenderer", &mStandardContext.directionalLight.direction.y, 0.25f, -1.0f, 1.0f);
			directionChanged |= ImGui::DragFloat("Direction Z##Light##H_MeshRenderer", &mStandardContext.directionalLight.direction.z, 0.25f, -1.0f, 1.0f);

			if (directionChanged)
			{
				mStandardContext.directionalLight.direction = Math::Normalize(mStandardContext.directionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light",  &mStandardContext.directionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light",  &mStandardContext.directionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mStandardContext.directionalLight.specular.r);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Material##H_MeshRenderer"))
		{
			ImGui::ColorEdit4("Ambient##Material##H_MeshRenderer", &mStandardContext.material.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material##H_MeshRenderer", &mStandardContext.material.diffuse.r);
			ImGui::ColorEdit4("Specular##Material##H_MeshRenderer", &mStandardContext.material.specular.r);
			ImGui::DragFloat("Power##Material##H_MeshRenderer", &mStandardContext.material.power, 1.0f, 0.0f, 100.0f);

			ImGui::TreePop();
		}

 
	}

}

void H::TMeshRenderer::SetMesh(const Mesh & mesh)
{
	mMesh = mesh;
}

void H::TMeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}
 