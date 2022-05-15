#include "TDirectionalLight.h"

void TDirectionalLight::Initialize()
{
	{
		mLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
		mLight.specular = { 0.8f,0.8f,0.8f,1.0f };
		mLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
		mLight.direction = H::Math::Normalize({ 0.0f,0.0f,1.0f });
	}
	
	mDirection = H::Math::Normalize({ 0.0f,-0.5f,0.5f });
	mDirection = H::Math::Normalize(mDirection);

	mLightCam.SetDirection(mDirection);
	mLight.direction = mDirection;

}

void TDirectionalLight::Update(float dt)
{
}

void TDirectionalLight::DebugDraw() const
{
	SimpleDraw::AddBox({ mPosition,5.0f }, Colors::Cyan);
	SimpleDraw::AddLine(mPosition, mPosition + mDirection, Colors::Blue);
}

void TDirectionalLight::DebugUI()
{
	ImGui::Begin("TDirectionalLight");
	if (ImGui::CollapsingHeader("Direction##TDirectionalLight"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Direction##TDirectionalLight", &mDirection.x, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Direction##TDirectionalLight", &mDirection.y, 0.25f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Direction##TDirectionalLight", &mDirection.z, 0.25f, -1.0f, 1.0f);

		if (directionChanged)
		{
			mDirection = H::Math::Normalize(mDirection);
			mLightCam.SetDirection(mDirection);
			mLight.direction = mDirection;
		}

	}
	if (ImGui::CollapsingHeader("Light##TDirectionalLight"))
	{

		ImGui::ColorEdit4("Ambient##Direction##TDirectionalLight", &mLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Direction##TDirectionalLight", &mLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Direction##TDirectionalLight", &mLight.specular.r);


	}
	if (ImGui::CollapsingHeader("Views##TDirectionalLight"))
	{

	}
	ImGui::End();
}
