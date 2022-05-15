#pragma once
#include <HUGE/Inc/HUGE.h>

struct NSettingBuffer
{
	float specularMapWeight;
	float bumpMapWeight  ;
	float normalMapWeight;
	float aoMapWeight ;
	float brightness  ;
	bool useShadow    ;
	float depthBias   ;
	float padding;

};
struct NShadowBuffer
{
	Matrix4 WVPLight;
};

class NewStandardShader
{
public:
	struct NS_Context : ShaderEffectContext
	{
		TransformData transform;
		DirectionalLight light;
		Material material;
		NSettingBuffer setting;
		NShadowBuffer shadow;

		TextureId diffuse;
		TextureId specular;
		TextureId dispalcement;
		TextureId normal;
		TextureId ao;
		TextureId depth;
 
	} mContext;

	void Initialize();
	void Terminate();
 	void Bind();
	void UnBind();

	void UpdateContextValue(const NS_Context& ctx);
	void Render();
 	void DebugUI();
 
private:
	const std::filesystem::path path_shader = L"../../Assets/Shaders/Standard.fx";
	VertexShader mVS;
	PixelShader mPS;


	std::unique_ptr<TypedConstantBuffer<TransformData>> mTransformBuf;
	std::unique_ptr<TypedConstantBuffer<DirectionalLight>> mDLightBuf;
	std::unique_ptr<TypedConstantBuffer<Material>> mMaterialBuf;
	std::unique_ptr<TypedConstantBuffer<NSettingBuffer>> mSettingBuf;
	std::unique_ptr<TypedConstantBuffer<NShadowBuffer>> mShadowBuf;

};

 