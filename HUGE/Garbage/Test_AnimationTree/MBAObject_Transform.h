#pragma once

#include <HUGE/Inc/HUGE.h>

class MBAO_Transform
{
public:
	MBAO_Transform();
	void SetPosition(const H::Math::Vector3& v);
	void SetRotation(const H::Math::Vector3& v);
	void SetScale(const H::Math::Vector3& v);
 
	void AddPosition(const H::Math::Vector3& v);
	void AddRotation(const H::Math::Vector3& v);
	void AddScale(const H::Math::Vector3& v);

	void ComputeTransformMat();

	H::Math::Matrix4 GetPosition() const;
	H::Math::Matrix4 GetRotation() const;
	H::Math::Matrix4 GetScale() const;

	H::Math::Matrix4 GetTransformMatrix() const;

	void DebugUI();

	H::Math::Vector3 mPosition;
	H::Math::Vector3 mRotation;
	H::Math::Vector3 mScale{1.0f};

private:
	H::Math::Matrix4 mTransformMatrix;
};

 