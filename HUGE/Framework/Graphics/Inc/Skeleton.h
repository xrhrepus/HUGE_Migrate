#ifndef HUGE_GRAPHICS_SKELETON_H
#define HUGE_GRAPHICS_SKELETON_H
#include "Bone.h"


namespace H::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<H::Graphics::Bone>> bones;
	};
}

#endif // !HUGE_GRAPHICS_SKELETON_H

