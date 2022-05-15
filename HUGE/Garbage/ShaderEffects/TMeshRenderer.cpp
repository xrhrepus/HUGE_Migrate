#include "TMeshRenderer.h"
 
using namespace H;

 
void H::TMeshRenderer::Initialize()
{
	if (useStandard)
	{
		mShaderEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
		mShaderEffectContext = &mStandardContext;
	}
	else
	{
		mShaderEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Toon>("Toon");
		mShaderEffectContext = &mToonContext;
	}

 
  }
 
void H::TMeshRenderer::Terminate()
{
 
 }

void H::TMeshRenderer::Update(float deltatime)
{
}

void H::TMeshRenderer::Render()
{
 
	mShaderEffect->Render(mShaderEffectContext);
 }

void H::TMeshRenderer::DebugUI()
{
	if (ImGui::CollapsingHeader("H_MeshRenderer"))
	{
		if (ImGui::Checkbox("use standard", &useStandard))
		{
			if (useStandard)
			{
				mShaderEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Standard>("Standard");
				mShaderEffectContext = &mStandardContext;
			}
			else
			{
				mShaderEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Toon>("Toon");
				mShaderEffectContext = &mToonContext;
			}
 		}

		if (useStandard)
		{
			if (ImGui::TreeNode("StandardEffect"))
			{
				if (ImGui::TreeNode("Light##H_MeshRenderer##StandardEffect"))
				{
					bool directionChanged = false;
					directionChanged |= ImGui::DragFloat("Direction X##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.direction.x, 0.25f, -1.0f, 1.0f);
					directionChanged |= ImGui::DragFloat("Direction Y##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.direction.y, 0.25f, -1.0f, 1.0f);
					directionChanged |= ImGui::DragFloat("Direction Z##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.direction.z, 0.25f, -1.0f, 1.0f);

					if (directionChanged)
					{
						mStandardContext.directionalLight.direction = Math::Normalize(mStandardContext.directionalLight.direction);
					}

					ImGui::ColorEdit4("Ambient##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.ambient.r);
					ImGui::ColorEdit4("Diffuse##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.diffuse.r);
					ImGui::ColorEdit4("Specular##Light##H_MeshRenderer##StandardEffect", &mStandardContext.directionalLight.specular.r);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Material##H_MeshRenderer##StandardEffect"))
				{
					ImGui::ColorEdit4("Ambient##Material##H_MeshRenderer##StandardEffect", &mStandardContext.material.ambient.r);
					ImGui::ColorEdit4("Diffuse##Material##H_MeshRenderer##StandardEffect", &mStandardContext.material.diffuse.r);
					ImGui::ColorEdit4("Specular##Material##H_MeshRenderer##StandardEffect", &mStandardContext.material.specular.r);
					ImGui::DragFloat("Power##Material##H_MeshRenderer##StandardEffect", &mStandardContext.material.power, 1.0f, 0.0f, 100.0f);

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Context##H_MeshRenderer##StandardEffect"))
				{
					ImGui::DragFloat("brightness##Context##H_MeshRenderer##StandardEffect", &mStandardContext.settings.brightness, 0.05f, 0.0f, 100.0f);
					ImGui::DragFloat("bumpmap weight##Context##H_MeshRenderer##StandardEffect", &mStandardContext.settings.bumpMapWeight, 0.05f, 0.0f, 100.0f);
					ImGui::TreePop();
				}


				ImGui::TreePop();
			}
 		}
		else
		{
			if (ImGui::TreeNode("ToonEffect"))
		{
			if (ImGui::TreeNode("Light##H_MeshRenderer##ToonEffect"))
			{
				bool directionChanged = false;
				directionChanged |= ImGui::DragFloat("Direction X##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.direction.x, 0.25f, -1.0f, 1.0f);
				directionChanged |= ImGui::DragFloat("Direction Y##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.direction.y, 0.25f, -1.0f, 1.0f);
				directionChanged |= ImGui::DragFloat("Direction Z##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.direction.z, 0.25f, -1.0f, 1.0f);

				if (directionChanged)
				{
					mToonContext.directionalLight.direction = Math::Normalize(mToonContext.directionalLight.direction);
				}

				ImGui::ColorEdit4("Ambient##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.ambient.r);
				ImGui::ColorEdit4("Diffuse##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.diffuse.r);
				ImGui::ColorEdit4("Specular##Light##H_MeshRenderer##ToonEffect", &mToonContext.directionalLight.specular.r);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Material##H_MeshRenderer##ToonEffect"))
			{
				ImGui::ColorEdit4("Ambient##Material##H_MeshRenderer##ToonEffect", &mToonContext.material.ambient.r);
				ImGui::ColorEdit4("Diffuse##Material##H_MeshRenderer##ToonEffect", &mToonContext.material.diffuse.r);
				ImGui::ColorEdit4("Specular##Material##H_MeshRenderer##ToonEffect", &mToonContext.material.specular.r);
				ImGui::DragFloat("Power##Material##H_MeshRenderer##ToonEffect", &mToonContext.material.power, 1.0f, 0.0f, 100.0f);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Context##H_MeshRenderer##ToonEffect"))
			{
				ImGui::DragFloat("brightness##Context##H_MeshRenderer##ToonEffect", &mToonContext.settings.brightness, 0.05f, 0.0f, 100.0f);
				ImGui::DragFloat("bumpmap weight##Context##H_MeshRenderer##ToonEffect", &mToonContext.settings.bumpMapWeight, 0.05f, 0.0f, 100.0f);
				ImGui::TreePop();
			}
			 
			ImGui::TreePop();
		}
		}

 
	}

}
 

void H::TMeshRenderer::SetContext(const ShaderEffect_Standard::SE_Context_Standard & context)
{
	mStandardContext = context;
}

void H::TMeshRenderer::SetContext(const ShaderEffect_Toon::SE_Context_Toon & context)
{
	mToonContext = context;
}
 