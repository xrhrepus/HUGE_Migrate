#include "TBloom.h"

void TBloom::Initialize()
{

	mRT_Origin.Initialize(GraphicSystem::Get()->GetBackBufferWidth(), GraphicSystem::Get()->GetBackBufferHeight(), H::Graphics::RenderTarget::Format::RGBA_U32);

	mVS_Bloom.Initialize(VertexPX::Format, POSTPROCESSING_BLOOM_FILE_PATH);
	mPS_Bloom.Initialize(POSTPROCESSING_BLOOM_FILE_PATH, "PS");
	mRT_Bloom
		.Initialize(GraphicSystem::Get()->GetBackBufferWidth(), GraphicSystem::Get()->GetBackBufferHeight(), H::Graphics::RenderTarget::Format::RGBA_U32);

	mVS_Combine.Initialize(VertexPX::Format, POSTPROCESSING_COMBINE_FILE_PATH);
	mPS_Combine.Initialize(POSTPROCESSING_COMBINE_FILE_PATH, "PS");
	mRT_Combine
		.Initialize(GraphicSystem::Get()->GetBackBufferWidth(), GraphicSystem::Get()->GetBackBufferHeight(), H::Graphics::RenderTarget::Format::RGBA_U32);
	mData.baseIntensity = 1.0f;
	mData.baseSaturation = 1.0f;
	mData.bloomIntensity = 1.0f;
	mData.bloomSaturation = 1.0f;


	mData.u_step = 1.0f / GraphicSystem::Get()->GetBackBufferWidth();
	mData.v_step = 1.0f / GraphicSystem::Get()->GetBackBufferHeight();
	mData.decay = 0.01f;
	mData.blurIntensity = 0.8f;
	mData.radiation = 0.08f;
	mData.offset = 5.0f;
	mDataBuffer.Initialize(mData);

	mScreenMesh = MeshBuilder::CreateNDCQuadPX();//create screen quad;
	mScreenMeshBuffer.Initialize(mScreenMesh);

}

void TBloom::Terminate()
{
	mVS_Bloom.Terminate();
	mPS_Bloom.Terminate();
	mVS_Combine.Terminate();
	mPS_Combine.Terminate();
	mDataBuffer.Terminate();

}

void TBloom::BloomOriginBegin()
{
	mRT_Origin.BeginRender();
}

void TBloom::BloomOriginEnd()
{
	mRT_Origin.EndRender();
}

void TBloom::Render()
{
	mRT_Origin.BindPS(0);
	if (isBloomOn)
	{
		mRT_Bloom.BeginRender();

		mVS_Bloom.Bind();
		mPS_Bloom.Bind();
		SamplerManager::Get()->GetSampler("PointClamp")->BindPS(0);

		mDataBuffer.Set(mData);
		mDataBuffer.BindVS(0);
		mDataBuffer.BindPS(0);
		mScreenMeshBuffer.Render();
		mRT_Bloom.EndRender();
	}
	//
	mRT_Combine.BeginRender();
	mRT_Origin.BindPS(0);
	mRT_Bloom.BindPS(1);

	mVS_Combine.Bind();
	mPS_Combine.Bind();


	mScreenMeshBuffer.Render();//draw
	mRT_Combine.EndRender();
	mRT_Bloom.Reset();
	//mRT_Origin.UnbindPS(0);
	//mRT_Bloom.UnbindPS(1);

	////reset shader res view
	//mPostProc_NoneVertexShader.Bind();
	//mPostProc_NonePixelShader.Bind();

	//mPostProc_Bloom_RenderTargets.BeginRender();
	//mScreenMeshBuffer.Render();
	//mPostProc_Bloom_RenderTargets.EndRender();


}

void TBloom::DebugUI()
{
	if (ImGui::CollapsingHeader("Post processing"))
	{
		ImGui::TextDisabled("[------Bloom---------]");
		ImGui::Checkbox("On/Off##Bloom", &isBloomOn);
		ImGui::DragFloat("threshold##Bloom", &mData.bloomThreshold, 0.01f, 0.0f, 0.999f);
		ImGui::DragFloat("bloom_intensity##Bloom", &mData.bloomIntensity, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("bloom_saturation##Bloom", &mData.bloomSaturation, 0.01f, 0.0f, 1.0f);

		ImGui::TextDisabled("[------SimpleBlur---------]");
		if (ImGui::DragFloat("blur_radiation##Bloom", &mData.radiation, 0.001f, 0.01f, 1.0f))
		{
			float r = mData.radiation / (60.0f * mData.u_step);
			float r2 = mData.radiation / (20.0f * mData.u_step);

			if (mData.offset < r)
			{
				mData.offset = r;
			}
			else if (mData.offset > r2)
			{
				mData.offset = r2;
			}
		}

		ImGui::DragFloat("blur_dacay##Bloom", &mData.decay, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("blur_intensity##Bloom", &mData.blurIntensity, 0.01f, 0.0f, 1.0f);
		if (ImGui::DragFloat("blur_offset##Bloom", &mData.offset, 0.1f, 1.0f, 10.0f))
		{
			if (mData.radiation > 60.0f * mData.u_step * mData.offset)
			{
				mData.radiation = 60.0f * mData.u_step * mData.offset;
			}
			else if (mData.radiation < 20.0f * mData.u_step * mData.offset)
			{
				mData.radiation = 20.0f * mData.u_step * mData.offset;
			}
		}
		ImGui::Text("or");
		ImGui::Image(mRT_Origin.GetShaderResourceView(), { 320.0f,180.0f });
		ImGui::Text("bl");
		ImGui::Image(mRT_Bloom.GetShaderResourceView(), { 320.0f,180.0f });
		ImGui::Text("cm");
		ImGui::Image(mRT_Combine.GetShaderResourceView(), { 320.0f,180.0f });

 	}

}
