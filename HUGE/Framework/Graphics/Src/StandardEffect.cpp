#include "Precompiled.h"
#include <SamplerManager.h>
#include "D3DUtil.h"
#include <DepthStencilManager.h>
#include <format>
#include <StandardEffect.h>

using namespace H::Graphics;

#pragma region Standard


H::Graphics::ShaderEffect_Standard::ShaderEffect_Standard()
{
	ShaderEffect::DEFAULT_FILE_PATH = ShaderEffect_Standard::DEFAULT_FILE_PATH;
}

std::unique_ptr<H::Graphics::ShaderEffectContext> H::Graphics::ShaderEffect_Standard::CreateShaderEffectContext() const
{
	return std::make_unique<SE_Context_Standard>();
}

void H::Graphics::ShaderEffect_Standard::SetValue_TransformBuffer(const TransformData& tdata) const
{
	mTransformBuf->Set(tdata);
}

void H::Graphics::ShaderEffect_Standard::SetValue_LightBuffer(const DirectionalLight& ldata) const
{
	mLightBuf->Set(ldata);

}

void H::Graphics::ShaderEffect_Standard::SetValue_MaterialBuffer(const MaterialSettings& mdata) const
{
	mMaterialBuf->Set(mdata);

}

void H::Graphics::ShaderEffect_Standard::SetValue_SettingsBuffer(const Settings& settings) const
{
	mSettingsBuf->Set(settings);
}

void H::Graphics::ShaderEffect_Standard::SetValue_ShadowBuffer(const ShadowData& shadow) const
{
	mShadowDataBuf->Set(shadow);
}



void H::Graphics::ShaderEffect_Standard::SetValue_SamplerType(const std::string samplerType) const
{
	SamplerManager::Get()->GetSampler(samplerType)->BindPS(0);
}

void H::Graphics::ShaderEffect_Standard::SetValue_DiffuseMap(TextureId tid) const
{
	//mDiffuseMap = tid;
	TextureManager::Get()->GetTexture(tid)->BindPS(0);

}

void H::Graphics::ShaderEffect_Standard::SetValue_SpecularMap(TextureId tid) const
{
	//mSpecularMap = tid;
	TextureManager::Get()->GetTexture(tid)->BindPS(1);

}

void H::Graphics::ShaderEffect_Standard::SetValue_DisplacementMap(TextureId tid) const
{
	//mDisplacementMap = tid;
	TextureManager::Get()->GetTexture(tid)->BindVS(2);

}

void H::Graphics::ShaderEffect_Standard::SetValue_NormalMap(TextureId tid) const
{
	//mNormalMap = tid;
	TextureManager::Get()->GetTexture(tid)->BindPS(3);

}

void H::Graphics::ShaderEffect_Standard::SetValue_AoMap(TextureId tid) const
{
	TextureManager::Get()->GetTexture(tid)->BindPS(4);

}

void H::Graphics::ShaderEffect_Standard::SetValue_DepthMap(TextureId tid) const
{
	TextureManager::Get()->GetTexture(tid)->BindPS(5);

}


void H::Graphics::ShaderEffect_Standard::InitBuffers()
{
	mTransformBuf = std::make_unique<H::Graphics::TransformBuffer>();
	mLightBuf = std::make_unique<H::Graphics::LightBuffer>();
	mMaterialBuf = std::make_unique<MaterialSettingsBuffer>();
	mSettingsBuf = std::make_unique<SettingsBuffer>();
	mShadowDataBuf = std::make_unique<ShadowDataBuffer>();



	mTransformBuf->Initialize();
	mLightBuf->Initialize();
	mMaterialBuf->Initialize();
	mSettingsBuf->Initialize();
	mShadowDataBuf->Initialize();

}

void H::Graphics::ShaderEffect_Standard::TerminateBuffers()
{
	mTransformBuf->Terminate();
	mLightBuf->Terminate();
	mMaterialBuf->Terminate();
	mSettingsBuf->Terminate();
	mShadowDataBuf->Terminate();

}

void H::Graphics::ShaderEffect_Standard::Bind_Other_Buffers() const
{
	//constant buffers
	mTransformBuf->BindVS(0);
	mTransformBuf->BindPS(0);
	mLightBuf->BindVS(1);
	mLightBuf->BindPS(1);
	mMaterialBuf->BindVS(2);
	mMaterialBuf->BindPS(2);
	mSettingsBuf->BindVS(3);
	mSettingsBuf->BindPS(3);
	mShadowDataBuf->BindVS(4);
	mShadowDataBuf->BindPS(4);


}

void H::Graphics::ShaderEffect_Standard::UnBind_Other_Buffers() const
{
	ShaderEffectHelpers::ConstantBuffer_UnBindVS(0);
	ShaderEffectHelpers::ConstantBuffer_UnBindPS(0);
	ShaderEffectHelpers::ConstantBuffer_UnBindVS(1);
	ShaderEffectHelpers::ConstantBuffer_UnBindPS(1);
	ShaderEffectHelpers::ConstantBuffer_UnBindVS(2);
	ShaderEffectHelpers::ConstantBuffer_UnBindPS(2);
	ShaderEffectHelpers::ConstantBuffer_UnBindVS(3);
	ShaderEffectHelpers::ConstantBuffer_UnBindPS(3);
	ShaderEffectHelpers::ConstantBuffer_UnBindVS(4);
	ShaderEffectHelpers::ConstantBuffer_UnBindPS(4);


	ShaderEffectHelpers::ShaderResource_UnBindPS(0);
	ShaderEffectHelpers::ShaderResource_UnBindPS(1);
	ShaderEffectHelpers::ShaderResource_UnBindPS(2);
	ShaderEffectHelpers::ShaderResource_UnBindPS(3);
	ShaderEffectHelpers::ShaderResource_UnBindPS(4);
	ShaderEffectHelpers::ShaderResource_UnBindPS(5);

	//mSampler->UnBindPS(0);
}

void H::Graphics::ShaderEffect_Standard::SE_Context_Standard::DebugUI()
{
	//if (ImGui::TreeNode("StandardEffect"))
	//{
		// imgui attribute
		//static bool disable_mouse_wheel = false;
		//static bool disable_menu = false;
		//ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
		//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		//ImGui::BeginChild("StandardEffect", ImVec2(0, 260), true, window_flags);
		//// imgui attribute
		//if (ImGui::BeginTabBar("StandardEffect##bar", ImGuiTabBarFlags_None))
		//{
		//	if (ImGui::BeginTabItem("Material color"))
		//	{
		//		ImGui::ColorEdit3("Diffuse color", &(material.diffuse.x));
		//		ImGui::ColorEdit3("Ambient color", &(material.ambient.x));
		//		ImGui::ColorEdit3("Specular color", &(material.specular.x));
		//		ImGui::DragFloat("Power", &(material.power), 0.1f);
		//		ImGui::EndTabItem();
		//	}
		//	if (ImGui::BeginTabItem("Settings"))
		//	{
		//		ImGui::DragFloat("Brightness", &(settings.brightness), 0.1f);
		//		ImGui::EndTabItem();

		//	}
		//	if (ImGui::BeginTabItem("Textures"))
		//	{
		//		static const ImVec2 size = { 50.0f,50.0f };
		//		// display only for now
		//		if (diffuse != static_cast<TextureId>( 0 ))
		//		{
		//			ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(diffuse)), size);
		//		}
		//		else
		//		{
		//			ImGui::Button("None", size);
		//		}
		//		ImGui::SameLine();
		//		ImGui::Text("Diffuse");
		//		ImGui::Button("Edit##tex");
		//		ImGui::Separator();

		//		// display only for now
		//		if (normal != static_cast<TextureId>(0))
		//		{
		//			ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(normal)), size);
		//		}
		//		else
		//		{
		//			ImGui::Button("None", size);
		//		}
		//		ImGui::SameLine();
		//		ImGui::Text("Normal");
		//		ImGui::Button("Edit##tex");
		//		ImGui::Separator();

		//		// display only for now
		//		if (specular != static_cast<TextureId>(0))
		//		{
		//			ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(specular)), size);
		//		}
		//		else
		//		{
		//			ImGui::Button("None", size);
		//		}
		//		ImGui::SameLine();
		//		ImGui::Text("Specular");
		//		ImGui::Button("Edit##tex");
		//		ImGui::Separator();

		//		ImGui::EndTabItem();
		//	}
		//	ImGui::EndTabBar();
		//}

		//ImGui::EndChild();
		//ImGui::PopStyleVar();
}

void H::Graphics::ShaderEffect_Standard::SetContextInfo(const ShaderEffectContext& context) const
{
	SamplerManager::Get()->GetSampler("PointClamp")->BindPS(0);
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();

	const auto& stdContext = static_cast<const SE_Context_Standard&>(context);

	SetValue_TransformBuffer(stdContext.transformData);
	SetValue_LightBuffer(stdContext.directionalLight);
	SetValue_MaterialBuffer(stdContext.material);
	SetValue_SettingsBuffer(stdContext.settings);
	SetValue_ShadowBuffer(stdContext.shadow);

	if (stdContext.diffuse != 0)
	{
		SetValue_DiffuseMap(stdContext.diffuse);
	}
	if (stdContext.specular != 0)
	{
		SetValue_SpecularMap(stdContext.specular);
	}
	if (stdContext.dispalcement != 0)
	{
		SetValue_DisplacementMap(stdContext.dispalcement);
	}
	if (stdContext.normal != 0)
	{
		SetValue_NormalMap(stdContext.normal);
	}
	if (stdContext.ao != 0)
	{
		SetValue_AoMap(stdContext.ao);
	}
	if (stdContext.depth != 0)
	{
		SetValue_DepthMap(stdContext.depth);
	}

}

#pragma endregion
