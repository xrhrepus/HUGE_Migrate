#pragma once
#include <HUGE/Inc/HUGE.h>
#include "TAnimator.h"
#include "MBAObject_Transform.h"
 
class MBAObject
{
public:
	void Init();
	void Terminate();
	void Update(float deltatime);

	void Render(const H::Graphics::TransformData& tfdata,const H::Graphics::DirectionalLight& light, const H::Graphics::Material& material);
	void SwitchAnimation(int animIndex);
	void UpdateAnimationTimer(float deltatime);
	void DebugUI();

	void Control(const Vector3& camDir, float dt);
	void Move(const Vector3& camDir, float dt);

private:
	const wchar_t* PATH_MODEL = L"../../Assets/Meshes/goofyrun.model";
	const wchar_t* PATH_SKELETON = L"../../Assets/Meshes/goofyrun.skeleton";

	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Meshes/goofyrun.animset";
	const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Meshes/idle.animset";
	const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Meshes/sitidle.animset";
	const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Meshes/happy.animset";

private:
	Matrix4 mWorldMat = Matrix4::Identity();
 
	//model
	Model mModel;
	Skeleton mSkeleton;
	std::vector<Matrix4> mBoneMatrices;

	BoneTransformBuffer boneTransformBuf;


	//test animator
	TAnimator mAnimator{mSkeleton};


public:
	H::Math::Vector3 mPrevDirection{ 0.0f,0.0f,1.0f };
	H::Math::Vector3 mDirection{ 0.0f,0.0f,1.0f };

	//test translate
	MBAO_Transform mTransform;

	float mSpeed = 1.0f;
	H::Math::Vector3 mVelocity;
};

 