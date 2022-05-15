#include "TDirection.h"
#include <HUGE/Inc/HUGE.h>
using namespace H;

void TDirection::Rotating(float dt)
{
	mRotationTimer += dt * mRotationSpeed;
	mRotationProgress = mRotationTimer / mRotationTime;
	dir = H::Math::Lerp(startdir, targetDir, mRotationProgress);
	if (mRotationTimer >= mRotationTime)
	{
		mRotationTimer = 0.0f;
		mRotationProgress = 0.0f;
		mRotating = false;
	}
	
}

//H::Math::Matrix4 TDirection::ComputeRotationMat() const
//{
// 	Quaternion q = H::Math::QuaternionFromToRotation(prevDir, dir);
//	q = H::Math::Normalize(q);
//	auto m = H::Math::MatrixRotationQuaternion(q);
//	return m;
//}

H::Math::Matrix4 TDirection::ComputeRotationMat() const
{
	Quaternion q = H::Math::QuaternionFromToRotation(dir, targetDir);
	q = H::Math::Normalize(q);
	auto m = H::Math::MatrixRotationQuaternion(q);
	return m;
}

void TDirection::ComputeRotation()
{
	targetDir = targetDrawPos;
	targetDir = H::Math::Normalize(targetDir);
 
 }

void TDirection::Update(float dt)
{
}

void TDirection::DebugUI()
{
	if (ImGui::CollapsingHeader("TDirection"))
	{
		//ImGui::DragFloat3("dir", dir.v.data());
		ImGui::Text("Direction:[ %f,%f,%f ]", dir.x, dir.y, dir.z);
		ImGui::Text("Target Direction:[ %f,%f,%f ]", targetDir.x, targetDir.y, targetDir.z);

 		ImGui::DragFloat3("targetPos", targetDrawPos.v.data());
		ImGui::DragFloat("rotation speed", &mRotationSpeed, 0.1f, 0.0f,10.0f);
		ImGui::DragFloat("rotation time", &mRotationTime, 0.1f, 0.0f,20.0f);
		ImGui::Text("Progress"); ImGui::SameLine();
		ImGui::ProgressBar(mRotationProgress);
		if (mRotating)
			ImGui::Text("[ %f / %f ]", mRotationTimer, mRotationTime);


		if (ImGui::Button("Rotate"))
		{
			ComputeRotation();
		}

	}

	
 }

void TDirection::DebugDraw()
{
	SimpleDraw::AddLine( dir * 30.0f , Vector3(0.0f), Colors::OrangeRed);
	SimpleDraw::AddLine(targetDir * 30.0f, Vector3(0.0f), Colors::LawnGreen);
	SimpleDraw::AddSolidBox({ targetDrawPos,3.0f }, Colors::Pink);
	SimpleDraw::AddBox({ dir * 30.0f,5.0f }, Colors::LemonChiffon);


}
