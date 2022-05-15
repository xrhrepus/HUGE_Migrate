#pragma once
#include <Math/Inc/HUGEMath.h>
class TDirection
{
public:
	void Update(float dt);
	void DebugUI();
	void DebugDraw();

	//return a rotation matrix between dir of prev frame and curr frame
	H::Math::Matrix4 ComputeRotationMat() const;
	//H::Math::Matrix4 ComputeRotationMat(const H::Math::Vector3& from, const H::Math::Vector3& to) const;

 
	bool mRotating = false;//rotation in progress?
private:
	void ComputeRotation();
	void Rotating(float dt);
 
	
public:
	float mRotationSpeed = 1.0f;//rotation speed
	float mRotationProgress = 0.0f;//% of curr rotation progress
	float mRotationTime = 1.0f;//time to finish this rotation
	float mRotationTimer = 0.0f;//curr rotation progress


	H::Math::Vector3 prevDir{ 0.0f, 0.0f, 1.0f };
	H::Math::Vector3 dir{0.0f, 0.0f, 1.0f};

	H::Math::Vector3 startdir{ 0.0f, 0.0f, 1.0f };
	H::Math::Vector3 targetDir{ 0.0f, 0.0f, 1.0f };
	H::Math::Vector3 targetDrawPos{ 0.0f, 20.0f, 20.0f };

};

 