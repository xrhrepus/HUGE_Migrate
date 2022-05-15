#include "MBAObject_Transform.h"

H::Math::Matrix4 MBAO_Transform::GetTransformMatrix() const
{
	return mTransformMatrix;
	
}

void MBAO_Transform::DebugUI()
{
	ImGui::Begin("Translate");
	bool changed = false;
	if (ImGui::DragFloat3("pos", mPosition.v.data(), 0.2f))
		changed = true;
	if (ImGui::DragFloat3("rot", mRotation.v.data(), 0.2f))
	{
 		changed = true;
	}
	
	if(ImGui::DragFloat3("sca", mScale.v.data(), 0.2f))
		changed = true;
	
 
	if (ImGui::Button("reset"))
	{
		mPosition = H::Math::Vector3::Zero();
		mRotation = H::Math::Vector3::Zero();
		mScale = H::Math::Vector3{1.0f};
 		changed = true;

	}
		ComputeTransformMat();

	if (changed)
	{
	}
	ImGui::End();
}

MBAO_Transform::MBAO_Transform()
{
}

void MBAO_Transform::SetPosition(const H::Math::Vector3 & v)
{
	mPosition = v;
}

void MBAO_Transform::SetRotation(const H::Math::Vector3 & v)
{
	mRotation = v;
}

void MBAO_Transform::SetScale(const H::Math::Vector3 & v)
{
	mScale = v;
}

 
void MBAO_Transform::AddPosition(const H::Math::Vector3 & v)
{
	mPosition += v;
}

void MBAO_Transform::AddRotation(const H::Math::Vector3 & v)
{
	mRotation += v;
}

void MBAO_Transform::AddScale(const H::Math::Vector3 & v)
{
	mScale += v;
}

void MBAO_Transform::ComputeTransformMat()
{
	mTransformMatrix =
		//H::Math::MatrixRotationQuaternion(QuaternionFromToRotation(H::Math::Normalize({ mPrevDirection.x,0.0f,mPrevDirection.z }), H::Math::Normalize({ mDirection.x,0.0f,mDirection.z })))*
		//H::Math::MatrixRotationQuaternion(QuaternionFromToRotation(H::Math::Vector3::zAxis(), H::Math::Normalize({ mDirection.x,0.0f,mDirection.z })))*

		H::Math::MatrixRotationQuaternion(H::Math::QuaternionRotationAxis(H::Math::Vector3::yAxis(), H::Math::Constants::DegToRad * mRotation.y))*
		H::Math::Matrix4::scaling(mScale)*
		H::Math::Matrix4::translation(mPosition);

}
