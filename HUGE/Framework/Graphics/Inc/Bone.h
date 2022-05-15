#ifndef HUGE_GRAPHICS_BONE_H
#define HUGE_GRAPHICS_BONE_H
#include "Common.h"

namespace H::Graphics
{
	struct Bone
	{
		std::string name;
		int index;


		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childIndices;

		Math::Matrix4 toParentTransform;
		Math::Matrix4 offsetTransform;
	};

}//namespace H::Graphics

#endif // !HUGE_GRAPHICS_BONE_H

