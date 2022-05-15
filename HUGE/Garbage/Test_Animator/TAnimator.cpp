#include "TAnimator.h"
#include "Graphics/Src/MeshIO.h"

using namespace H::Graphics;
using namespace H::Math;

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

TAnimator::TAnimator(Skeleton & skeleton)
	:mSkeleton(skeleton)
{
}

void TAnimator::Init()
{
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS);
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_2);
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_3);
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_4);
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_5);
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_6);

	mNumAnimClips = mAnimFile_Paths.size();
	mAnimClips.resize(mNumAnimClips);
	for (size_t i = 0; i < mNumAnimClips; i++)
	{
		
		MeshIO::LoadAnimationClip(mAnimClips[i], mAnimFile_Paths[i]);

	}

	//CreateSection(mAnimClips, "entirebody");
	//
	//RegisterBoneToSection((mSkeleton.bones[54]).get(), 0);
	//CreateSection(mAnimClips, "rightarm");
	//RegisterBoneToSection((mSkeleton.bones[25]).get(), 0);
	//CreateSection(mAnimClips, "leftarm");
	//RegisterBoneToSection((mSkeleton.bones[44]).get(), 0);
	//CreateSection(mAnimClips, "upperbody");
	//RegisterBoneToSection((mSkeleton.bones[49]).get(), 0);
	//CreateSection(mAnimClips, "lowerbody");
	//RegisterBoneToSection((mSkeleton.bones[51]).get(), 0);
	//CreateSection(mAnimClips, "head");
	//RegisterBoneToSection((mSkeleton.bones[47]).get(), 5);//neck

	//hard code section
	CreateSection(mAnimClips, "entirebody");
	
	for (size_t i = 0; i < mSkeleton.bones.size(); i++)
	{
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 0);
	}
	mAnimBlending.mSections[0].mRootOfSectionIndex = 54;

	CreateSection(mAnimClips, "rightarm");
	for (size_t i = 8; i < 27; i++)
	{
 
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 1);
	}
	mAnimBlending.mSections[1].mRootOfSectionIndex = 25;


	CreateSection(mAnimClips, "leftarm");
	for (size_t i = 27; i < 45; i++)
	{
 
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 2);
	}
	mAnimBlending.mSections[2].mRootOfSectionIndex = 44;

	CreateSection(mAnimClips, "upperbody");
	for (size_t i = 45; i < mSkeleton.bones.size(); i++)
	{
		if (i== 51 || i == 55 || i == 54)
		{
			continue;
		}
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 3);
	}
	mAnimBlending.mSections[3].mRootOfSectionIndex = 49;

	CreateSection(mAnimClips, "lowerbody");
	for (size_t i = 0; i < 8; i++)
	{
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 4);
	}
	RegisterBoneToSection((mSkeleton.bones[51]).get(), 4);
	mAnimBlending.mSections[4].mRootOfSectionIndex = 51;

	CreateSection(mAnimClips, "head");
	RegisterBoneToSection((mSkeleton.bones[46]).get(), 5);
	RegisterBoneToSection((mSkeleton.bones[47]).get(), 5);//neck
	RegisterBoneToSection((mSkeleton.bones[55]).get(), 5);
	mAnimBlending.mSections[5].mRootOfSectionIndex = 47;

 // 
}

void TAnimator::Terminate()
{
}

void TAnimator::Update(float deltatime)
{
	if (mAnimClips.size() > 0)
	{
		if (mAnimBlending.IsActive())
		{
			mAnimBlending.Update(deltatime);
		}
 
		UpdateAnimationTimer(deltatime);

	}
	
}

BoneTransformData TAnimator::ComputeToParentTransform(std::vector<Matrix4>& boneMats)
{
	BoneTransformData btd;

	const size_t numBones = mSkeleton.bones.size();
 	if(mAnimBlending.IsActive())
	{
		Update_Section_ToparentTransform(btd, boneMats);
		UpdateTransforms(mSkeleton.root, boneMats);
 	}
	else//if no bone or anim
	{
		for (size_t i = 0; i < numBones; i++)
		{
			btd.boneTransform[i] = H::Math::Transpose(mSkeleton.bones[i]->offsetTransform *  boneMats[i]);
		}
	}
	return btd;
}
 

void TAnimator::UpdateAnimationTimer(float deltatime)
{
	if (mAnimBlending.IsActive())
	{
		mAnimBlending.UpdateTimer(deltatime);
	}
 	
}

void TAnimator::DebugUI()
{
	ImGui::Begin("animation");
 
	mAnimBlending.DebugUI();
  
  
	ImGui::End();
}
 
void TAnimator::CreateSection(const AnimationClips& animClips, std::string name)
{
	mAnimBlending.CreateSection(animClips, name);
}

void TAnimator::RegisterBoneToSection(Bone * bone, int sec_index)
{
	if (sec_index > mAnimBlending.mSections.size())
	{
		return;
	}
	mAnimBlending.mSections[sec_index].RegisterBoneToSection(bone);
}

void TAnimator::Update_Section_ToparentTransform(BoneTransformData & btd, std::vector<Matrix4>& boneMats)
{
	mAnimBlending.UpdateBoneSection_ToParentTransform(btd, boneMats);
}

void TAnimator::ChangeExtraMatrix(int boneSecIndex, const Matrix4 & mat)
{
	mAnimBlending.mSections[boneSecIndex].mExtraTransform = mat;
}

Bone_Section::Bone_Section(const AnimationClips& animClips, std::string name)
	:mAnimClipsRef(animClips),mName(name), mDebugUIName(("Bone_Section: " + name))
{
	//mExtraTransform = H::Math::MatrixRotationAxis(Vector3::xAxis(), 0.5f * H::Math::Constants::Pi);
	mExtraTransform = Matrix4::Identity();

	SwitchAnimation(0);
	//mATransition.mCurrAnimClip = mAnimClipsRef[0].get();
}

void Bone_Section::RegisterRootBoneToSection(const Bone * root)
{
	mRootOfSectionIndex = root->index;
}

void Bone_Section::RegisterBoneToSection(Bone * bone)
{
	mBoneRefs.push_back(bone);
}

void Bone_Section::Update_ToparentTransform(BoneTransformData& btd,std::vector<Matrix4>& boneMats) const
{
	const size_t numBones = mBoneRefs.size();
	for (size_t i = 0; i < numBones; i++)
	{
		Matrix4 animTf = mBoneRefs[i]->toParentTransform;
		const int currBoneIndex = mBoneRefs[i]->index;
		if (mATransition.mCurrAnimClip != nullptr)
		{
			if (mATransition.mAnimInTransition)
			{
				Vector3  pp, cp;
				Vector3 ps{ 1.0f }, cs{ 1.0f };
				Quaternion pr{ 0.0f,0.0f,0.0f,1.0f }, cr{ 0.0f,0.0f,0.0f,1.0f };
				auto& pba = mATransition.mPrevAnimClip->boneAnimations[currBoneIndex];
				if (pba != nullptr)
				{
					ps = pba->animation.GetScaleValue(mATransition.mPrevAnimProgress);
					pr = pba->animation.GetRotationValue(mATransition.mPrevAnimProgress);
					pp = pba->animation.GetPositionValue(mATransition.mPrevAnimProgress);
				}
				auto& cba = mATransition.mCurrAnimClip->boneAnimations[currBoneIndex];

				if (cba != nullptr)
				{
					cs = cba->animation.GetScaleValue(mAnimTimer);
					cr = cba->animation.GetRotationValue(mAnimTimer);
					cp = cba->animation.GetPositionValue(mAnimTimer);

				}
				float oneminus = (1.0f - mATransition.mAnimTransitProgress);
				ps = cs * mATransition.mAnimTransitProgress + ps * (oneminus);
				pr = cr * mATransition.mAnimTransitProgress + pr * (oneminus);
				pp = cp * mATransition.mAnimTransitProgress + pp * (oneminus);

				animTf = MatrixRotationQuaternion(pr);
				animTf._41 = pp.x;
				animTf._42 = pp.y;
				animTf._43 = pp.z;
				animTf._11 *= ps.x;
				animTf._22 *= ps.y;
				animTf._33 *= ps.z;

				mBoneRefs[i]->toParentTransform = animTf;

			}
			else
			{

				if (mATransition.mCurrAnimClip->GetTransform(mAnimTimer, currBoneIndex, animTf))
				{
					//mBoneRefs[i]->toParentTransform =  mExtraTransform *  animTf;//<============
					mBoneRefs[i]->toParentTransform = mRootOfSectionIndex == mBoneRefs[i]->index ? mExtraTransform * animTf : animTf;//<============

				}
			}

		}
		btd.boneTransform[currBoneIndex] = H::Math::Transpose( mBoneRefs[i]->offsetTransform *   boneMats[currBoneIndex] /** mExtraTransform*/);
	}
}

void Bone_Section::SwitchAnimation(int animIndex)
{
	if (mATransition.mAnimTransitOn)
	{
		//save progress remained by prev anim
		mATransition.mPrevAnimProgress = mAnimTimer;
		//reset progress for current anim
		mAnimTimer = 0.0f;
		//compute animTransit step for animTransit progress
		mATransition.mAnimTransitStep = 1.0f / (mAnimPlayTime - mATransition.mPrevAnimProgress);
		//reset animTransit progress
		mATransition.mAnimTransitProgress = 0.0f;
		//transition start
		mATransition.mAnimInTransition = true;
	}

	//change index
	mCurrChooseAnim_Index = animIndex;

	//reset pointer
	mATransition.mPrevAnimClip = mATransition.mCurrAnimClip;
	mATransition.mCurrAnimClip = mAnimClipsRef[mCurrChooseAnim_Index].get();
	//reset anim timer
	mAnimPlayTime = mATransition.mCurrAnimClip->duration;
}

void Bone_Section::UpdateAnimTimer(float deltatime)
{
	if (mPlayAnim)
	{
		mAnimTimer += deltatime * mATransition.mCurrAnimClip->tickPerSecond;

		if (mATransition.mAnimInTransition)
		{
			mATransition.mPrevAnimProgress += deltatime * mATransition.mPrevAnimClip->tickPerSecond;
			mATransition.mAnimTransitProgress += deltatime * mATransition.mPrevAnimClip->tickPerSecond * mATransition.mAnimTransitStep;
		}
	}

	//if transition done
	if (mATransition.mAnimTransitProgress >= 1.0f)
		mATransition.mAnimInTransition = false;

	//reset timer
	if (mAnimTimer > mAnimPlayTime)
	{
		mAnimTimer = 0.0f;
		mPlayAnim = mLoopAnim ? true : false;
	}
}

void Bone_Section::Update(float deltatime)
{
	if (animChanged)
	{
		if (mATransition.mAnimTransitOn)
		{
			float prog = mAnimTimer / mAnimPlayTime;
			if (prog >= mATransition.mAnimTransitStart)
			{
				SwitchAnimation(mCurrChooseAnim_Index);
				animChanged = false;
			}
		}
		else
		{
			SwitchAnimation(mCurrChooseAnim_Index);
			animChanged = false;
		}
	}
}

void Bone_Section::ResetTimer()
{
	mATransition.ResetTimer();
	mAnimTimer = 0.0f;//current anim progress
}

void Bone_Section::Reset()
{
	mATransition.Reset();
	mCurrChooseAnim_Index = 0;
	mAnimTimer = 0.0f;//current anim progress
	mPlayAnim = false;
	mLoopAnim = false;
	SwitchAnimation(0);

	mActive = false;
}

void Animator_Blending::SetActive(bool active)
{
	mActive = active;

	if (mAnimclipRef.empty())
	{
		mActive = false;
	}
}

bool Animator_Blending::IsActive() const
{
	return mActive;
}

void Bone_Section::DebugUI()
{
 	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild(("Anim Progress: ##" + mDebugUIName).c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 260), true, window_flags);
 
		ImGui::Text("CurrChooseAnim: [%d]  %s", mCurrChooseAnim_Index, mAnimClipsRef[mCurrChooseAnim_Index]->name.c_str());

		ImGui::DragFloat(("progress##" + mDebugUIName).c_str(), &mAnimTimer, 0.1f, 0.0f, mAnimPlayTime);
		char buf[32];
		sprintf_s(buf, "%f/%f", mAnimTimer, mAnimPlayTime);
		ImGui::ProgressBar(mAnimTimer / mAnimPlayTime, ImVec2(0.0f, 0.0f), buf);

 		ImGui::Checkbox(("On/Off##" + mDebugUIName).c_str(), &mActive);
		ImGui::SameLine();
		ImGui::Checkbox(("play##" + mDebugUIName).c_str(), &mPlayAnim);
		ImGui::SameLine();
		ImGui::Checkbox(("loop##" + mDebugUIName).c_str(), &mLoopAnim);
		ImGui::SameLine();
		ImGui::Checkbox(("transition##" + mDebugUIName).c_str(), &mATransition.mAnimTransitOn);
		if (ImGui::Button(("Reset##" + mDebugUIName).c_str()))
			Reset();
		ImGui::SameLine();
		if (ImGui::Button(("ResetTimer##" + mDebugUIName).c_str()))
			ResetTimer();

		mATransition.DebugUI();

		ImGui::EndChild();
		ImGui::PopStyleVar();
 	}

}

Animator_Blending::Animator_Blending(const AnimationClips & animclipRef)
	:mAnimclipRef(animclipRef)
{
}

void Animator_Blending::CreateSection(const AnimationClips & animClips, std::string name)
{
	mSections.emplace_back(Bone_Section(animClips, name));
}

void Animator_Blending::UpdateBoneSection_ToParentTransform(BoneTransformData& btd,std::vector<Matrix4>& boneMats)
{
	for (const auto& bs : mSections )
	{
		if (bs.mActive)
		{
			bs.Update_ToparentTransform(btd, boneMats);
		}
	}
}

void Animator_Blending::UpdateTimer(float deltatime)
{
	for (auto& sec : mSections)
	{
		if (sec.mActive)
			sec.UpdateAnimTimer(deltatime);
	}
}

void Animator_Blending::Update(float deltatime)
{
	for (auto& sec : mSections)
	{
		if (sec.mActive)
			sec.Update(deltatime);
	}

}

void Animator_Blending::ResetTimer()
{
	for (auto& bonesec : mSections)
	{
		bonesec.ResetTimer();
	}
}

void Animator_Blending::Reset()
{
	for (auto& bonesec : mSections)
	{
		bonesec.Reset();
	}

}

void Animator_Blending::DebugUI()
{
	if (ImGui::CollapsingHeader("Animator_Blending"))
	{
		ImGui::Checkbox("On/Off", &mActive);
		if (ImGui::Button("Reset All Sections##Animator_Blending"))
			Reset();
		if (ImGui::Button("ResetTimer of All Sections##Animator_Blending"))
			ResetTimer();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("BoneSection Control"))
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton /*| ImGuiTabBarFlags_FittingPolicyScroll*/;
			if (ImGui::BeginTabBar("BoneSections##BoneSection Controls##Animator_Blending", tab_bar_flags))
			{
				for (size_t i = 0; i < mSections.size(); i++)
				{
					char buf[32]; sprintf_s(buf, "[%03d]: %s", i, mSections[i].mName.c_str());
					if (ImGui::BeginTabItem(buf))
					{
						ImGui::Text(buf);
						mCurrentSection_index = i;
						mSections[i].DebugUI();
						ImGui::EndTabItem();
					}

				}
				ImGui::EndTabBar();
			}

			ImGui::Separator();
			if (ImGui::BeginTabBar("AnimClips##BoneSection Controls##Animator_Blending", 0))
			{
				if (ImGui::BeginTabItem("Animation Clips##BoneSection Controls##Animator_Blending"))
				{
					char buf[32];
					ImGui::Columns(3);
					for (size_t i = 0; i < mAnimclipRef.size(); i++)
					{
						sprintf_s(buf, "[%03d] ", i);
						const auto& ac = mAnimclipRef[i];
						if (ImGui::Button((buf + ac->name).c_str()))
						{
							auto& s = mSections[mCurrentSection_index];
							s.animChanged |= true;
							s.mCurrChooseAnim_Index = i;
						}
						ImGui::NextColumn();

					}
					ImGui::Columns(1);
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

		}
 
 
  
 	//	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;

		//if(!mSections.empty())
		//{
		//	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		//	ImGui::BeginChild("Bone Sections##Sections##Animator_Blending", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 130), true, window_flags);

		//	char buf[32];
		//	//1 select bone secs
		//	for (size_t i = 0; i < mSections.size(); i++)
		//	{
		//		sprintf_s(buf, "[%03d] ", i);
		//		auto& sec = mSections[i];
		//		if (ImGui::Button((buf + sec.mName).c_str()))
		//		{
		//			mCurrentSection_index = i;
		//		}
		//	}

		//	ImGui::EndChild();
		//	ImGui::PopStyleVar();
		//}
 
 
	}
	

}

 
void Animator_Transition::ResetTimer()
{
	mAnimInTransition = false;
 	mPrevAnimProgress = 0.0f;
	mAnimTransitStep = 0.0f;
	mAnimTransitProgress = 0.0f;
}

void Animator_Transition::Reset()
{
	mAnimTransitOn = false;
	ResetTimer();
}

 

void Animator_Transition::DebugUI()
{
	if (ImGui::TreeNode("Animator_Transition"))
	{
 		ImGui::DragFloat("Anim Transition Start", &mAnimTransitStart,0.1f,0.0f,1.0f);
 		ImGui::Text("AnimTransitProgress"); ImGui::SameLine(); ImGui::ProgressBar(mAnimTransitProgress);

		ImGui::TreePop();
	}
}
