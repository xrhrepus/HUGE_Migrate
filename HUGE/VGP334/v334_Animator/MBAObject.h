#pragma once
#include <HUGE/Inc/HUGE.h>
#include "TAnimator.h"

//struct TransformData;
//using TransformBuffer = TypedConstantBuffer<TransformData>;

class MBAObject
{
public:
	void Init();
	void Terminate();
	void Update(float deltatime);

	void Render( );
	void SwitchAnimation(int animIndex);
	void UpdateAnimationTimer(float deltatime);
	void DebugUI();


private:
	const wchar_t* PATH_MODEL = L"../../Assets/Meshes/goofyrun.model";
	const wchar_t* PATH_SKELETON = L"../../Assets/Meshes/goofyrun.skeleton";
	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Meshes/goofyrun.animset";
	const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Meshes/idle.animset";
	const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Meshes/sitidle.animset";
	const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Meshes/happy.animset";

private:
	Matrix4 mWorldMat = Matrix4::Identity();
	
	////animation
	//AnimationClips mAnimClips;
	//std::vector<std::filesystem::path> mAnimFile_Paths;
	//size_t mNumAnimClips = 0;
	//int mCurrChooseAnim = 0;

	//bool mPlayAnim = false;
	//bool mAnimTransitOn = false;

	//float mAnimTimer = 0.0f;//current anim progress
	//float mAnimPlayTime = 0.0f;//current anim duration

	////transition
	//bool animChanged = false;
	//bool mAnimInTransition = false;
	//float mAnimTransitStart = 0.9f;//percentage of current anim duration
	//float mPrevAnimProgress = 0.0f;//duration remained by prev anim
	//float mAnimTransitStep = 0.0f;//steps from 0 ~ 1
	//float mAnimTransitProgress = 0.0f;//ratio 0 ~ 1

	//AnimationClip* mCurrAnimClip = nullptr;
	//AnimationClip* mPrevAnimClip = nullptr;


	//model
	Model mModel;
	Skeleton mSkeleton;
	std::vector<Matrix4> mBoneMatrices;

	H::Graphics::BoneTransformBuffer boneTransformBuf;


	//test animator
	TAnimator mAnimator{mSkeleton};

};

 