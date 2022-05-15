#include "LineBone.h"

namespace
{
	void UpdateTransforms(const Bone* bone, std::vector<H::Math::Matrix4>& bonematrix/*, const Matrix4& animTF*/)
	{
		if (bone->parent)
		{
			bonematrix[bone->index] = (bone->toParentTransform   * bonematrix[bone->parentIndex])/* *bonematrix[bone->index]*/;
		}
		else
		{
			bonematrix[bone->index] = bone->toParentTransform /* * bonematrix[bone->index]*/;
		}

		for (auto child : bone->children)
			UpdateTransforms(child, bonematrix);
	}
}

void LineBone::Initialize()
{
}

void LineBone::Terminate()
{
}

void LineBone::Render()
{
	 
}

void LineBone::DrawBone(const H::Math::Vector3& from, const H::Math::Vector3& to) const
{
	SimpleDraw::AddShpere({ from,mSize }, mColor);
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
	SimpleDraw::AddLine(square[0], from, mColor);
	SimpleDraw::AddLine(square[1], from, mColor);
	SimpleDraw::AddLine(square[2], from, mColor);
	SimpleDraw::AddLine(square[3], from, mColor);
	//
	SimpleDraw::AddShpere({ to,mSize }, mColor);

}

void LineBone::DrawBoneRecursive(H::Graphics::Bone* root) const
{
	/*if (root->children.empty())
	{
		DrawBone()
		return;
	}*/
}

 

void LineBone::Update(float deltatime)
{
}

void LineBone::DebugUI()
{
	if (ImGui::CollapsingHeader("LineBone"))
	{
		
	}
}
