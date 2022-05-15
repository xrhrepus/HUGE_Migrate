#pragma once
#include <HUGE/Inc/HUGE.h>
#include "TAnimator.h"
#include "C_Transform.h"
#include "TDirection.h"
#include "BoneSec_ExtraTransform.h"
#include "LineSkeleton.h"

//struct TransformData;
//using TransformBuffer = TypedConstantBuffer<TransformData>;


class MBAObject
{
public:
	void Init();
	void Terminate();
	void Update(float deltatime);

	void Render(const Matrix4& viewMat, const Matrix4& projMat, const Camera& cam);
	void SwitchAnimation(int animIndex);
	void UpdateAnimationTimer(float deltatime);
	void DebugUI();

	LineSkeleton& GetLineSkeleton();
private:
	const wchar_t* PATH_MODEL = L"../../Assets/Meshes/goofyrun.model";
	//const wchar_t* PATH_MODEL = L"../../Assets/Models/BH3model/debugtestd2.model";

	const wchar_t* PATH_SKELETON = L"../../Assets/Meshes/goofyrun.skeleton";
	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Meshes/goofyrun.animset";
	const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Meshes/idle.animset";
	const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Meshes/sitidle.animset";
	const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Meshes/happy.animset";

private:
	//
	DirectionalLight mLight;
	Material  mMaterial;

	Matrix4 mWorldMat = Matrix4::Identity();
	C_Transform mTransform;
	bool mUseTDirection = false;
	TDirection mDirection;
	H::Graphics::ShaderEffect_Skinning::SE_Context_Skinning mSkinningCtx;
 
	//model
	bool mDrawModel = true;
	Model mModel;
	Skeleton mSkeleton;
	std::vector<Matrix4> mBoneMatrices;

	H::Graphics::BoneTransformBuffer boneTransformBuf;


	//test animator
	TAnimator mAnimator{mSkeleton};
	BoneSec_ExtraTransform mBoneSecExtraTransform{mAnimator};
	LineSkeleton mLineSkeleton{ mBoneMatrices,mSkeleton };
};

 