#pragma once
#include "LineSkeleton.h"
#include <HUGE/Inc/HUGE.h>

class Test_DrawSkeleton
{
public:
	void Init();
  	void Terminate();


	void Render();
	void Update(float dt);
	void DeBugUI();
	const wchar_t* PATH_SKELETON = L"../../Assets/Models/Movement/idle.skeleton";

private:
	std::vector<H::Math::Matrix4> mBoneMatrices;
	Skeleton mSkeleton;
	LineSkeleton mLineSkeleton{ mBoneMatrices, mSkeleton };
};
 