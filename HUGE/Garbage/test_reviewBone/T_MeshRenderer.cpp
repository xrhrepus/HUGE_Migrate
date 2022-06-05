#include "T_MeshRenderer.h"
#include "T_Components.h"
#include "T_Scene.h"
#pragma region Abstract render material
void T_Material::Bind() const
{
	mShaderEffect->SetContextInfo(*mShaderEffectContext);
	mShaderEffect->Bind();
}

void T_Material::UnBind() const
{
	mShaderEffect->UnBind();
}
void T_Material::SetTransform(const TransformData & data)
{
	//do nothing default
}
void T_Material::SetCamera(const Camera & cam)
{
	//do nothing default
}
void T_Material::SetLight(const DirectionalLight & light)
{
	//do nothing default
}
void T_Material::SetScene(const T_Scene& scene)
{
	mScene = &scene;
}
void T_Material::SetShaderContextDebugUIFunc(std::function<void()>&& func)
{
	debugUICallBack = func;
}
#pragma endregion

#pragma region Render mat

void Standard_Material::Init()
{
	mShaderEffect = (&ShaderEffectManager::Get()->GetEffect("Standard"));
	mShaderEffectContext = mShaderEffect->CreateShaderEffectContext();
	SetShaderContextDebugUIFunc(mShaderEffectContext->CreateDebugUI());
}

void Standard_Material::SetTransform(const TransformData& data)
{
	mShaderEffectContext->transformData = data;
}

void Standard_Material::SetCamera(const Camera& cam)
{
	mShaderEffectContext->camera = &cam;
}

void Standard_Material::SetLight(const DirectionalLight& light)
{
	static_cast<ShaderEffect_Standard::SE_Context_Standard&>(*mShaderEffectContext).directionalLight = light;
}

void Standard_Material::SetDiffuseTexture(TextureId tid)
{
	static_cast<ShaderEffect_Standard::SE_Context_Standard&>(*mShaderEffectContext).diffuse = tid;
}

void Standard_Material::DebugUI()
{
	ShaderEffect_Standard::SE_Context_Standard& ctx = static_cast<ShaderEffect_Standard::SE_Context_Standard&>(*mShaderEffectContext);

	if (ImGui::TreeNode("testsd"))
	{
		debugUICallBack();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Render Material"))
	{
		//copy from context debugui to get resource from Tscene
		// imgui attribute
		static bool disable_mouse_wheel = false;
		static bool disable_menu = false;
		ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("StandardEffect", ImVec2(0, 260), true, window_flags);
		// imgui attribute
		if (ImGui::BeginTabBar("StandardEffect##bar", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Material color"))
			{
				ImGui::ColorEdit3("Diffuse color", &(ctx.material.diffuse.x));
				ImGui::ColorEdit3("Ambient color", &(ctx.material.ambient.x));
				ImGui::ColorEdit3("Specular color", &(ctx.material.specular.x));
				ImGui::DragFloat("Power", &(ctx.material.power), 0.1f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Settings"))
			{
				ImGui::DragFloat("Brightness", &(ctx.settings.brightness), 0.1f);
				ImGui::DragFloat("normalMapWeight", &(ctx.settings.normalMapWeight), 0.1f);
				ImGui::DragFloat("specularMapWeight", &(ctx.settings.specularMapWeight), 0.1f);
				ImGui::DragFloat("aoMapWeight", &(ctx.settings.aoMapWeight), 0.1f);
				ImGui::EndTabItem();

			}
			if (ImGui::BeginTabItem("Textures"))
			{
				if (mScene)
				{
					static const ImVec2 size = { 50.0f,50.0f };
					if (ctx.diffuse != 0)
					{
						ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(ctx.diffuse)), size);
					}
					else
					{
						ImGui::Button("None", size);
					}
					ImGui::SameLine();
					ImGui::Text("Diffuse");

					if (ImGui::Button("Edit##diff"))
					{
						ImGui::OpenPopup("popupDiff");

					}
					if (ImGui::BeginPopup("popupDiff"))
					{
						mScene->ForEachTexture([&ctx](const std::string& name, const TextureId& tid)
							{
								ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tid)), size);
								ImGui::SameLine();
								if (ImGui::Selectable(name.c_str()))
								{
									ctx.diffuse = tid;
								}
							});

						ImGui::EndPopup();
					}
					ImGui::Separator();

					//
					if (ctx.normal != 0)
					{
						ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(ctx.normal)), size);
					}
					else
					{
						ImGui::Button("None", size);
					}
					ImGui::SameLine();
					ImGui::Text("Normal");
					if (ImGui::Button("Edit##norm"))
					{
						ImGui::OpenPopup("popupNorm");

					}
					if (ImGui::BeginPopup("popupNorm"))
					{
						mScene->ForEachTexture([&ctx](const std::string& name, const TextureId& tid)
							{
								ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tid)), size);
								ImGui::SameLine();
								if (ImGui::Selectable(name.c_str()))
								{
									ctx.normal = tid;
								}
							});

						ImGui::EndPopup();
					}
					ImGui::Separator();

					//
					if (ctx.specular != 0)
					{
						ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(ctx.specular)), size);
					}
					else
					{
						ImGui::Button("None", size);
					}
					ImGui::SameLine();
					ImGui::Text("Specular");
					if (ImGui::Button("Edit##spec"))
					{
						ImGui::OpenPopup("popupSpec");

					}
					if (ImGui::BeginPopup("popupSpec"))
					{
						mScene->ForEachTexture([&ctx](const std::string& name, const TextureId& tid)
							{
								ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tid)), size);
								ImGui::SameLine();
								if (ImGui::Selectable(name.c_str()))
								{
									ctx.specular = tid;
								}
							});

						ImGui::EndPopup();
					}
					ImGui::Separator();

					//
					if (ctx.ao != 0)
					{
						ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(ctx.ao)), size);
					}
					else
					{
						ImGui::Button("None", size);
					}
					ImGui::SameLine();
					ImGui::Text("AO");
					if (ImGui::Button("Edit##ao"))
					{
						ImGui::OpenPopup("popupAO");

					}
					if (ImGui::BeginPopup("popupAO"))
					{
						mScene->ForEachTexture([&ctx](const std::string& name, const TextureId& tid)
							{
								ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tid)), size);
								ImGui::SameLine();
								if (ImGui::Selectable(name.c_str()))
								{
									ctx.ao = tid;
								}
							});

						ImGui::EndPopup();
					}
					ImGui::Separator();

				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();



		ImGui::TreePop();
	}
}

#pragma endregion

#pragma region Mesh renderer
void T_MeshRenderer::SetScene(const T_Scene& scene)
{
	mScene = &scene;
	mMat->SetScene(scene);
}
void T_MeshRenderer::SetCamera(const Camera& cam)
{
	mCurrentCam = &cam;
}

void T_MeshRenderer::SetMesh(const Mesh& mesh)
{
	mMesh = &mesh;
	mMeshBuffer.Initialize(*mMesh);
}

void T_MeshRenderer::SetMaterial(T_Material& mat)
{
	mMat = &mat;
}

void T_MeshRenderer::SetLight(T_Light& light)
{
	mLight = &light;
}

void T_MeshRenderer::SetTransform(T_Transform& transform)
{
	mTransform = &transform;
}

void T_MeshRenderer::PreRender()
{
	Matrix4 vm = mCurrentCam->GetViewMatrix();
	Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	Matrix4 world = mTransform->computeTransform();
	mMat->SetTransform(TransformData{ H::Math::Transpose(world) ,H::Math::Transpose(world * vm * pm) ,mCurrentCam->GetPosition() });
	mMat->SetCamera(*mCurrentCam);
	mMat->SetLight(mLight->light);
	mMat->Bind();
}

void T_MeshRenderer::Render() const
{
	mMeshBuffer.Render();
}

void T_MeshRenderer::PostRender()
{
	mMat->UnBind();
}

void T_MeshRenderer::DebugUI()
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
		mLight->DebugUI();
		mTransform->DebugUI();
		mMat->DebugUI();
		ImGui::TreePop();
	}
}

#pragma endregion

