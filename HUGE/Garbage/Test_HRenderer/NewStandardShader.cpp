#include "NewStandardShader.h"
#include "Graphics/Src/D3DUtil.h"

void NewStandardShader::Initialize()
{
	mTransformBuf = std::make_unique<H::Graphics::TypedConstantBuffer<TransformData>>();
	mDLightBuf = std::make_unique<H::Graphics::TypedConstantBuffer<DirectionalLight>>();
	mMaterialBuf = std::make_unique<H::Graphics::TypedConstantBuffer<Material>>();
	mSettingBuf = std::make_unique<H::Graphics::TypedConstantBuffer<NSettingBuffer>>();
	mShadowBuf = std::make_unique<H::Graphics::TypedConstantBuffer<NShadowBuffer>>();


	mTransformBuf->Initialize();
	mDLightBuf  ->Initialize();
	mMaterialBuf->Initialize();
	mSettingBuf ->Initialize();
	mShadowBuf  ->Initialize();
	mVS.Initialize(path_shader.c_str());
	mPS.Initialize(path_shader.c_str(), "PS");
}

void NewStandardShader::Terminate()
{
	mTransformBuf->Terminate();
	mDLightBuf->Terminate();
	mMaterialBuf->Terminate();
	mSettingBuf->Terminate();
	mShadowBuf->Terminate();
	mVS.Terminate();
	mPS.Terminate();
 
}

void NewStandardShader::Bind()
{
	mTransformBuf->BindVS(0);
	mTransformBuf->BindPS(0);
	mTransformBuf->Set(mContext.transform);


	mDLightBuf->BindVS(1);
	mDLightBuf->BindPS(1);
	mDLightBuf->Set(mContext.light);

	mMaterialBuf->BindVS(2);
	mMaterialBuf->BindPS(2);
	mMaterialBuf->Set(mContext.material);

	mSettingBuf->BindVS(3);
	mSettingBuf->BindPS(3);
	mSettingBuf->Set(mContext.setting);

	mShadowBuf->BindVS(4);
	mShadowBuf->BindPS(4);
	mShadowBuf->Set(mContext.shadow);

 //
	auto txMngr = TextureManager::Get();

	txMngr->GetTexture(mContext.diffuse)->BindPS(0);
	if (mContext.specular != 0)
 		txMngr->GetTexture(mContext.specular)->BindPS(1);
	if (mContext.dispalcement != 0)
	 	txMngr->GetTexture(mContext.dispalcement)->BindVS(2);
	if (mContext.normal != 0)
		txMngr->GetTexture(mContext.normal)->BindPS(3);
	if (mContext.ao != 0)
		txMngr->GetTexture(mContext.ao)->BindPS(4);
	if (mContext.depth != 0)
		txMngr->GetTexture(mContext.depth)->BindPS(5);
 
	mVS.Bind();
	mPS.Bind();

}

void NewStandardShader::UnBind()
{
	H::Graphics::GetContext()->VSSetConstantBuffers(0, 0, nullptr);
	H::Graphics::GetContext()->VSSetConstantBuffers(1, 0, nullptr);
	H::Graphics::GetContext()->VSSetConstantBuffers(2, 0, nullptr);
	H::Graphics::GetContext()->VSSetConstantBuffers(3, 0, nullptr);
	H::Graphics::GetContext()->VSSetConstantBuffers(4, 0, nullptr);

	H::Graphics::GetContext()->PSSetConstantBuffers(0, 0, nullptr);
	H::Graphics::GetContext()->PSSetConstantBuffers(1, 0, nullptr);
	H::Graphics::GetContext()->PSSetConstantBuffers(2, 0, nullptr);
	H::Graphics::GetContext()->PSSetConstantBuffers(3, 0, nullptr);
	H::Graphics::GetContext()->PSSetConstantBuffers(4, 0, nullptr);

 	H::Graphics::GetContext()->PSSetShaderResources(0, 0, nullptr);
	H::Graphics::GetContext()->PSSetShaderResources(1, 0, nullptr);
	H::Graphics::GetContext()->VSSetShaderResources(2, 0, nullptr);//displacement on vs
	H::Graphics::GetContext()->PSSetShaderResources(3, 0, nullptr);
	H::Graphics::GetContext()->PSSetShaderResources(4, 0, nullptr);
	H::Graphics::GetContext()->PSSetShaderResources(5, 0, nullptr);
 	
}

void NewStandardShader::UpdateContextValue(const NS_Context & ctx)
{
	mContext = (ctx);
}

void NewStandardShader::Render()
{
}

void NewStandardShader::DebugUI()
{
	if (ImGui::CollapsingHeader("NewStandardShader"))
	{
		/*bool lightChanged = false;
 
		if (ImGui::DragFloat3("LightDir##NewStandardShader", mContext.light.direction.v.data(), 0.1f))
		{
			lightChanged = true;
		}*/

 
		ImGui::DragFloat("Bias##NewStandardShader", &mContext.setting.depthBias, 0.001f);
		//ImGui::DragFloat("Brightness##NewStandardShader", &mContext.setting.brightness, 0.1f);
		ImGui::Checkbox("use shadow##NewStandardShader", &mContext.setting.useShadow);

		/*if (lightChanged)
		{
			mContext.light.direction = H::Math::Normalize(mContext.light.direction);
		}*/
	}
	
 
}
