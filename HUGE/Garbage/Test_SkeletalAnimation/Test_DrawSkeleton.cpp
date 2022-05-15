#include "Test_DrawSkeleton.h"
#include "Graphics/Src/MeshIO.h"

void Test_DrawSkeleton::Init()
{
	H::Graphics::MeshIO::LoadSkeleton(mSkeleton, PATH_SKELETON);
	for (size_t i = 0; i < mSkeleton.bones.size(); i++)
	{
		mBoneMatrices.emplace_back(Matrix4::Identity());
	}
	
	mLineSkeleton.Init();
}

void Test_DrawSkeleton::Terminate()
{
}

void Test_DrawSkeleton::Render()
{
	mLineSkeleton.Render();
}

void Test_DrawSkeleton::Update(float dt)
{
}

void Test_DrawSkeleton::DeBugUI()
{
	mLineSkeleton.DeBugUI();
}
