#pragma once
#include <HUGE/Inc/HUGE.h>
//struct BoneTransformData
//{
//	Matrix4 boneTransform[128];
//};
//using  BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;

struct Animator_Transition
{

	bool mAnimTransitOn = false;

	//transition
	bool mAnimInTransition = false;
	float mAnimTransitStart = 0.9f;//percentage of current anim duration
	float mPrevAnimProgress = 0.0f;//duration remained by prev anim
	float mAnimTransitStep = 0.0f;//steps from 0 ~ 1
	float mAnimTransitProgress = 0.0f;//ratio 0 ~ 1

	AnimationClip* mCurrAnimClip = nullptr;
	AnimationClip* mPrevAnimClip = nullptr;

 };
struct Bone_Section
{
	//Animator_Blending& mAB;
	//Bone_Section() = default;
	Bone_Section(AnimationClips& animClips, std::string name);
	std::string mName;

	bool mPlayAnim = true;
	bool mLoopAnim = true;
	bool animChanged = false;

	std::vector<Bone*> mBoneRefs;//a ref to all bones belong to this section
	float mAnimTimer = 0.0f;//current anim progress
	float mAnimPlayTime = 0.0f;//current anim duration

	int mCurrChooseAnim_Index = 0;
	AnimationClips& mAnimClipsRef;
	Animator_Transition mATransition;

	void RegisterBoneToSection(Bone* bone);
	void Update_ToparentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats) const;
	void SwitchAnimation(int animIndex);
	void UpdateAnimTimer(float deltatime);
	void Update(float deltatime);

};
struct Animator_Blending
{
	//animation blending
	bool mBlendAnim = false;
	int mCurrentSection_index = 0;
	//create for each section
	//std::vector<float> mBA_timers;
	//std::vector<float> mBA_playTimes;
	//std::vector<Animator_Transition*> mATransitions;
	std::vector<Bone_Section> mSections;

	void UpdateBoneSection_ToParentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats);
	void UpdateTimer(float deltatime);
	void Update(float deltatime);
 };

//imgui file
const int file_path_buffer_size = 50;

class TAnimator
{
public:
	TAnimator(Skeleton& skeleton);

	void Init();
	void Terminate();
	void Update(float deltatime);
	BoneTransformData ComputeToParentTransform(std::vector<Matrix4>& boneMats);
	void SwitchAnimation(int animIndex);
	void UpdateAnimationTimer(float deltatime);
	void DebugUI();

	//blending
	void CreateSection();
	void CreateSection(AnimationClips& animClips, std::string name);
	void RegisterBoneToSection(Bone * bone,int sec_index);
	void Update_Section_ToparentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats);

private:
	//imgui load file
	char in_anim_file_path[file_path_buffer_size];

 	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Meshes/goofyrun.animset";
	const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Meshes/idle.animset";
	const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Meshes/sitidle.animset";
	const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Meshes/happy.animset";
	const wchar_t* PATH_ANIMCLIPS_5 = L"../../Assets/Meshes/cowmilk.animset";
	const wchar_t* PATH_ANIMCLIPS_6 = L"../../Assets/Meshes/sprintleft.animset";
	//import from imgui test
	const wchar_t* PATH_ROOT = L"../../Assets/Meshes/";
	const wchar_t* ANIMCLIPS = L"goofyrun.animset";
	const wchar_t* ANIMCLIPS_2 = L"idle.animset";
	const wchar_t* ANIMCLIPS_3 = L"sitidle.animset";
	const wchar_t* ANIMCLIPS_4 = L"happy.animset";

private:
	//skeleton
	Skeleton& mSkeleton;

	//animation
	AnimationClips mAnimClips;
	std::vector<std::filesystem::path> mAnimFile_Paths;
	size_t mNumAnimClips = 0;
	int mCurrChooseAnim_Index = 0;


	bool mPlayAnim = false;
	bool mLoopAnim = false;
	bool animChanged = false;

	//bool mAnimTransitOn = false;

	float  mAnimTimer = 0.0f;//current anim progress
	float  mAnimPlayTime = 0.0f;//current anim duration

	//float mHardcode_AnimTimer = 0.0f;//current anim progress
	//float mHardcode_AnimPlayTime = 0.0f;//current anim duration


	//transition
 

	//Animator_Transition mHardCode_Transition;
	Animator_Transition mTransition;

	//animation blending
	Animator_Blending mAnimBlending;
	//int mCurrChooseBoneSection;
 
 

};

 