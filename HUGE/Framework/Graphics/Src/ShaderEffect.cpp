#include "Precompiled.h"
#include "ShaderEffect.h"
#include "SamplerManager.h"
#include "D3DUtil.h"
#include "DepthStencilManager.h"
#include <format>

using namespace H::Graphics;
namespace H::Graphics::ShaderEffectHelpers
{
	void ConstantBuffer_UnBindVS(uint32_t slot) 
	{
		H::Graphics::GetContext()->VSSetConstantBuffers(slot, 0, nullptr);
	}

	void ConstantBuffer_UnBindPS(uint32_t slot) 
	{
		H::Graphics::GetContext()->PSSetConstantBuffers(slot, 0, nullptr);
	}
	void ConstantBuffer_UnBindGS(uint32_t slot) 
	{
		H::Graphics::GetContext()->GSSetConstantBuffers(slot, 0, nullptr);
	}

	void ShaderResource_UnBindVS(uint32_t slot)
	{
		H::Graphics::GetContext()->VSSetShaderResources(slot, 0, nullptr);
	}

	void ShaderResource_UnBindPS(uint32_t slot)
	{
		H::Graphics::GetContext()->PSSetShaderResources(slot, 0, nullptr);
	}

}

#pragma region Abstract 
void H::Graphics::ShaderEffect::Initialize()
{
	mVertexShader.Initialize(DEFAULT_FILE_PATH.c_str());
	mPixelShader.Initialize(DEFAULT_FILE_PATH.c_str(), "PS");
	InitBuffers();
}

void H::Graphics::ShaderEffect::Initialize(std::filesystem::path file)
{
	mVertexShader.Initialize(file.c_str());
	mPixelShader.Initialize(file.c_str(), "PS");
	InitBuffers();

}

void H::Graphics::ShaderEffect::Bind() const
{
	Bind_VS_and_PS();
	Bind_Other_Buffers();

}

void H::Graphics::ShaderEffect::UnBind() const
{
	mVertexShader.UnBind();
	mPixelShader.UnBind();
	UnBind_Other_Buffers();
}

void H::Graphics::ShaderEffect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	TerminateBuffers();
}

std::string H::Graphics::ShaderEffect::GetShaderFilePath()
{
	return DEFAULT_FILE_PATH.u8string();
}

void H::Graphics::ShaderEffect::Bind_VS_and_PS() const
{
	mVertexShader.Bind();
	mPixelShader.Bind();
}

#pragma endregion

//#pragma region ToonShading
//H::Graphics::ShaderEffect_Toon::ShaderEffect_Toon()
//{
//	ShaderEffect::DEFAULT_FILE_PATH = ShaderEffect_Toon::DEFAULT_FILE_PATH;
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_TransformBuffer(const TransformData & tdata) const
//{
//	mTransformBuf->Set(tdata);
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_LightBuffer(const DirectionalLight & ldata) const
//{
//	mLightBuf->Set(ldata);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_MaterialBuffer(const Material & mdata) const
//{
//	mMaterialBuf->Set(mdata);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_SettingsBuffer(const Settings & settings) const
//{
//	mSettingsBuf->Set(settings);
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_ShadowBuffer(const ShadowData & shadow) const
//{
//	mShadowDataBuf->Set(shadow);
//}
//
//
//
//void H::Graphics::ShaderEffect_Toon::SetValue_SamplerType(const std::string samplerType) const
//{
//	SamplerManager::Get()->GetSampler(samplerType)->BindPS(0);
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_DiffuseMap(TextureId tid) const
//{
//	//mDiffuseMap = tid;
//	TextureManager::Get()->GetTexture(tid)->BindPS(0);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_SpecularMap(TextureId tid) const
//{
//	//mSpecularMap = tid;
//	TextureManager::Get()->GetTexture(tid)->BindPS(1);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_DisplacementMap(TextureId tid) const
//{
//	//mDisplacementMap = tid;
//	TextureManager::Get()->GetTexture(tid)->BindVS(2);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_NormalMap(TextureId tid) const
//{
//	//mNormalMap = tid;
//	TextureManager::Get()->GetTexture(tid)->BindPS(3);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_AoMap(TextureId tid) const
//{
//	TextureManager::Get()->GetTexture(tid)->BindPS(4);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_DepthMap(TextureId tid) const
//{
//	TextureManager::Get()->GetTexture(tid)->BindPS(5);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::SetValue_RampMap(TextureId tid) const
//{
//	TextureManager::Get()->GetTexture(tid)->BindPS(6);
//}
//
//void H::Graphics::ShaderEffect_Toon::InitBuffers()
//{
//	mTransformBuf = std::make_unique<H::Graphics::TransformBuffer>();
//	mLightBuf = std::make_unique<H::Graphics::LightBuffer>();
//	mMaterialBuf = std::make_unique<H::Graphics::MaterialBuffer>();
//	mSettingsBuf = std::make_unique<SettingsBuffer>();
//	mShadowDataBuf = std::make_unique<ShadowDataBuffer>();
//
//
//
//	mTransformBuf->Initialize();
//	mLightBuf->Initialize();
//	mMaterialBuf->Initialize();
//	mSettingsBuf->Initialize();
//	mShadowDataBuf->Initialize();
//
//}
//
//void H::Graphics::ShaderEffect_Toon::TerminateBuffers()
//{
//	mTransformBuf->Terminate();
//	mLightBuf->Terminate();
//	mMaterialBuf->Terminate();
//	mSettingsBuf->Terminate();
//	mShadowDataBuf->Terminate();
//
//}
//
//void H::Graphics::ShaderEffect_Toon::Bind_Other_Buffers() const
//{
//	//constant buffers
//	mTransformBuf->BindVS(0);
//	mTransformBuf->BindPS(0);
//	mLightBuf->BindVS(1);
//	mLightBuf->BindPS(1);
//	mMaterialBuf->BindVS(2);
//	mMaterialBuf->BindPS(2);
//	mSettingsBuf->BindVS(3);
//	mSettingsBuf->BindPS(3);
//	mShadowDataBuf->BindVS(4);
//	mShadowDataBuf->BindPS(4);
//
//	if (mContext->diffuse != 0)
//		SetValue_DiffuseMap(mContext->diffuse);
//	if (mContext->specular != 0)
//		SetValue_SpecularMap(mContext->specular);
//	if (mContext->dispalcement != 0)
//		SetValue_DisplacementMap(mContext->dispalcement);
//	if (mContext->normal != 0)
//		SetValue_NormalMap(mContext->normal);
//	if (mContext->ao != 0)
//		SetValue_AoMap(mContext->ao);
//	if (mContext->depth != 0)
//		SetValue_DepthMap(mContext->depth);
//	if (mContext->ramp != 0)
//		SetValue_RampMap(mContext->ramp);
//
//}
//
//void H::Graphics::ShaderEffect_Toon::UnBind_Other_Buffers() const
//{
//	ConstantBuffer_UnBindVS(0);
//	ConstantBuffer_UnBindPS(0);
//	ConstantBuffer_UnBindVS(1);
//	ConstantBuffer_UnBindPS(1);
//	ConstantBuffer_UnBindVS(2);
//	ConstantBuffer_UnBindPS(2);
//	ConstantBuffer_UnBindVS(3);
//	ConstantBuffer_UnBindPS(3);
//	ConstantBuffer_UnBindVS(4);
//	ConstantBuffer_UnBindPS(4);
//
//
//	ShaderResource_UnBindPS(0);
//	ShaderResource_UnBindPS(1);
//	ShaderResource_UnBindPS(2);
//	ShaderResource_UnBindPS(3);
//	ShaderResource_UnBindPS(4);
//	ShaderResource_UnBindPS(5);
//	ShaderResource_UnBindPS(6);
//
//	//mSampler->UnBindPS(0);
//}
//void H::Graphics::ShaderEffect_Toon::SetContextInfo(ShaderEffectContext * context)
//{
//	mContext = static_cast<SE_Context_Toon*>(context);
//
//	SetValue_TransformBuffer(mContext->transformData);
//	SetValue_LightBuffer(mContext->directionalLight);
//	SetValue_MaterialBuffer(mContext->material);
//	SetValue_SettingsBuffer(mContext->settings);
//	SetValue_ShadowBuffer(mContext->shadow);
//
//
//}
//void H::Graphics::ShaderEffect_Toon::Render(ShaderEffectContext * context)
//{
// 
//	SetContextInfo(context);
//	Bind();
//	mContext->meshBuffer->Render();
//	UnBind();
//}
//
//#pragma endregion
//
//
//#pragma region Skinning
//
//
//H::Graphics::ShaderEffect_Skinning::ShaderEffect_Skinning()
//{
//	ShaderEffect::DEFAULT_FILE_PATH = ShaderEffect_Skinning::DEFAULT_FILE_PATH;
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_TransformBuffer(const TransformData & tdata)const
//{
//	mTransformBuf->Set(tdata);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_LightBuffer(const DirectionalLight & ldata)const
//{
//	mLightBuf->Set(ldata);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_MaterialBuffer(const Material & mdata)const
//{
//	mMaterialBuf->Set(mdata);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_OptionsBuffer(const OptionsData & odata)const
//{
//	mOptionsBuf->Set(odata);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_BoneTransformBuffer(const BoneTransformData & btdata)const
//{
//	mBoneTransformBuf->Set(btdata);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_SamplerType(const std::string samplerType)const
//{
//	SamplerManager::Get()->GetSampler(samplerType)->BindPS(0);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_DiffuseMap(H::Graphics::TextureId tid)const
//{
// 	TextureManager::Get()->GetTexture(tid)->BindPS(0);
//
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_SpecularMap(H::Graphics::TextureId tid)const
//{
// 	TextureManager::Get()->GetTexture(tid)->BindPS(1);
//
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_DisplacementMap(H::Graphics::TextureId tid)const
//{
// 	TextureManager::Get()->GetTexture(tid)->BindVS(2);
//}
//
//void H::Graphics::ShaderEffect_Skinning::SetValue_NormalMap(H::Graphics::TextureId tid)const
//{
// 	TextureManager::Get()->GetTexture(tid)->BindPS(3);
// }
//
//void H::Graphics::ShaderEffect_Skinning::InitBuffers()
//{
//	mTransformBuf = std::make_unique<H::Graphics::TransformBuffer>();
//	mLightBuf = std::make_unique<H::Graphics::LightBuffer>();
//	mMaterialBuf = std::make_unique<H::Graphics::MaterialBuffer>();
//	mOptionsBuf = std::make_unique<H::Graphics::OptionsBuffer>();
//	mBoneTransformBuf = std::make_unique<H::Graphics::BoneTransformBuffer>();
//
//	mTransformBuf->Initialize();
//	mLightBuf->Initialize();
//	mMaterialBuf->Initialize();
//	mOptionsBuf->Initialize();
//	mBoneTransformBuf->Initialize();
//}
//
//void H::Graphics::ShaderEffect_Skinning::TerminateBuffers()
//{
//	mTransformBuf->Terminate();
//	mLightBuf->Terminate();
//	mMaterialBuf->Terminate();
//	mOptionsBuf->Terminate();
//	mBoneTransformBuf->Terminate();
//}
//
//void H::Graphics::ShaderEffect_Skinning::Bind_Other_Buffers() const
//{
//	mTransformBuf->BindVS(0);
//	mTransformBuf->BindPS(0);
//	mLightBuf->BindVS(1);
//	mLightBuf->BindPS(1);
//	mMaterialBuf->BindVS(2);
//	mMaterialBuf->BindPS(2);
//	mOptionsBuf->BindVS(3);
//	mOptionsBuf->BindPS(3);
//	mBoneTransformBuf->BindVS(4);
//	mBoneTransformBuf->BindPS(4);
//
//	if (mContext->diffuse != 0)
//		SetValue_DiffuseMap(mContext->diffuse);
//	if (mContext->specular != 0)
//		SetValue_SpecularMap(mContext->specular);
//	if (mContext->dispalcement != 0)
//		SetValue_DisplacementMap(mContext->dispalcement);
//	if (mContext->normal != 0)
//		SetValue_NormalMap(mContext->normal);
//
//}
//
//void H::Graphics::ShaderEffect_Skinning::UnBind_Other_Buffers() const
//{
//	ConstantBuffer_UnBindVS(0);
//	ConstantBuffer_UnBindPS(0);
//	ConstantBuffer_UnBindVS(1);
//	ConstantBuffer_UnBindPS(1);
//	ConstantBuffer_UnBindVS(2);
//	ConstantBuffer_UnBindPS(2);
//	ConstantBuffer_UnBindVS(3);
//	ConstantBuffer_UnBindPS(3);
//	ConstantBuffer_UnBindVS(4);
//	ConstantBuffer_UnBindPS(4);
//
//
//	ShaderResource_UnBindPS(0);
//	ShaderResource_UnBindPS(1);
//	ShaderResource_UnBindPS(2);
//	ShaderResource_UnBindPS(3);
// 
// 
//	//mTransformBuf->UnBindVS(0);
//	//mTransformBuf->UnBindPS(0);
//	//mLightBuf->UnBindVS(1);
//	//mLightBuf->UnBindPS(1);
//	//mMaterialBuf->UnBindVS(2);
//	//mMaterialBuf->UnBindPS(2);
//	//mOptionsBuf->UnBindVS(3);
//	//mOptionsBuf->UnBindPS(3);
//	//mBoneTransformBuf->UnBindVS(4);
//	//mBoneTransformBuf->UnBindPS(4);
//
//	//auto txMngr = TextureManager::Get();
//
//	//txMngr->GetTexture(mDiffuseMap)->UnBindPS(0);
//	//txMngr->GetTexture(mSpecularMap)->UnBindPS(1);
//	//txMngr->GetTexture(mDisplacementMap)->UnBindVS(2);
//	//txMngr->GetTexture(mNormalMap)->UnBindPS(3);
//
//	//mSampler->UnBindPS(0);
//}
//void H::Graphics::ShaderEffect_Skinning::SetContextInfo(ShaderEffectContext * context)
//{
//	mContext = static_cast<SE_Context_Skinning*>(context);
//
//	SetValue_TransformBuffer(mContext->transformData);
//	SetValue_LightBuffer(mContext->directionalLight);
//	SetValue_MaterialBuffer(mContext->material);
//	SetValue_OptionsBuffer(mContext->settings);
//	SetValue_BoneTransformBuffer(mContext->boneTransform);
//
// 
//}
//void H::Graphics::ShaderEffect_Skinning::Render(ShaderEffectContext * context)
//{
//	SetContextInfo(context);
//	Bind();
//	mContext->meshBuffer->Render();
//	UnBind();
//
//}
// #pragma endregion

ShaderData_Vector1::ShaderData_Vector1(ShaderEffectContext& context, const std::string& name, float value)
	: data(value)
{
	context._registerFloat({ name, &data });
}

float H::Graphics::ShaderData_Vector1::operator=(float rhs)
{
	data = rhs;
	return data;
}

void H::Graphics::ShaderEffectContext::CreateVec1DebugUI(const SD_Vec1_Ref& vec1Ref) const
{
	ImGui::DragFloat(vec1Ref.first.c_str(), vec1Ref.second, 0.1f);
}

void H::Graphics::ShaderEffectContext::CreateVec4DebugUI(const SD_Vec4_Ref& ref) const
{
	const char* name = ref.first.c_str();
	ImGui::PushID(name);
	ImGui::DragFloat4(name, &ref.second->x, 0.1f);
	ImGui::SameLine();
	if (ImGui::ArrowButton("colorpicker", ImGuiDir{ ImGuiDir_::ImGuiDir_Right }))
	{
		ImGui::OpenPopup("colorpickerpopup");
	}
	if (ImGui::BeginPopup("colorpickerpopup"))
	{
		ImGui::ColorPicker4(name, &ref.second->x);
		ImGui::EndPopup();
	}
	ImGui::PopID();
}

ShaderData_TextureId::ShaderData_TextureId(ShaderEffectContext& context, const std::string& name, TextureId value)
	: data(value)
{
	context._registerTextureId({ name, &data });
}
TextureId ShaderData_TextureId::operator=(TextureId rhs)
{
	data = rhs;
	return data;
}

bool H::Graphics::ShaderData_TextureId::operator==(TextureId rhs)
{
	return data == rhs;
}

bool H::Graphics::ShaderData_TextureId::operator!=(TextureId rhs)
{
	return data != rhs;
}

H::Graphics::ShaderData_TextureId::operator TextureId() const
{
	return data;
}


void H::Graphics::ShaderEffectContext::CreateTextureIdDebugUI(const SD_TextureId_Ref& ref) const
{
	static const ImVec2 size = { 50.0f,50.0f };

	// current
	if (*ref.second != 0)
	{
		ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(*ref.second)), size);
	}
	else
	{
		ImGui::Button("None", size);
	}

	ImGui::SameLine();
	const char* name = ref.first.c_str();
	ImGui::Text(name);

	// edit
	ImGui::PushID(name);
	if (ImGui::Button("Edit"))
	{
		ImGui::OpenPopup("popupTexEdit");
	}

	if (ImGui::BeginPopup("popupTexEdit"))
	{
		const int itemInSameRow = 5;
		int count = 0;
		TextureManager::Get()->ForEachTextureId([&ref,&count, itemInSameRow](const TextureId& tid)
			{
				if (ImGui::ImageButton(GetShaderResourceView(*TextureManager::Get()->GetTexture(tid)), size))
				{
					*ref.second = tid;
				}
				if (++count % itemInSameRow + 1 < itemInSameRow)
				{
					ImGui::SameLine();
				}
			});
		ImGui::EndPopup();
	}
	ImGui::PopID();
}

void H::Graphics::ShaderEffectContext::CreateBooleanDebugUI(const SD_Boolean_Ref& ref) const
{
	ImGui::Checkbox(ref.first.c_str(), ref.second);
}

std::function<void()> H::Graphics::ShaderEffectContext::CreateDebugUI() const
{
	return [this]() {
		ImGui::PushID("HUGE Standard");
		if (ImGui::TreeNode("Settings"))
		{
			for (const auto& v1 : this->mFloats)
			{
				CreateVec1DebugUI(v1);
			}
			for (const auto& v4 : this->mVec4s)
			{
				CreateVec4DebugUI(v4);
			}
			for (const auto& b : this->mBooleans)
			{
				CreateBooleanDebugUI(b);
			}

			if (ImGui::TreeNode("Textures"))
			{
				for (const auto& tex : this->mTextureIds)
				{
					CreateTextureIdDebugUI(tex);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	};
}

void ShaderEffectContext::_registerFloat(const SD_Vec1_Ref& data)
{
	mFloats.emplace_back(data);
}
void ShaderEffectContext::_registerVec4(const SD_Vec4_Ref& data)
{
	mVec4s.emplace_back(data);
}
void ShaderEffectContext::_registerTextureId(const SD_TextureId_Ref& data) 
{
	mTextureIds.emplace_back(data);
}

void H::Graphics::ShaderEffectContext::_registerBoolean(const SD_Boolean_Ref& data)
{
	mBooleans.emplace_back(data);
}

H::Graphics::ShaderData_Vector4::ShaderData_Vector4(ShaderEffectContext& context, const std::string& name, const Vector4& value)
	:data(value)
{
	context._registerVec4({ name, &data });

}

Vector4& H::Graphics::ShaderData_Vector4::operator=(const Vector4& rhs)
{
	data = rhs;
	return data;
}

//bool H::Graphics::ShaderData_Vector4::operator==(const Vector4& rhs) const
//{
//	return data == rhs;
//}
//
//bool H::Graphics::ShaderData_Vector4::operator!=(const Vector4& rhs) const
//{
//	return false;
//}

H::Graphics::ShaderData_Boolean::ShaderData_Boolean(ShaderEffectContext& context, const std::string& name, bool value)
	:data(value)
{
	context._registerBoolean({ name, &data });
}

bool H::Graphics::ShaderData_Boolean::operator=(bool rhs)
{
	data = rhs;
	return data;
}

bool H::Graphics::ShaderData_Boolean::operator==(bool rhs)
{
	return data == rhs;
}

bool H::Graphics::ShaderData_Boolean::operator!=(bool rhs)
{
	return data != rhs;
}
