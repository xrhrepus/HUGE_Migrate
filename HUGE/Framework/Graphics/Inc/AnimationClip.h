#ifndef HUGE_GRAPHICS_ANIMATIONCLIP_H
#define HUGE_GRAPHICS_ANIMATIONCLIP_H

#include "Animation.h"

namespace H::Graphics
{
	class AnimationClip
	{
	public:
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;

		std::string name;
		float duration = 0.0f;
		float tickPerSecond = 0.0f;
		BoneAnimations boneAnimations;
 
	};
	using AnimationClips = std::vector<std::unique_ptr< AnimationClip>>;
 
}
#endif // !HUGE_GRAPHICS_ANIMATIONCLIP_H

