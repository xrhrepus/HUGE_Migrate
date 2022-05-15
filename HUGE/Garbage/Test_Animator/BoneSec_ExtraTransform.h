#pragma once
#include <Math/Inc/HUGEMath.h>
#include "C_Transform.h"

class TAnimator;
struct BoneSec_ExtraTransform
{
	BoneSec_ExtraTransform(TAnimator& anmtor, std::vector<Matrix4>& boneMatrices);

	C_Transform transform;
	int boneSec_Index = 0;
	bool active = false;
	TAnimator& animator;

	void Update(float dt);
	void ChangeTargetBoneSecTransform(int index, const H::Math::Matrix4& mat) const;
	void DebugUI();

	//look at obj
	Vector3 lookAtTargetPos;
	void ComputeTransform(const Vector3& charLookat, const Vector3& charPos,float maxAngleRad);
	
	void DebugDraw() const;
	std::vector<Matrix4>& boneMats;
	bool startLookAt = false;
	float maxAngleRad = 1.0f;
 };
