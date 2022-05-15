#include "TPostProcess.h"

void TPostProcess::Initialize()
{
	auto gSys = GraphicSystem::Get();

	mScreenMesh = MeshBuilder::CreateNDCQuadPX();
	mScreenMeshBuffer.Initialize(mScreenMesh);

	mOriginVS.Initialize(0,POSTPROCESSING_FILE_PATH.c_str());
	mOriginPS.Initialize(POSTPROCESSING_FILE_PATH.c_str(), "PS");
	mOriginRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);


	mBloomVS.Initialize(0, POSTPROCESSING_BLOOM_FILE_PATH.c_str());
	mBloomPS.Initialize(POSTPROCESSING_BLOOM_FILE_PATH.c_str(), "PS");
	mBloomRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);

	//mBloomMidVS.Initialize(0, POSTPROCESSING_MID_FILE_PATH.c_str());
	//mBloomMidPS.Initialize(POSTPROCESSING_MID_FILE_PATH.c_str(), "PS");
	//mBloomMidRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);


	mCombineVS.Initialize(0, POSTPROCESSING_COMBINE_FILE_PATH.c_str());
	mCombinePS.Initialize(POSTPROCESSING_COMBINE_FILE_PATH.c_str(), "PS");
	mCombineRT.Initialize(gSys->GetBackBufferWidth(), gSys->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);


	{
		mBloomBuffer.Initialize(mBloomData);
		mBloomData.baseIntensity = 1.0f;
		mBloomData.baseSaturation = 1.0f;
		mBloomData.bloomIntensity = 1.0f;
		mBloomData.bloomSaturation = 1.0f;


		mBloomData.u_step = 1.0f / GraphicSystem::Get()->GetBackBufferWidth();
		mBloomData.v_step = 1.0f / GraphicSystem::Get()->GetBackBufferHeight();
		mBloomData.decay = 0.01f;
		mBloomData.blurIntensity = 0.8f;
		mBloomData.radiation = 0.08f;
		mBloomData.offset = 5.0f;
	}
}

void TPostProcess::Terminate()
{
	mScreenMeshBuffer.Terminate();
	mOriginRT.Terminate();
	mOriginVS.Terminate();
	mOriginPS.Terminate();
	mBloomVS.Terminate();
	mBloomPS.Terminate();
	mBloomRT.Terminate();

	//mBloomMidVS.Terminate();
	//mBloomMidPS.Terminate();
	//mBloomMidRT.Terminate();

	mCombineVS.Terminate();
	mCombinePS.Terminate();
	mCombineRT.Terminate();
	mBloomBuffer.Terminate();
}

void TPostProcess::Begin()
{
	mOriginRT.BeginRender();
	BlendStateManager::Get()->GetBlendState(Blend::AlphaBlend)->Set();
	SamplerManager::Get()->GetSampler(SamplerType::PointClamp)->BindPS(0);

}

void TPostProcess::End()
{
	mOriginRT.EndRender();
}

void TPostProcess::Render()
{
	//mOriginVS.Bind();
	//mOriginPS.Bind();
	//mOriginRT.BindPS(0);
	//mScreenMeshBuffer.Render();

	//mBloomVS.Bind();
	//mBloomPS.Bind();
	//
	if (isBloomOn)
	{
		mBloomRT.BeginRender();
		mOriginRT.BindPS(0);
		mBloomVS.Bind();
		mBloomPS.Bind();
		mBloomBuffer.Set(mBloomData);
		mBloomBuffer.BindPS();
		mScreenMeshBuffer.Render();
		mBloomRT.EndRender();
	}
	/*{
		mBloomRT.BeginRender();
		mOriginRT.BindPS(0);
		mBloomMidVS.Bind();
		mBloomMidPS.Bind();
		mBloomBuffer.Set(mBloomData);
		mBloomBuffer.BindPS();
		mScreenMeshBuffer.Render();
		mBloomRT.EndRender();
	}*/

	//
	mCombineRT.BeginRender();
	mOriginRT.BindPS(0);
	mBloomRT.BindPS(1);
	mCombineVS.Bind();
	mCombinePS.Bind();
	mScreenMeshBuffer.Render();
	mCombineRT.EndRender();

	//
 	mCombineRT.BindPS(0);
	mScreenMeshBuffer.Render();

}

void TPostProcess::DebugUI()
{
	if (ImGui::CollapsingHeader("Post processing"))
	{
		ImGui::TextDisabled("[------Bloom---------]");
		ImGui::Checkbox("On/Off##Bloom", &isBloomOn);
		ImGui::DragFloat("threshold##Bloom", &mBloomData.bloomThreshold, 0.01f, 0.0f, 0.999f);
		ImGui::DragFloat("bloom_intensity##Bloom", &mBloomData.bloomIntensity, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("bloom_saturation##Bloom", &mBloomData.bloomSaturation, 0.01f, 0.0f, 1.0f);

		ImGui::TextDisabled("[------SimpleBlur---------]");
		if (ImGui::DragFloat("blur_radiation##Bloom", &mBloomData.radiation, 0.001f, 0.01f, 1.0f))
		{
			float r = mBloomData.radiation / (60.0f * mBloomData.u_step);
			float r2 = mBloomData.radiation / (20.0f * mBloomData.u_step);

			if (mBloomData.offset < r)
			{
				mBloomData.offset = r;
			}
			else if (mBloomData.offset > r2)
			{
				mBloomData.offset = r2;
			}
		}

		ImGui::DragFloat("blur_dacay##Bloom", &mBloomData.decay, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("blur_intensity##Bloom", &mBloomData.blurIntensity, 0.01f, 0.0f, 1.0f);
		if (ImGui::DragFloat("blur_offset##Bloom", &mBloomData.offset, 0.1f, 1.0f, 10.0f))
		{
			if (mBloomData.radiation > 60.0f * mBloomData.u_step * mBloomData.offset)
			{
				mBloomData.radiation = 60.0f * mBloomData.u_step * mBloomData.offset;
			}
			else if (mBloomData.radiation < 20.0f * mBloomData.u_step * mBloomData.offset)
			{
				mBloomData.radiation = 20.0f * mBloomData.u_step * mBloomData.offset;
			}
		}

		//ImGui::TextDisabled("[------BaseScene---------]");
		//ImGui::DragFloat("base_intensity##Bloom", &mBloomData.baseIntensity, 0.01f, 0.0f, 1.0f);
		//ImGui::DragFloat("base_saturation##Bloom", &mBloomData.baseSaturation, 0.01f, 0.0f, 1.0f);
	}

}
