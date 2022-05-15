#include "BoneSec_ExtraTransform.h"
#include "TAnimator.h"

BoneSec_ExtraTransform::BoneSec_ExtraTransform(TAnimator & anmtor, std::vector<Matrix4>& boneMatrices)
	:animator(anmtor),boneMats(boneMatrices)
{
}

void BoneSec_ExtraTransform::Update(float dt)
{
	if (active)
	{
		ChangeTargetBoneSecTransform(boneSec_Index, transform.GetTransformMatrix());
	}

	if (startLookAt)
	{
		int idx = animator.mAnimBlending.mSections[boneSec_Index].mRootOfSectionIndex;
 
		Vector3 rootOfSec = H::Math::TransformCoord(Vector3{0.0f}, boneMats[idx]);
		SimpleDraw::AddBox({ rootOfSec,30.0f }, Colors::Cyan);
		ComputeTransform( - Vector3::zAxis(), rootOfSec, maxAngleRad);
	}


}

void BoneSec_ExtraTransform::ChangeTargetBoneSecTransform(int index, const H::Math::Matrix4 & mat)const
{
	animator.ChangeExtraMatrix(index, mat);
}

void BoneSec_ExtraTransform::DebugUI()
{
	DebugDraw();


	if (ImGui::CollapsingHeader("BoneSec_ExtraTransform"))
	{
		ImGui::Checkbox("On/Off", &active);
		transform.DebugUI();
		ImGui::Columns(3);
		for (size_t i = 0; i < animator.GetAnimationClipsNum(); i++)
		{
			char b[32]; sprintf_s(b, "%d", i);
			if (ImGui::Button(b))
			{
				boneSec_Index = i;
				active = false;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::Text("Look at object");
		if (ImGui::Checkbox("looking at", &startLookAt))
		{
			if (!startLookAt)
			{
				transform.rotationQuaternion = Quaternion::Identity();
			}

		}

		ImGui::DragFloat("maxAngle", &maxAngleRad, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("testPOs", lookAtTargetPos.v.data(), 0.1f);
		//if (ImGui::Button("Looking at object"))
		//{
		//	startLookAt = true;
		//}


		//ImGui::DragInt("target index", &boneSec_Index, 1.0f, 0, animator.GetAnimationClipsNum()-1);
		if (ImGui::Button("Manually Change transform"))
		{
			ChangeTargetBoneSecTransform(boneSec_Index, transform.GetTransformMatrix());
		}
		
	}
}

void BoneSec_ExtraTransform::ComputeTransform(const Vector3& charLookat, const Vector3& charPos, float maxAngleRad)
{
	Vector3 char2Tar = H::Math::Normalize(lookAtTargetPos - charPos);
	float dot = char2Tar.Dot(charLookat);
	if (dot > maxAngleRad)
	{
		transform.rotationQuaternion = 
		/*H::Math::MatrixRotationQuaternion*/(H::Math::QuaternionFromToRotation(charLookat, char2Tar));
	}
	else
	{
		transform.rotationQuaternion =
			Quaternion::Identity();
	}

}

void BoneSec_ExtraTransform::DebugDraw() const
{
	SimpleDraw::AddBox({ lookAtTargetPos,5.0f }, Colors::Yellow);
}
 
