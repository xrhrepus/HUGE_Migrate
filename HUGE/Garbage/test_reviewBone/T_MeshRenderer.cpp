#include "T_MeshRenderer.h"

#pragma region Render mat
T_Material::T_Material()
{
	mShaderEffect = (&ShaderEffectManager::Get()->GetEffect("Standard"));
	mShaderEffectContext = mShaderEffect->CreateShaderEffectContext();
}

void T_Material::BindShader() const
{
	mShaderEffect->SetContextInfo(*mShaderEffectContext);
	mShaderEffect->Bind();
}

void T_Material::UnBindShader() const
{
	mShaderEffect->UnBind();
}

void T_Material::DebugUI()
{
	if (ImGui::CollapsingHeader("Render Material"))
	{
		mShaderEffectContext->DebugUI();
	}
}

#pragma endregion

#pragma region Mesh renderer
T_MeshRenderer::T_MeshRenderer(const Mesh& mesh)
	:mMesh(mesh)
{
	mMeshBuffer.Initialize(mMesh);
}

void T_MeshRenderer::PreRender() const
{
	mMat.BindShader();
}

void T_MeshRenderer::Render() const
{
	mMeshBuffer.Render();
}

void T_MeshRenderer::PostRender() const
{
	mMat.UnBindShader();
}

void T_MeshRenderer::DebugUI()
{
	ImGui::Begin("Mesh Renderer");
	mMat.DebugUI();
	ImGui::End();
}

#pragma endregion

