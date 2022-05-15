#pragma once
#include <HUGE/Inc/HUGE.h>

class TShadowMap
{
public:
	void Initialize();
	void Terminate();
	void Bind();
 	void DebugUI();
	struct SE_Context_TShadowMap : public ShaderEffectContext
	{
		TransformData transformData;
		DirectionalLight directionalLight;
		Material material;
		OptionsData optionsData;
 
	};
	SE_Context_TShadowMap mContext;
private:
	const std::filesystem::path path_shader = L"../../Assets/Shaders/ShadowMap_Test.fx";
	std::unique_ptr<TransformBuffer> mTransformBuf;
	std::unique_ptr<LightBuffer> mLightBuf;

	VertexShader mVS;
	PixelShader mPS;
};
 