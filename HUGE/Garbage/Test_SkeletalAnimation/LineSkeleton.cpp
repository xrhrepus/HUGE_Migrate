#include "LineSkeleton.h"
#include "Graphics/Src/MeshIO.h"
using namespace H;
using namespace H::Math;
namespace 
{
	void DrawBoneRecursive(Bone* bone, const Matrix4& parentTransform)
	{
		for (size_t i = 0; i < bone->children.size(); i++)
		{
			Matrix4 currTrans = parentTransform * bone->children[i]->toParentTransform;
			//SimpleDraw::AddAABB(H::Math::AABB{ parentTransform * Vector3::Zero(),1.0f }, Colors::Red);

			SimpleDraw::AddLine(parentTransform * Vector3::Zero(), currTrans  * Vector3::Zero(),Colors::Yellow);
			DrawBoneRecursive(bone->children[i], currTrans);
		}
	}

	void ComputeBonePositionRecursive(const Bone* bone, const Matrix4& parentTransform, std::vector<H::Math::Vector3>& bonePos)
	{
		for (size_t i = 0; i < bone->children.size(); i++)
		{
			Matrix4 currTrans = bone->children[i]->toParentTransform *parentTransform ;
 			bonePos[bone->children[i]->index] = currTrans * Vector3::Zero();

 			ComputeBonePositionRecursive(bone->children[i], currTrans, bonePos);
		}
	}
	void UpdateTransforms(const Bone* bone, std::vector<H::Math::Matrix4>& bonematrix)
	{
		if (bone->parent)
		{
 			bonematrix[bone->index] =  bone->toParentTransform * bonematrix[bone->parentIndex];
		}
		else
		{
 			bonematrix[bone->index] =  bone->toParentTransform;
		}

		for (auto child : bone->children)
			UpdateTransforms(child, bonematrix);
	}

 
 }
 

LineSkeleton::LineSkeleton(std::vector<H::Math::Matrix4>& boneMats, Skeleton & Skeleton)
	:mSkeleton(Skeleton),mBoneMatrices(boneMats)
{
 
}

void LineSkeleton::Init()
 {
	mNumBones = mSkeleton.bones.size();
	mBonePos.resize(mNumBones);
	ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);
 	
  }

 void LineSkeleton::Terminate()
{
 }

 
void LineSkeleton::Render( )
{
	if (draw)
	{
		for (size_t i = 0; i < mBonePos.size(); i++)
		{
			const Vector3& currnode = mBonePos[i];
			SimpleDraw::AddSolidBox(H::Math::AABB{ currnode,1.0f }, Colors::Red);
			//DrawBoneRecursive(mSkeleton.root,mSkeleton.root->toParentTransform);

			for (size_t j = 0; j < mSkeleton.bones[i]->childIndices.size(); j++)
			{

				if (lineMode)
				{
					SimpleDraw::AddLine(currnode, mBonePos[mSkeleton.bones[i]->childIndices[j]], Colors::Yellow);
				}
				else
				{
					DrawBone(currnode, mBonePos[mSkeleton.bones[i]->childIndices[j]]);
				}

			}
		}
		SimpleDraw::AddBox({ mBonePos[mBone_currChoose],5.0f }, Colors::Cyan);

	}
  }

void LineSkeleton::Update(float dt)
{
	if (updating)
	{
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);
	}
}

void LineSkeleton::DeBugUI()
{
	if (ImGui::CollapsingHeader("LineSkeleton"))
	{
		ImGui::Checkbox("Updating skeleton##LineSkeleton", &updating);
		ImGui::Checkbox("Draw skeleton##LineSkeleton", &draw);
		ImGui::Checkbox("Line / Cone ##LineSkeleton", &lineMode);

		ImGui::DragInt("current bone",&mBone_currChoose,1.0f,0.0f, mNumBones);
		if (ImGui::ArrowButton("prev", 0)) mBone_currChoose -= mBone_currChoose > 0 ? 1 : 0;
		ImGui::SameLine();
		ImGui::Text(" % d ", mBone_currChoose);
		ImGui::SameLine();
		if (ImGui::ArrowButton("next", 1)) mBone_currChoose += mBone_currChoose < mNumBones - 1 ? 1 : 0;


 	//	ImGui::Checkbox("Enable Edit", &enableEdit);
		//if (enableEdit)
		//{
		//	mTransformCompute.DebugUI();
		//	if (ImGui::Button("Apply Edit"))
		//	{
		//		mSkeleton.bones[mBone_currChoose]->toParentTransform = mTransformCompute.GetTransformMatrix() * mSkeleton.bones[mBone_currChoose]->toParentTransform;
		//		//mBoneMatrix[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mTransformCompute.GetTransformMatrix();
		//		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);
		//		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

		//	}

		//}
  
 	}

 

 }

void LineSkeleton::DrawBone(const H::Math::Vector3 & from, const H::Math::Vector3 & to) const
{
	SimpleDraw::AddBox({ from,mSize }, mColor);
	const Vector3 fromTo = H::Math::Normalize(to - from);
	const Vector3 right = H::Math::Normalize(H::Math::Vector3::yAxis().Cross(fromTo));
	const Vector3 up = H::Math::Normalize(right.Cross(fromTo));
	const Vector3 square[] =
	{
		from + ((right * -1.0f + up) * mSize),
		from + ((right + up) * mSize),
		from + ((right * -1.0f - up) * mSize),
		from + ((right - up) * mSize)
	};
	//
	SimpleDraw::AddLine(square[0], square[1], mColor);
	SimpleDraw::AddLine(square[0], square[2], mColor);
	SimpleDraw::AddLine(square[3], square[1], mColor);
	SimpleDraw::AddLine(square[3], square[2], mColor);
	//
	SimpleDraw::AddLine(square[0], to, mColor);
	SimpleDraw::AddLine(square[1], to, mColor);
	SimpleDraw::AddLine(square[2], to, mColor);
	SimpleDraw::AddLine(square[3], to, mColor);
	//
	//SimpleDraw::AddBox({ to,mSize }, mColor);

}
