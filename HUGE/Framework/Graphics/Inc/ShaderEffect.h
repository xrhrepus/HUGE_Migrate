#ifndef HUGE_GRAPHICS_SHADEREFFECT_H
#define HUGE_GRAPHICS_SHADEREFFECT_H
#include "Mesh.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "ShaderEffectBufferType.h"
#include "TextureManager.h"
#include "VertexShader.h"

namespace H::Graphics
{
	class Camera;
	struct ShaderEffectContext
	{
		virtual void DebugUI() = 0;
		Camera* camera = nullptr;
	};
 

	class ShaderEffect
	{
	public:
  
 		void Initialize();
		void Initialize(std::filesystem::path file);
		void Bind() const;
		virtual std::unique_ptr<ShaderEffectContext> CreateShaderEffectContext() const = 0;
		virtual void SetContextInfo(const ShaderEffectContext& context) const = 0;
		void UnBind() const;
		void Terminate();
		std::string GetShaderFilePath();
 	protected:
		//rasterizer
 		std::string default_RasterizerState = "Solid";
		std::filesystem::path DEFAULT_FILE_PATH;
 

	private:
		void Bind_VS_and_PS() const;
		virtual void InitBuffers() = 0;
		virtual void TerminateBuffers() = 0;

		virtual void Bind_Other_Buffers() const = 0;
		virtual void UnBind_Other_Buffers() const = 0;

 		VertexShader mVertexShader;
		PixelShader mPixelShader;
	};

	class ShaderEffect_Standard : public ShaderEffect
	{
		//Bind->(Setcontextinfo->render)->unbind
#pragma region Standard_datatypes
	public:
		struct Settings
		{
			float specularMapWeight;
			float bumpMapWeight;
			float normalMapWeight;
			float aoMapWeight;
			float brightness;
			bool useShadow;
			float depthBias;
			float padding;
		};
		struct ShadowData
		{
			Matrix4 WVPLight;
		};
		using SettingsBuffer = H::Graphics::TypedConstantBuffer<Settings>;
		using ShadowDataBuffer = H::Graphics::TypedConstantBuffer<ShadowData>;

		struct SE_Context_Standard : public ShaderEffectContext
		{
			void DebugUI() override;

			TransformData transformData;
			DirectionalLight directionalLight;
			Material material;
			Settings settings;
			ShadowData shadow;

			TextureId diffuse = 0;
			TextureId specular = 0;
			TextureId dispalcement = 0;
			TextureId normal = 0;
			TextureId ao = 0;
			TextureId depth = 0;
		};
#pragma endregion

#pragma region SetValueFunctions

	public:

 		ShaderEffect_Standard();
		std::unique_ptr<ShaderEffectContext> CreateShaderEffectContext() const override;
#pragma region SetValueFunctions

  		//set buf values
		void SetValue_TransformBuffer(const TransformData& tdata) const ;
		void SetValue_LightBuffer(const DirectionalLight& ldata) const;
		void SetValue_MaterialBuffer(const Material& mdata) const;
		void SetValue_SettingsBuffer(const Settings& settings) const;
		void SetValue_ShadowBuffer(const ShadowData& shadow) const;

		//set values
		void SetValue_SamplerType(const std::string samplerType) const;
		void SetValue_DiffuseMap(TextureId tid) const;
		void SetValue_SpecularMap(TextureId tid) const;
		void SetValue_DisplacementMap(TextureId tid) const;
		void SetValue_NormalMap(TextureId tid) const;
		void SetValue_AoMap(TextureId tid) const;
		void SetValue_DepthMap(TextureId tid) const;

		void SetContextInfo(const ShaderEffectContext& context) const override;

#pragma endregion

#pragma endregion

	private:
		void InitBuffers() override;
		void TerminateBuffers()override;
		void Bind_Other_Buffers() const override;
		void UnBind_Other_Buffers() const override;

 

		//sampler/blendmode ...
		const char* default_SamplerType = "LinearWrap";
		//Sampler* mSampler = nullptr;

		//
		//const SE_Context_Standard* mContext;

		// buffers
		std::unique_ptr<TransformBuffer> mTransformBuf;
		std::unique_ptr<LightBuffer> mLightBuf;
		std::unique_ptr<MaterialBuffer> mMaterialBuf;
		std::unique_ptr<SettingsBuffer> mSettingsBuf;
		std::unique_ptr<ShadowDataBuffer> mShadowDataBuf;

		//const strings
		const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Standard.fx";

	};

//	class ShaderEffect_Toon : public ShaderEffect
//	{
//		//Bind->(Setcontextinfo->render)->unbind
//#pragma region ToonShading_datatypes
//	public:
//		struct Settings
//		{
//			float specularMapWeight;
//			float bumpMapWeight;
//			float normalMapWeight;
//			float aoMapWeight;
//			float brightness;
//			bool useShadow;
//			float depthBias;
//			float padding;
//		};
//		struct ShadowData
//		{
//			Matrix4 WVPLight;
//		};
//		using SettingsBuffer = H::Graphics::TypedConstantBuffer<Settings>;
//		using ShadowDataBuffer = H::Graphics::TypedConstantBuffer<ShadowData>;
//
//
//		struct SE_Context_Toon : public ShaderEffectContext
//		{
//			TransformData transformData;
//			DirectionalLight directionalLight;
//			Material material;
//			Settings settings;
//			ShadowData shadow;
//
//			TextureId diffuse = 0;
//			TextureId specular = 0;
//			TextureId dispalcement = 0;
//			TextureId normal = 0;
//			TextureId ao = 0;
//			TextureId depth = 0;
//			TextureId ramp = 0;
//			TextureId normalDepth = 0;
//
//		};
//#pragma endregion
//
//#pragma region SetValueFunctions
//
//	public:
//
//		ShaderEffect_Toon();
//#pragma region SetValueFunctions
//
//		//set buf values
//		void SetValue_TransformBuffer(const TransformData& tdata) const;
//		void SetValue_LightBuffer(const DirectionalLight& ldata) const;
//		void SetValue_MaterialBuffer(const Material& mdata) const;
//		void SetValue_SettingsBuffer(const Settings& settings) const;
//		void SetValue_ShadowBuffer(const ShadowData& shadow) const;
//
//		//set values
//		void SetValue_SamplerType(const std::string samplerType) const;
//		void SetValue_DiffuseMap(TextureId tid) const;
//		void SetValue_SpecularMap(TextureId tid) const;
//		void SetValue_DisplacementMap(TextureId tid) const;
//		void SetValue_NormalMap(TextureId tid) const;
//		void SetValue_AoMap(TextureId tid) const;
//		void SetValue_DepthMap(TextureId tid) const;
//		void SetValue_RampMap(TextureId tid) const;
//
//		void SetContextInfo(ShaderEffectContext* context) override;
//		void Render(ShaderEffectContext* context) override;
//
//#pragma endregion
//
//#pragma endregion
//
//	private:
//		void InitBuffers() override;
//		void TerminateBuffers()override;
//		void Bind_Other_Buffers() const override;
//		void UnBind_Other_Buffers() const override;
//
//
//
//		//sampler/blendmode ...
//		const char* default_SamplerType = "LinearWrap";
//		//Sampler* mSampler = nullptr;
//
//		//
//		SE_Context_Toon* mContext;
//
//		// buffers
//		std::unique_ptr<TransformBuffer> mTransformBuf;
//		std::unique_ptr<LightBuffer> mLightBuf;
//		std::unique_ptr<MaterialBuffer> mMaterialBuf;
//		std::unique_ptr<SettingsBuffer> mSettingsBuf;
//		std::unique_ptr<ShadowDataBuffer> mShadowDataBuf;
//
//		//const strings
//		const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/ToonShading.fx";
//
//	};
//
//
//
//	class ShaderEffect_Skinning : public ShaderEffect
//	{
//	public:
//#pragma region Skinning_datatypes
//
//		//struct Settings
//		//{
//		//	float option;
//		//	float padding[3];
//		//};
//		struct SE_Context_Skinning : public ShaderEffectContext
//		{
//			TransformData transformData;
//			DirectionalLight directionalLight;
//			Material material;
//			OptionsData settings;
//			BoneTransformData boneTransform;
// 
//			TextureId diffuse;
//			TextureId specular;
//			TextureId dispalcement;
//			TextureId normal;
// 		};
//#pragma endregion
//
//		ShaderEffect_Skinning();
//		//set buffer ptr
//  
//		//set buf values
//		void SetValue_TransformBuffer(const TransformData& tdata) const;
//		void SetValue_LightBuffer(const DirectionalLight& ldata) const;
//		void SetValue_MaterialBuffer(const Material& mdata) const;
//		void SetValue_OptionsBuffer(const OptionsData& odata) const;
//		void SetValue_BoneTransformBuffer(const BoneTransformData& btdata) const;
//
//		//set values
//		void SetValue_SamplerType(const std::string samplerType) const;
//		void SetValue_DiffuseMap(TextureId tid) const;
//		void SetValue_SpecularMap(TextureId tid) const;
//		void SetValue_DisplacementMap(TextureId tid) const;
//		void SetValue_NormalMap(TextureId tid)const;
//
//		void SetContextInfo(ShaderEffectContext* context) override;
//		void Render(ShaderEffectContext* context) override;
//
//	private:
//		void InitBuffers() override;
//		void TerminateBuffers()override;
//		void Bind_Other_Buffers() const override;
//		void UnBind_Other_Buffers() const override;
//
//
// 
//		const char* default_SamplerType = "LinearWrap";
//		//Sampler* mSampler = nullptr;
//
//		SE_Context_Skinning* mContext;
//
//		//5 bufs
//		std::unique_ptr<TransformBuffer> mTransformBuf;
//		std::unique_ptr<LightBuffer> mLightBuf ;
//		std::unique_ptr<MaterialBuffer> mMaterialBuf ;
//		std::unique_ptr<OptionsBuffer> mOptionsBuf ;
//		std::unique_ptr<BoneTransformBuffer> mBoneTransformBuf ;
//
// 
//		//const strings
//		const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Skinning.fx";
//
//	};

//	class ShaderEffect_DrawEdge : public ShaderEffect
//	{
//		//Bind->(Setcontextinfo->render)->unbind
//#pragma region DrawEdge_datatypes
//	public:
//		struct Settings
//		{
//			float specularMapWeight;
//			float bumpMapWeight;
//			float normalMapWeight;
//			float aoMapWeight;
//			float brightness;
//			bool useShadow;
//			float depthBias;
//			float padding;
//		};
//		struct ShadowData
//		{
//			Matrix4 WVPLight;
//		};
//		using SettingsBuffer = H::Graphics::TypedConstantBuffer<Settings>;
//		using ShadowDataBuffer = H::Graphics::TypedConstantBuffer<ShadowData>;
//
//
//		struct SE_Context_Standard : public ShaderEffectContext
//		{
//			TransformData transformData;
//			DirectionalLight directionalLight;
//			Material material;
//			Settings settings;
//			ShadowData shadow;
//
//			TextureId diffuse;
//			TextureId specular;
//			TextureId dispalcement;
//			TextureId normal;
//			TextureId ao;
//			TextureId depth;
//		};
//#pragma endregion
//
//#pragma region SetValueFunctions
//
//	public:
//
//		ShaderEffect_Standard();
//#pragma region SetValueFunctions
//
//		//set buf values
//		void SetValue_TransformBuffer(const TransformData& tdata) const;
//		void SetValue_LightBuffer(const DirectionalLight& ldata) const;
//		void SetValue_MaterialBuffer(const Material& mdata) const;
//		void SetValue_SettingsBuffer(const Settings& settings) const;
//		void SetValue_ShadowBuffer(const ShadowData& shadow) const;
//
//		//set values
//		void SetValue_SamplerType(const std::string samplerType) const;
//		void SetValue_DiffuseMap(TextureId tid) const;
//		void SetValue_SpecularMap(TextureId tid) const;
//		void SetValue_DisplacementMap(TextureId tid) const;
//		void SetValue_NormalMap(TextureId tid) const;
//		void SetValue_AoMap(TextureId tid) const;
//		void SetValue_DepthMap(TextureId tid) const;
//
//		void SetContextInfo(ShaderEffectContext* context) override;
//		void Render(ShaderEffectContext* context) override;
//
//#pragma endregion
//
//#pragma endregion
//
//	private:
//		void InitBuffers() override;
//		void TerminateBuffers()override;
//		void Bind_Other_Buffers() const override;
//		void UnBind_Other_Buffers() const override;
//
//
//
//		//sampler/blendmode ...
//		const char* default_SamplerType = "LinearWrap";
//		//Sampler* mSampler = nullptr;
//
//		//
//		SE_Context_Standard* mContext;
//
//		// buffers
//		std::unique_ptr<TransformBuffer> mTransformBuf;
//		std::unique_ptr<LightBuffer> mLightBuf;
//		std::unique_ptr<MaterialBuffer> mMaterialBuf;
//		std::unique_ptr<SettingsBuffer> mSettingsBuf;
//		std::unique_ptr<ShadowDataBuffer> mShadowDataBuf;
//
//		//const strings
//		const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Standard.fx";
//
//	};

}//namespace H::Graphics

#endif // !HUGE_GRAPHICS_SHADEREFFECT_H

