#pragma once
#include <HUGE/Inc/HUGE.h>
 
//struct BoneTransformData
//{
//	Matrix4 boneTransform[128];
//};
//using  BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;
struct BoneSec_ExtraTransform;
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

	void ResetTimer();
	void Reset();

	void DebugUI();

 };
struct Bone_Section
{
 	Bone_Section(const AnimationClips& animClips, std::string name);
	std::string mName;

 	bool mActive = false;
 	bool mPlayAnim = false;
	bool mLoopAnim = false;
	bool animChanged = false;

	int mRootOfSectionIndex = -1;//Index of the root of this section
	std::vector<Bone*> mBoneRefs;//a ref to all bones belong to this section
	float mAnimTimer = 0.0f;//current anim progress
	float mAnimPlayTime = 0.0f;//current anim duration

	int mCurrChooseAnim_Index = 0;
	const AnimationClips& mAnimClipsRef;
	Animator_Transition mATransition;
	
	//
 	Matrix4 mExtraTransform;

	//methods
	void RegisterRootBoneToSection(const Bone* root);
	void RegisterBoneToSection(Bone* bone);
	void Update_ToparentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats) const;
	void SwitchAnimation(int animIndex);
	void UpdateAnimTimer(float deltatime);
	void Update(float deltatime);

	void ResetTimer();
	void Reset();


	//DebugUI
	std::string mDebugUIName;
	void DebugUI();

};
struct Animator_Blending
{
public:
	Animator_Blending(const AnimationClips& animclipRef);
 	const AnimationClips& mAnimclipRef;


	//animation blending
private:
	bool mActive = false;

public:
	int mCurrentSection_index = 0;
 
	std::vector<Bone_Section> mSections;


	void CreateSection(const AnimationClips& animClips, std::string name);

	void UpdateBoneSection_ToParentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats);
	void UpdateTimer(float deltatime);
	void Update(float deltatime);
	void ResetTimer();
	void Reset();


	void SetActive(bool active);
	bool IsActive()const;

 	void DebugUI();
 };

//imgui file
const int file_path_buffer_size = 50;

class TAnimator
{
public:
	TAnimator(Skeleton& skeleton);

	//method
	void Init();
	void Terminate();
	void Update(float deltatime);
	BoneTransformData ComputeToParentTransform(std::vector<Matrix4>& boneMats);
	void SwitchAnimation(int animIndex);
	void UpdateAnimationTimer(float deltatime);
	void DebugUI();
	//get set
	inline int GetAnimationClipsNum()const { return mNumAnimClips; }

	//blending
	void CreateSection(const AnimationClips& animClips, std::string name);
	void RegisterBoneToSection(Bone * bone,int sec_index);
	void Update_Section_ToparentTransform(BoneTransformData& btd, std::vector<Matrix4>& boneMats);

	void ChangeExtraMatrix(int boneSecIndex, const Matrix4& mat);

private:
#pragma region filepath
	//imgui load file
	char in_anim_file_path[file_path_buffer_size];

 //	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Meshes/goofyrun.animset";
	//const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Meshes/idle.animset";
	//const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Meshes/sitidle.animset";
	//const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Meshes/happy.animset";
	//const wchar_t* PATH_ANIMCLIPS_5 = L"../../Assets/Meshes/cowmilk.animset";
	//const wchar_t* PATH_ANIMCLIPS_6 = L"../../Assets/Meshes/sprintleft.animset";
	const wchar_t* PATH_ANIMCLIPS = L"../../Assets/Models/Movement/idle.animset";
	const wchar_t* PATH_ANIMCLIPS_2 = L"../../Assets/Models/Movement/attack.animset";
	const wchar_t* PATH_ANIMCLIPS_3 = L"../../Assets/Models/Movement/walking.animset";
	const wchar_t* PATH_ANIMCLIPS_4 = L"../../Assets/Models/Movement/running.animset";
	const wchar_t* PATH_ANIMCLIPS_5 = L"../../Assets/Meshes/goofyrun.animset";
	const wchar_t* PATH_ANIMCLIPS_6 = L"../../Assets/Models/Movement/die.animset";

	//import from imgui test
	const wchar_t* PATH_ROOT = L"../../Assets/Meshes/";
	const wchar_t* ANIMCLIPS = L"goofyrun.animset";
	const wchar_t* ANIMCLIPS_2 = L"idle.animset";
	const wchar_t* ANIMCLIPS_3 = L"sitidle.animset";
	const wchar_t* ANIMCLIPS_4 = L"happy.animset";
#pragma endregion

private:
 	//skeleton
	Skeleton& mSkeleton;

	//animation
	AnimationClips mAnimClips;
	std::vector<std::filesystem::path> mAnimFile_Paths;
	size_t mNumAnimClips = 0;
 

	//animation blending
	friend struct BoneSec_ExtraTransform;
	Animator_Blending mAnimBlending{mAnimClips};
  

};

 