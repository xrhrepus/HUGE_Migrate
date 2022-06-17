#pragma once
#include <Graphics/Inc/ShaderEffect.h>
namespace H::Graphics
{
	class ShaderEffect_Standard : public ShaderEffect
	{
		//Bind->(Setcontextinfo->render)->unbind
#pragma region Standard_datatypes
	public:
		// same as H::Grahpics::Material, but using Shader data types
		struct MaterialSettings
		{
			ShaderData_Vector4 ambient;
			ShaderData_Vector4 diffuse;
			ShaderData_Vector4 specular;
			ShaderData_Vector1 power;
			float padding[3];
		};
		struct Settings
		{
			ShaderData_Vector1 specularMapWeight;
			ShaderData_Vector1 bumpMapWeight;
			ShaderData_Vector1 normalMapWeight;
			ShaderData_Vector1 aoMapWeight;
			ShaderData_Vector1 brightness;
			ShaderData_Boolean useShadow;
			ShaderData_Vector1 depthBias;
			float padding;
		};
		struct ShadowData
		{
			Matrix4 WVPLight;
		};
		using SettingsBuffer = H::Graphics::TypedConstantBuffer<Settings>;
		using ShadowDataBuffer = H::Graphics::TypedConstantBuffer<ShadowData>;

		class SE_Context_Standard : public ShaderEffectContext
		{
		public:
			void DebugUI() override;

			MaterialSettings material =
			{
				{*this, "ambient"},
				{*this, "diffuse"},
				{*this, "specular"},
				{*this, "power"}
				// 3 * float size padding don't need to show up in UI
			};
			Settings settings =
			{
				{ *this,"specularMapWeight" },
				{ *this,"bumpMapWeight" },
				{ *this,"normalMapWeight" },
				{ *this,"aoMapWeight" },
				{ *this,"brightness" },
				{ *this,"useShadow" },
				{ *this,"depthBias" }
			};
			ShadowData shadow;

			ShaderData_TextureId diffuse = { *this,"diffuse" };
			ShaderData_TextureId specular = { *this,"specular" };
			ShaderData_TextureId dispalcement = { *this,"dispalcement" };
			ShaderData_TextureId normal = { *this,"normal" };
			ShaderData_TextureId ao = { *this,"ao" };
			ShaderData_TextureId depth = { *this,"depth" };
		};
#pragma endregion

#pragma region SetValueFunctions

	public:

		ShaderEffect_Standard();
		std::unique_ptr<ShaderEffectContext> CreateShaderEffectContext() const override;
#pragma region SetValueFunctions

		//set buf values
		void SetValue_TransformBuffer(const TransformData& tdata) const;
		void SetValue_LightBuffer(const DirectionalLight& ldata) const;
		void SetValue_MaterialBuffer(const MaterialSettings& mdata) const;
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

		using MaterialSettingsBuffer = TypedConstantBuffer<MaterialSettings>;

		// buffers
		std::unique_ptr<TransformBuffer> mTransformBuf;
		std::unique_ptr<LightBuffer> mLightBuf;
		std::unique_ptr<MaterialSettingsBuffer> mMaterialBuf;
		std::unique_ptr<SettingsBuffer> mSettingsBuf;
		std::unique_ptr<ShadowDataBuffer> mShadowDataBuf;

		//const strings
		const wchar_t* DEFAULT_FILE_PATH = L"../../Assets/Shaders/Standard.fx";

	};

}
