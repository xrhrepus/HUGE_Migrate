#include "Precompiled.h"
#include "ShaderEffect.h"
#include "SamplerManager.h"
#include "D3DUtil.h"

namespace 
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
#pragma region Standard


H::Graphics::ShaderEffect_Standard::ShaderEffect_Standard()
{
	ShaderEffect::DEFAULT_FILE_PATH = ShaderEffect_Standard::DEFAULT_FILE_PATH;
}

void H::Graphics::ShaderEffect_Standard::SetValue_TransformBuffer(const TransformData & tdata) const
{
	mTransformBuf->Set(tdata);
 }

void H::Graphics::ShaderEffect_Standard::SetValue_LightBuffer(const DirectionalLight & ldata) const
{
	mLightBuf->Set(ldata);
 
}

void H::Graphics::ShaderEffect_Standard::SetValue_MaterialBuffer(const Material & mdata) const
{
	mMaterialBuf->Set(mdata);
 
}

void H::Graphics::ShaderEffect_Standard::SetValue_SettingsBuffer(const Settings & settings) const
{
	mSettingsBuf->Set(settings);
}

void H::Graphics::ShaderEffect_Standard::SetValue_ShadowBuffer(const ShadowData & shadow) const
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
	mMaterialBuf = std::make_unique<H::Graphics::MaterialBuffer>();
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
 	ConstantBuffer_UnBindVS(0);
	ConstantBuffer_UnBindPS(0);
	ConstantBuffer_UnBindVS(1);
	ConstantBuffer_UnBindPS(1);
	ConstantBuffer_UnBindVS(2);
	ConstantBuffer_UnBindPS(2);
	ConstantBuffer_UnBindVS(3);
	ConstantBuffer_UnBindPS(3);
	ConstantBuffer_UnBindVS(4);
	ConstantBuffer_UnBindPS(4);


 	ShaderResource_UnBindPS(0);
	ShaderResource_UnBindPS(1);
	ShaderResource_UnBindPS(2);
	ShaderResource_UnBindPS(3);
	ShaderResource_UnBindPS(4);
	ShaderResource_UnBindPS(5);

	//mSampler->UnBindPS(0);
}

void H::Graphics::ShaderEffect_Standard::RenderData::DebugUI()
{
	ImGui::Begin("Standard Effect Data");

	ImGui::End();
}

void H::Graphics::ShaderEffect_Standard::SetContextInfo(const ShaderEffectContext& context) const
{
	const auto& stdContext = static_cast<const SE_Context_Standard&>(context);
	// dumb
	if (stdContext.transformData)
	{
		SetValue_TransformBuffer(*stdContext.transformData);
	}
	if (stdContext.directionalLight)
	{
		SetValue_LightBuffer(*stdContext.directionalLight);
	}
	if (stdContext.material)
	{
		SetValue_MaterialBuffer(*stdContext.material);
	}
	if (stdContext.settings)
	{
		SetValue_SettingsBuffer(*stdContext.settings);
	}
	if (stdContext.shadow)
	{
		SetValue_ShadowBuffer(*stdContext.shadow);
	}

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
void H::Graphics::ShaderEffect_Standard::Render(const ShaderEffectContext& context)
{
	SetContextInfo(context);
	Bind();
	context.meshBuffer->Render();
	UnBind();
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
