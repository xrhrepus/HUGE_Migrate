#include "ShaderEffects.h"
#include "Graphics/Src/D3DUtil.h"
#include "MBAObject.h"
#pragma region AbstractEffectClass


void Effect::Init()
{
	mVertexShader.Initialize(VertexPX::Format, DEFAULT_FILE_PATH);
	mPixelShader.Initialize(DEFAULT_FILE_PATH, "PS");
 }

void Effect::Init(std::filesystem::path file)
{
	mVertexShader.Initialize(0,file.c_str());
	mPixelShader.Initialize(file.c_str(), "PS");
 }
 
 
void Effect::Bind()
{
 	Bind_VS_and_PS();
	Bind_Other_Buffers();
}

void Effect::UnBind()
{
	mVertexShader.UnBind();
	mPixelShader.UnBind();
	UnBind_Other_Buffers();
}

void Effect::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}
void Effect::Bind_VS_and_PS()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

}
#pragma endregion
#pragma region Dotexture
Effect_DoTexture::Effect_DoTexture()
{
	Effect::DEFAULT_FILE_PATH = Effect_DoTexture::DEFAULT_FILE_PATH;
	//
}
void Effect_DoTexture::SetPointer_TransformBuffer(TransformBuffer * tfb)
{
	mTransformBuf = tfb;
}
void Effect_DoTexture::SetValue_SamplerType(const std::string samplerType)
{
	mSampler = SamplerManager::Get()->GetSampler(samplerType);
	mSampler->BindPS(0);
}
void Effect_DoTexture::SetValue_TransformBuffer(const TransformData & tdata)
{
	mTransformBuf->Set(tdata); 
	//mTransformBuf->BindVS(0);
	//mTransformBuf->BindPS(0);
}

void Effect_DoTexture::SetValue_DiffuseMap(TextureId tid) 
{
	mDiffuseMap = tid; 
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
}
void Effect_DoTexture::RenderMBAObject(const MBAObject * mbaobj) const
{
}
void Effect_DoTexture::Bind_Other_Buffers() const
{
 	//mSampler->BindPS(0);

	//TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);

	mTransformBuf->BindVS(0);
	mTransformBuf->BindPS(0);

}

void Effect_DoTexture::UnBind_Other_Buffers() const
{
 
	mTransformBuf->UnBindVS(0);
	mTransformBuf->UnBindPS(0);

	TextureManager::Get()->GetTexture(mDiffuseMap)->UnBindPS(0);
	
	mSampler->UnBindPS(0);
}
 
 #pragma endregion
#pragma region Standard


//Effect_Standard::Effect_Standard(TransformBuffer & tfBuf, LightBuffer & lightBuf, MaterialBuffer & materialBuf, OptionsBuffer & optBuf)
//	:mTransformBuf(tfBuf),mLightBuf(lightBuf),mMaterialBuf(materialBuf),mOptionsBuf(optBuf)
//{
//}

 

Effect_Standard::Effect_Standard()
{
	Effect::DEFAULT_FILE_PATH = Effect_Standard::DEFAULT_FILE_PATH;
}

 

void Effect_Standard::SetPointer_TransformBuffer(TransformBuffer * tfb)
{
	mTransformBuf = tfb;
}

void Effect_Standard::SetPointer_LightBuffer(LightBuffer * lb)
{
	mLightBuf = lb;
}

void Effect_Standard::SetPointer_MaterialBuffer(MaterialBuffer * mb)
{
	mMaterialBuf = mb;
}

void Effect_Standard::SetPointer_OptionsBuffer(OptionsBuffer * ob)
{
	mOptionsBuf = ob;
}


void Effect_Standard::SetValue_TransformBuffer(const TransformData & tdata)
{
	mTransformBuf->Set(tdata);
	//mTransformBuf->BindVS(0);
	//mTransformBuf->BindPS(0);

}

void Effect_Standard::SetValue_LightBuffer(const DirectionalLight & ldata) 
{
	mLightBuf->Set(ldata);
	//mLightBuf->BindVS(1);
	//mLightBuf->BindPS(1);
}

void Effect_Standard::SetValue_MaterialBuffer(const Material & mdata)
{
	mMaterialBuf->Set(mdata);
	//mMaterialBuf->BindVS(2);
	//mMaterialBuf->BindPS(2);
}

void Effect_Standard::SetValue_OptionsBuffer(const OptionsData & odata)
{
	mOptionsBuf->Set(odata);
	//mOptionsBuf->BindVS(3);
	//mOptionsBuf->BindPS(3);
}
void Effect_Standard::SetValue_SamplerType(const std::string samplerType)
{
	mSampler = SamplerManager::Get()->GetSampler(samplerType);
	mSampler->BindPS(0);
}

void Effect_Standard::SetValue_DiffuseMap(TextureId tid) 
{
	mDiffuseMap = tid;
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
}

void Effect_Standard::SetValue_SpecularMap(TextureId tid) 
{
	mSpecularMap = tid;
	TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);
}

void Effect_Standard::SetValue_DisplacementMap(TextureId tid) 
{
	mDisplacementMap = tid;
	TextureManager::Get()->GetTexture(mDisplacementMap)->BindVS(2);
}

void Effect_Standard::SetValue_NormalMap(TextureId tid) 
{
	mNormalMap = tid;
	TextureManager::Get()->GetTexture(mNormalMap)->BindPS(3);
}

void Effect_Standard::RenderMBAObject(const MBAObject * mbaobj) const
{
}

 
void Effect_Standard::Bind_Other_Buffers() const
{
 	//mSampler->BindPS(0);

	//pSamplerMngr->GetSampler(mSamplerType)->BindPS(0);
	//TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
	//TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);
	//TextureManager::Get()->GetTexture(mDisplacementMap)->BindVS(2);
	//TextureManager::Get()->GetTexture(mNormalMap)->BindPS(3);

	mTransformBuf->BindVS(0);
	mTransformBuf->BindPS(0);
	mLightBuf->BindVS(1);
	mLightBuf->BindPS(1);
	mMaterialBuf->BindVS(2);
	mMaterialBuf->BindPS(2);
	mOptionsBuf->BindVS(3);
	mOptionsBuf->BindPS(3);
}

void Effect_Standard::UnBind_Other_Buffers() const
{
	
	mTransformBuf->UnBindVS(0);
	mTransformBuf->UnBindPS(0);
	mLightBuf->UnBindVS(1);
	mLightBuf->UnBindPS(1);
	mMaterialBuf->UnBindVS(2);
	mMaterialBuf->UnBindPS(2);
	mOptionsBuf->UnBindVS(3);
	mOptionsBuf->UnBindPS(3);

	auto txMngr = TextureManager::Get();

	txMngr->GetTexture(mDiffuseMap)->UnBindPS(0);
	txMngr->GetTexture(mSpecularMap)->UnBindPS(1);
	txMngr->GetTexture(mDisplacementMap)->UnBindVS(2);
	txMngr->GetTexture(mNormalMap)->UnBindPS(3);

	mSampler->UnBindPS(0);
}

 

 
 

#pragma endregion


#pragma region Skinning
Effect_Skinning::Effect_Skinning()
{
	Effect::DEFAULT_FILE_PATH = Effect_Skinning::DEFAULT_FILE_PATH;
}

void Effect_Skinning::Init()
{
	mTransformBuf = std::make_unique<TransformBuffer>();
	mLightBuf = std::make_unique<LightBuffer>();
	mMaterialBuf = std::make_unique<MaterialBuffer>();
	mOptionsBuf = std::make_unique<OptionsBuffer>();
	mBoneTransformBuf = std::make_unique<BoneTransformBuffer>();
}

//void Effect_Skinning::SetPointer_TransformBuffer(TransformBuffer * tfb)
//{
//	mTransformBuf = tfb;
//}
//
//void Effect_Skinning::SetPointer_LightBuffer(LightBuffer * lb)
//{
//	mLightBuf = lb;
//}
//
//void Effect_Skinning::SetPointer_MaterialBuffer(MaterialBuffer * mb)
//{
//	mMaterialBuf = mb;
//}
//
//void Effect_Skinning::SetPointer_OptionsBuffer(OptionsBuffer * ob)
//{
//	mOptionsBuf = ob;
//}
//
//void Effect_Skinning::SetPointer_BoneTransformBuffer(BoneTransformBuffer * btb)
//{
//	mBoneTransformBuf = btb;
//}

 
void Effect_Skinning::SetValue_TransformBuffer(const TransformData & tdata)
{
	mTransformBuf->Set(tdata);
}

void Effect_Skinning::SetValue_LightBuffer(const DirectionalLight & ldata)
{
	mLightBuf->Set(ldata);
}

void Effect_Skinning::SetValue_MaterialBuffer(const Material & mdata)
{
	mMaterialBuf->Set(mdata);
}

void Effect_Skinning::SetValue_OptionsBuffer(const OptionsData & odata)
{
	mOptionsBuf->Set(odata);
}

void Effect_Skinning::SetValue_BoneTransformBuffer(const BoneTransformData & btdata)
{
	mBoneTransformBuf->Set(btdata);
}

void Effect_Skinning::SetValue_SamplerType(const std::string samplerType)
{
	mSampler = SamplerManager::Get()->GetSampler(samplerType);
	mSampler->BindPS(0);
}

void Effect_Skinning::SetValue_DiffuseMap(TextureId tid)
{
	mDiffuseMap = tid;
	TextureManager::Get()->GetTexture(mDiffuseMap)->BindPS(0);
}

void Effect_Skinning::SetValue_SpecularMap(TextureId tid)
{
	mSpecularMap = tid;
	TextureManager::Get()->GetTexture(mSpecularMap)->BindPS(1);

}

void Effect_Skinning::SetValue_DisplacementMap(TextureId tid)
{
	mDisplacementMap = tid;
	TextureManager::Get()->GetTexture(mDisplacementMap)->BindVS(2);

}

void Effect_Skinning::SetValue_NormalMap(TextureId tid)
{
	mNormalMap = tid;
	TextureManager::Get()->GetTexture(mNormalMap)->BindPS(3);

}

void Effect_Skinning::RenderMBAObject(const MBAObject * mbaobj) const 
{
}

void Effect_Skinning::Bind_Other_Buffers() const
{
	mTransformBuf->BindVS(0);
	mTransformBuf->BindPS(0);
	mLightBuf->BindVS(1);
	mLightBuf->BindPS(1);
	mMaterialBuf->BindVS(2);
	mMaterialBuf->BindPS(2);
	mOptionsBuf->BindVS(3);
	mOptionsBuf->BindPS(3);
	mBoneTransformBuf->BindVS(4);
	mBoneTransformBuf->BindPS(4);


}

void Effect_Skinning::UnBind_Other_Buffers() const
{
	mTransformBuf->UnBindVS(0);
	mTransformBuf->UnBindPS(0);
	mLightBuf->UnBindVS(1);
	mLightBuf->UnBindPS(1);
	mMaterialBuf->UnBindVS(2);
	mMaterialBuf->UnBindPS(2);
	mOptionsBuf->UnBindVS(3);
	mOptionsBuf->UnBindPS(3);
	mBoneTransformBuf->UnBindVS(4);
	mBoneTransformBuf->UnBindPS(4);

	auto txMngr = TextureManager::Get();

	txMngr->GetTexture(mDiffuseMap)->UnBindPS(0);
	txMngr->GetTexture(mSpecularMap)->UnBindPS(1);
	txMngr->GetTexture(mDisplacementMap)->UnBindVS(2);
	txMngr->GetTexture(mNormalMap)->UnBindPS(3);

	mSampler->UnBindPS(0);
}

#pragma endregion

