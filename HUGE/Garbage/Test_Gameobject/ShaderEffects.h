#pragma once
#include <HUGE/Inc/HUGE.h>
#include "BufferType.h"
using namespace H::Math;
using namespace H::Graphics;

class MBAObject;

class Effect
{
public:
	void Init();
	void Init(std::filesystem::path file);
 	void Bind();
	void UnBind();
	virtual void RenderMBAObject(const MBAObject* mbaobj) const = 0;

	void Terminate();

protected:
	//rasterizer
	const char* default_RasterizerState = "Solid";
 	const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";

private:
	void Bind_VS_and_PS();
	virtual void Bind_Other_Buffers() const = 0;
	virtual void UnBind_Other_Buffers() const = 0;
	//virtual void SetAttributesByRenderComponent() const = 0;

	VertexShader mVertexShader;
	PixelShader mPixelShader;
};
//further usage
// Gameobject
// - Mesh
// - Tex/Diffmap/normap...
//
//private pure virtual
//Effect RenderGO(const Go& ) const
//{
//    set...(Go.tex...)
//		go.render
//}
//
//GOs use this effect
//std vector <GO*> GOs
//foreach go in GOs
//...
//

class Effect_DoTexture : public Effect
{
public:
	Effect_DoTexture();
 
	void SetPointer_TransformBuffer(TransformBuffer* tfb);

	void SetValue_SamplerType(const std::string samplerType);
	void SetValue_TransformBuffer(const TransformData& tdata);
	void SetValue_DiffuseMap(TextureId tid);

	void RenderMBAObject(const MBAObject* mbaobj) const override;
private:
	void Bind_Other_Buffers() const override;
	void UnBind_Other_Buffers() const override;

	const char* default_SamplerType = "LinearWarp";
	Sampler* mSampler = nullptr;
 
 	const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";
	TransformBuffer* mTransformBuf = nullptr;
	TextureId mDiffuseMap;
	//const char* mSamplerType = "LinearWarp";
	//const char* mRasterizerState = "CullFrontSolid";

};

class Effect_Standard : public Effect
{
public:
	//Effect_Standard(TransformBuffer& tfBuf, LightBuffer& lightBuf, MaterialBuffer& materialBuf, OptionsBuffer& optBuf);
	Effect_Standard();
	//set buffer ptr
 
	void SetPointer_TransformBuffer(TransformBuffer* tfb);
	void SetPointer_LightBuffer(LightBuffer* lb);
	void SetPointer_MaterialBuffer(MaterialBuffer* mb);
	void SetPointer_OptionsBuffer(OptionsBuffer* ob);

	//set buf values
	void SetValue_TransformBuffer(const TransformData& tdata);
	void SetValue_LightBuffer(const DirectionalLight& ldata);
	void SetValue_MaterialBuffer(const Material& mdata);
	void SetValue_OptionsBuffer(const OptionsData& odata);

	//set values
	void SetValue_SamplerType(const std::string samplerType);
	void SetValue_DiffuseMap(TextureId tid);
	void SetValue_SpecularMap(TextureId tid);
	void SetValue_DisplacementMap(TextureId tid);
	void SetValue_NormalMap(TextureId tid);

	void RenderMBAObject(const MBAObject* mbaobj) const override;

private:
	void Bind_Other_Buffers() const override;
	void UnBind_Other_Buffers() const override;

	const char* default_SamplerType = "LinearWarp";
	Sampler* mSampler = nullptr;
 
	//4 bufs
	TransformBuffer* mTransformBuf = nullptr;
	LightBuffer* mLightBuf = nullptr;
	MaterialBuffer* mMaterialBuf = nullptr;
	OptionsBuffer* mOptionsBuf = nullptr;

	//4 textures
	TextureId mDiffuseMap=0;
	TextureId mSpecularMap = 0;
	TextureId mDisplacementMap = 0;
	TextureId mNormalMap = 0;

	//const strings
	const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Standard.fx";
 
};

class Effect_Skinning : public Effect
{
public:
	Effect_Skinning();
	//set buffer ptr

	void Init();

	//void SetPointer_TransformBuffer(TransformBuffer* tfb);
	//void SetPointer_LightBuffer(LightBuffer* lb);
	//void SetPointer_MaterialBuffer(MaterialBuffer* mb);
	//void SetPointer_OptionsBuffer(OptionsBuffer* ob);
	//void SetPointer_BoneTransformBuffer(BoneTransformBuffer* btb);

	//set buf values
	void SetValue_TransformBuffer(const TransformData& tdata);
	void SetValue_LightBuffer(const DirectionalLight& ldata);
	void SetValue_MaterialBuffer(const Material& mdata);
	void SetValue_OptionsBuffer(const OptionsData& odata);
	void SetValue_BoneTransformBuffer(const BoneTransformData& btdata);

	//set values
	void SetValue_SamplerType(const std::string samplerType);
	void SetValue_DiffuseMap(TextureId tid);
	void SetValue_SpecularMap(TextureId tid);
	void SetValue_DisplacementMap(TextureId tid);
	void SetValue_NormalMap(TextureId tid);

	//render
	void RenderMBAObject(const MBAObject* mbaobj) const override;

private:
	void Bind_Other_Buffers() const override;
	void UnBind_Other_Buffers() const override;

	const char* default_SamplerType = "LinearWarp";
	Sampler* mSampler = nullptr;

	//5 bufs
	std::unique_ptr<TransformBuffer> mTransformBuf = nullptr;
	std::unique_ptr<LightBuffer> mLightBuf = nullptr;
	std::unique_ptr<MaterialBuffer> mMaterialBuf = nullptr;
	std::unique_ptr<OptionsBuffer> mOptionsBuf = nullptr;
	std::unique_ptr<BoneTransformBuffer> mBoneTransformBuf = nullptr;

	//4 textures
	TextureId mDiffuseMap = 0;
	TextureId mSpecularMap = 0;
	TextureId mDisplacementMap = 0;
	TextureId mNormalMap = 0;

	//const strings
	const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Skinning.fx";

};