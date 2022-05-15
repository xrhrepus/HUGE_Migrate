#include "MBAObject.h"
#include "Graphics/Src/MeshIO.h"
using namespace H;
namespace
{
	void UpdateTransforms(const Bone* bone, std::vector<H::Math::Matrix4>& bonematrix/*, const Matrix4& animTF*/)
	{
		if (bone->parent)
		{
			bonematrix[bone->index] =  (bone->toParentTransform   * bonematrix[bone->parentIndex] )/* *bonematrix[bone->index]*/;
		}
		else
		{
			bonematrix[bone->index] =    bone->toParentTransform /* * bonematrix[bone->index]*/;
		}

		for (auto child : bone->children)
			UpdateTransforms(child, bonematrix );
	}

	void BlendAnimTransforms(  Matrix4& from,   Matrix4& to, float frompercent)
	{
		from *= (1 - frompercent);
		from._14 = 0.0f; from._24 = 0.0f; from._34 = 0.0f; from._44 = 1.0f;

		to *= (frompercent);
		to._14 = 0.0f; to._24 = 0.0f; to._34 = 0.0f; to._44 = 1.0f;
	}
}
void MBAObject::Init()
{
 
	mModel.Initialize(PATH_MODEL);
	MeshIO::LoadSkeleton(mSkeleton, PATH_SKELETON);

	//mBoneMatrices.resize(mSkeleton.bones.size());
	for (size_t i = 0; i < mSkeleton.bones.size(); i++)
	{
		mBoneMatrices.emplace_back(Matrix4::Identity());
	}
	UpdateTransforms(mSkeleton.root, mBoneMatrices);

	boneTransformBuf.Initialize();

 
	mAnimator.Init();
}

void MBAObject::Terminate()
{
	mModel.Terminate();
	
}

void MBAObject::Update(float deltatime)
{
 
	mAnimator.Update(deltatime);

}
void MBAObject::Render()
{
 

 	boneTransformBuf.Set(mAnimator.ComputeToParentTransform(mBoneMatrices));
	boneTransformBuf.BindVS(4);
	boneTransformBuf.BindPS(4);

	//render
	mModel.Render();
}

 

void MBAObject::DebugUI()
{
	 
	mAnimator.DebugUI();
}
