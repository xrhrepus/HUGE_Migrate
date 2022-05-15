#include "C_MeshRenderer.h"
META_DERIVED_BEGIN(C_MeshRenderer, Component)
META_FIELD_BEGIN
//META_FIELD(mTimeElapsed, "timeElapsed")
META_FIELD_END
META_CLASS_END


void C_MeshRenderer::Initialize()
{
	mShaderEffect =
		ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
	mMeshBuffer.Initialize(mesh, true);

}

void C_MeshRenderer::Terminate()
{
}

void C_MeshRenderer::Update(float deltatime)
{
}

void C_MeshRenderer::Render()
{
	//set values
	RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	mShaderEffect->SetValue_SamplerType("LinearWarp");


	mShaderEffect->SetValue_TransformBuffer(transformData);
	mShaderEffect->SetValue_OptionsBuffer(optionsData);

	mShaderEffect->SetValue_MaterialBuffer(material);
	mShaderEffect->SetValue_DiffuseMap(diffuse);
	mShaderEffect->SetValue_NormalMap(normal);

	mMeshBuffer.Render();

}

void C_MeshRenderer::DebugUI()
{
	if (ImGui::CollapsingHeader("C_MeshRenderer"))
	{
		if (ImGui::TreeNode("Material##C_MeshRenderer"))
		{
			ImGui::ColorEdit4("Ambient##Material", &material.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material", &material.diffuse.r);
			ImGui::ColorEdit4("Specular##Material", &material.specular.r);
			ImGui::DragFloat("Power##Material", &material.power, 1.0f, 0.0f, 100.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GeoMeshes##C_MeshRenderer"))
		{
			bool meshChanged = false;
			if (ImGui::TreeNode("Sphere##C_MeshRenderer"))
			{
				static float i_sphr, i_sphs;
				ImGui::DragFloat("radius##Sphere", &i_sphr);
				ImGui::DragFloat("side##Sphere", &i_sphs, 1.0f, 1.0f, 10.0f);
				if (ImGui::Button("Create##Sphere"))
				{
					mesh = MeshBuilder::CreateShpere(i_sphr, i_sphs, { 0.0f });
					meshChanged = true;
				}
				ImGui::TreePop();

			}

			if (ImGui::TreeNode("Cube##CRender"))
			{
				static float i_sph_x, i_sph_y, i_sph_z;

				ImGui::DragFloat("x##Cube", &i_sph_x, 0.2f);
				ImGui::DragFloat("y##Cube", &i_sph_y, 0.2f);
				ImGui::DragFloat("z##Cube", &i_sph_z, 0.2f);
				if (ImGui::Button("Create##Cube"))
				{
					mesh = MeshBuilder::CreateCube(i_sph_x, i_sph_y, i_sph_z, { 0.0f });
					meshChanged = true;
				}
				ImGui::TreePop();
			}


			if (meshChanged)
			{
				//	mMeshBuffer.Update(mesh.vertices.data(), static_cast<uint32_t>(mesh.vertices.size()));
				mMeshBuffer.Update(mesh.vertices.data(), static_cast<uint32_t>(mesh.vertices.size()), mesh.indices.data(), static_cast<uint32_t>(mesh.indices.size()));
			}
			ImGui::TreePop();

		}

	}

}

