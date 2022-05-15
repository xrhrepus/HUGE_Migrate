#include "BoneSec_ExtraTransform.h"
#include "TAnimator.h"

BoneSec_ExtraTransform::BoneSec_ExtraTransform(TAnimator & anmtor)
	:animator(anmtor)
{
}

void BoneSec_ExtraTransform::Update(float dt)
{
	if (active)
	{
		ChangeTargetBoneSecTransform(boneSec_Index, transform.GetTransformMatrix());
	}
}

void BoneSec_ExtraTransform::ChangeTargetBoneSecTransform(int index, const H::Math::Matrix4 & mat)const
{
	animator.ChangeExtraMatrix(index, mat);
}

void BoneSec_ExtraTransform::DebugUI()
{
	if (ImGui::CollapsingHeader("BoneSec_ExtraTransform"))
	{
		ImGui::Checkbox("Always apply", &active);
		transform.DebugUI();
 
		ImGui::Columns(3);
		for (size_t i = 0; i < animator.GetAnimationClipsNum(); i++)
		{
			char b[32]; sprintf_s(b, "%d", i);
			if (ImGui::Button(b))
			{
				boneSec_Index = i;
			}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		//ImGui::DragInt("target index", &boneSec_Index, 1.0f, 0, animator.GetAnimationClipsNum()-1);
		if (ImGui::Button("Manually Change transform"))
		{
			ChangeTargetBoneSecTransform(boneSec_Index, transform.GetTransformMatrix());
		}
		
	}
}
