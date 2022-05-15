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

	void ComputeBonePositionRecursive(Bone* bone, const Matrix4& parentTransform, std::vector<H::Math::Vector3>& bonePos)
	{
		for (size_t i = 0; i < bone->children.size(); i++)
		{
			Matrix4 currTrans = bone->children[i]->toParentTransform *parentTransform ;
 			bonePos[bone->children[i]->index] = currTrans * Vector3::Zero();

 			ComputeBonePositionRecursive(bone->children[i], currTrans, bonePos);
		}
	}
	void UpdateTransforms(Bone* bone, std::vector<H::Math::Matrix4>& bonematrix)
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

 void LineSkeleton::Init(std::filesystem::path filename)
{
	H::Graphics::MeshIO::LoadSkeleton(mSkeleton, filename);
 
	size_t numbone = mSkeleton.bones.size();
	mBonePos.resize(numbone);
	ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);
	for (size_t i = 0; i < numbone; i++)
	{
		//mBoneMatrix.emplace_back(Matrix4::Identity());
		mBoneTransform.emplace_back(Vector3::Zero());
		mBoneRotation.emplace_back(Vector3::Zero());
	}
	
  
 }

void LineSkeleton::Terminate()
{
}

 
void LineSkeleton::Render( )
{
	for (size_t i = 0; i < mBonePos.size(); i++)
	{
		const Vector3& currnode = mBonePos[i];
		SimpleDraw::AddSolidBox(H::Math::AABB{ currnode,1.0f }, Colors::Red);
		//DrawBoneRecursive(mSkeleton.root,mSkeleton.root->toParentTransform);

		for (size_t j = 0; j < mSkeleton.bones[i]->childIndices.size(); j++)
		{
			
			{
				SimpleDraw::AddLine(currnode, mBonePos[mSkeleton.bones[i]->childIndices[j]], Colors::Yellow);
			}
		}
 	}

	SimpleDraw::AddLine(Vector3{-20.0f,0.0f, 80.0f},  mBonePos[mBone_currChoose] , Colors::Cyan);
}

void LineSkeleton::Update(float dt)
{
}

void LineSkeleton::DeBugUI()
{
	ImGui::Begin("LineSkeleton");
	ImGui::Text("current bone : ");
	ImGui::SameLine();
	if (ImGui::ArrowButton("prev", 0)) mBone_currChoose -= mBone_currChoose > 0 ? 1 : 0;
	ImGui::SameLine();
	ImGui::Text(" % d ", mBone_currChoose);
	ImGui::SameLine();
	if (ImGui::ArrowButton("next", 1)) mBone_currChoose += mBone_currChoose < mBonePos.size() - 1 ? 1 : 0;


	Vector3 prevPos = mBoneTransform[mBone_currChoose];
	if (ImGui::DragFloat("translate x ", &mBoneTransform[mBone_currChoose].x,0.1f))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = Matrix4::translation({ mBoneTransform[mBone_currChoose]  - prevPos }) * mSkeleton.bones[mBone_currChoose]->toParentTransform;

		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);

		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];

		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

	}
	if (ImGui::DragFloat("translate y ", &mBoneTransform[mBone_currChoose].y, 0.1f))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = Matrix4::translation({ mBoneTransform[mBone_currChoose] - prevPos}) * mSkeleton.bones[mBone_currChoose]->toParentTransform;
		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);
		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

	}
	if (ImGui::DragFloat("translate z ", &mBoneTransform[mBone_currChoose].z, 0.1f))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = Matrix4::translation({ mBoneTransform[mBone_currChoose]- prevPos}) * mSkeleton.bones[mBone_currChoose]->toParentTransform;
		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);
		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

	}

	Vector3 prevRot = mBoneRotation[mBone_currChoose];
 	if (ImGui::DragFloat("rotation x", &mBoneRotation[mBone_currChoose].x, 0.01f/*,-H::Math::Constants::Pi, H::Math::Constants::Pi*/))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = H::Math::MatrixRotationQuaternion(H::Math::QuaternionRotationAxis(Vector3::xAxis(), mBoneRotation[mBone_currChoose].x - prevRot.x)) * mSkeleton.bones[mBone_currChoose]->toParentTransform;
		/*mSkeleton.bones[mBone_currChoose]->toParentTransform = ((mSkeleton.bones[mBone_currChoose]->offsetTransform))*
			Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(Matrix4::Identity() * Vector3::xAxis(), mBoneRotation[mBone_currChoose].x - prevRot.x)) *
			H::Math::Inverse(mSkeleton.bones[mBone_currChoose]->offsetTransform) * mSkeleton.bones[mBone_currChoose]->toParentTransform;
*/
		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);

		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);


	}
	if (ImGui::DragFloat("rotation y", &mBoneRotation[mBone_currChoose].y, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = H::Math::MatrixRotationQuaternion(H::Math::QuaternionRotationAxis(Vector3::yAxis(), mBoneRotation[mBone_currChoose].y - prevRot.y)) * mSkeleton.bones[mBone_currChoose]->toParentTransform;

 
		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);
		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

	}
	if (ImGui::DragFloat("rotation z", &mBoneRotation[mBone_currChoose].z, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
	{
		mSkeleton.bones[mBone_currChoose]->toParentTransform = H::Math::MatrixRotationQuaternion(H::Math::QuaternionRotationAxis(Vector3::zAxis(), mBoneRotation[mBone_currChoose].z - prevRot.z)) * mSkeleton.bones[mBone_currChoose]->toParentTransform;

 
		UpdateTransforms(mSkeleton.bones[mBone_currChoose].get(), mBoneMatrix);
		//mBonePos[mBone_currChoose] = mBoneMatrix[mBone_currChoose] * mBonePos[mBone_currChoose];
		ComputeBonePositionRecursive(mSkeleton.root, mSkeleton.root->toParentTransform, mBonePos);

	}


	ImGui::End();
}
