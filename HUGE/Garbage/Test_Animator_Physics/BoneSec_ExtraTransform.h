#pragma once
#include <Math/Inc/HUGEMath.h>
#include "C_Transform.h"

class TAnimator;
struct BoneSec_ExtraTransform
{
	BoneSec_ExtraTransform(TAnimator& anmtor);

	C_Transform transform;
	int boneSec_Index = 0;
	bool active = false;
	TAnimator& animator;

	void Update(float dt);
	void ChangeTargetBoneSecTransform(int index, const H::Math::Matrix4& mat) const;
	void DebugUI();
};
