#include "Precompiled.h"
#include "AnimationClip.h"

using namespace H::Graphics;

bool H::Graphics::AnimationClip::GetTransform(float time, uint32_t boneIndex, Math::Matrix4 & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->animation.GetTransform(time);
		return true;
	}
	return false;

}
