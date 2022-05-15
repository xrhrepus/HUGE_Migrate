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
	:mSkeleton(skeleton)/*, mTransition(mHardCode_Transition),
	mAnimTimer(mHardcode_AnimTimer),mAnimPlayTime(mHardcode_AnimPlayTime)*/
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
	mAnimFile_Paths.push_back(PATH_ANIMCLIPS_7);

	mNumAnimClips = mAnimFile_Paths.size();
	mAnimClips.resize(mNumAnimClips);
	for (size_t i = 0; i < mNumAnimClips; i++)
	{
		
		MeshIO::LoadAnimationClip(mAnimClips[i], mAnimFile_Paths[i]);

	}
	if (mNumAnimClips > 0)
	{
		mTransition.mCurrAnimClip = mAnimClips[mCurrChooseAnim_Index].get();
		mAnimPlayTime = mAnimClips[mCurrChooseAnim_Index]->duration;
 	}

	//hard code section
	CreateSection(mAnimClips, "entirebody");
	
	for (size_t i = 0; i < mSkeleton.bones.size(); i++)
	{
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 0);
		
	}
	CreateSection(mAnimClips, "rightarm");
	for (size_t i = 8; i < 27; i++)
	{
 
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 1);
	}
	CreateSection(mAnimClips, "leftarm");
	for (size_t i = 27; i < 45; i++)
	{
 
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 2);
	}

	CreateSection(mAnimClips, "upperbody");
	for (size_t i = 45; i < mSkeleton.bones.size(); i++)
	{
		if (i==51 || i == 51)
		{
			continue;
		}
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 3);
	}

	CreateSection(mAnimClips, "lowerbody");
	for (size_t i = 0; i < 8; i++)
	{
		RegisterBoneToSection((mSkeleton.bones[i]).get(), 4);
	}
	RegisterBoneToSection((mSkeleton.bones[51]).get(), 4);
	RegisterBoneToSection((mSkeleton.bones[54]).get(), 4);


	mAnimBlending.mSections[0].mATransition.mCurrAnimClip = mTransition.mCurrAnimClip;
	mAnimBlending.mSections[1].mATransition.mCurrAnimClip = mTransition.mCurrAnimClip;
	mAnimBlending.mSections[2].mATransition.mCurrAnimClip = mTransition.mCurrAnimClip;
	mAnimBlending.mSections[3].mATransition.mCurrAnimClip = mTransition.mCurrAnimClip;
	mAnimBlending.mSections[4].mATransition.mCurrAnimClip = mTransition.mCurrAnimClip;
	

}

void TAnimator::Terminate()
{
}

void TAnimator::Update(float deltatime)
{
	if (mAnimClips.size() > 0)
	{
		if (mAnimBlending.mBlendAnim)
		{
			mAnimBlending.Update(deltatime);
		}

		if (animChanged)
		{
			if (mTransition.mAnimTransitOn)
			{
				float prog = mAnimTimer / mAnimPlayTime;
				if (prog >= mTransition.mAnimTransitStart)
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
		UpdateAnimationTimer(deltatime);

	}
}

void TAnimator::PlayAnim(int index)
{
	animChanged |= true;
	mCurrChooseAnim_Index = index;
}

BoneTransformData TAnimator::ComputeToParentTransform(std::vector<Matrix4>& boneMats)
{
	BoneTransformData btd;

	const size_t numBones = mSkeleton.bones.size();
	if (numBones >0 && mTransition.mCurrAnimClip != nullptr/*mAnimClips.size() > 0*/ && !mAnimBlending.mBlendAnim)
	{
		for (size_t i = 0; i < numBones; i++)
		{
			Matrix4 animTf = mSkeleton.bones[i]->toParentTransform;

			if (mTransition.mAnimInTransition)
			{
				Vector3  pp, cp;
				Vector3 ps{ 1.0f }, cs{ 1.0f };
				Quaternion pr{ 0.0f,0.0f,0.0f,1.0f }, cr{ 0.0f,0.0f,0.0f,1.0f };
				auto& pba = mTransition.mPrevAnimClip->boneAnimations[i];
				if (pba != nullptr)
				{
					ps = pba->animation.GetScaleValue(mTransition.mPrevAnimProgress);
					pr = pba->animation.GetRotationValue(mTransition.mPrevAnimProgress);
					pp = pba->animation.GetPositionValue(mTransition.mPrevAnimProgress);
				}
				auto& cba = mTransition.mCurrAnimClip->boneAnimations[i];

				if (cba != nullptr)
				{
					cs = cba->animation.GetScaleValue(mAnimTimer);
					cr = cba->animation.GetRotationValue(mAnimTimer);
					cp = cba->animation.GetPositionValue(mAnimTimer);

				}
				float oneminus = (1.0f - mTransition.mAnimTransitProgress);
				ps = cs * mTransition.mAnimTransitProgress + ps * (oneminus);
				pr = cr * mTransition.mAnimTransitProgress + pr * (oneminus);
				pp = cp * mTransition.mAnimTransitProgress + pp * (oneminus);

				animTf = MatrixRotationQuaternion(pr);
				animTf._41 = pp.x;
				animTf._42 = pp.y;
				animTf._43 = pp.z;
				animTf._11 *= ps.x;
				animTf._22 *= ps.y;
				animTf._33 *= ps.z;

				mSkeleton.bones[i]->toParentTransform = animTf;

			}
			else
			{

				if (mTransition.mCurrAnimClip->GetTransform(mAnimTimer, i, animTf))
				{
					mSkeleton.bones[i]->toParentTransform = animTf;
				}
			}
			btd.boneTransform[i] = H::Math::Transpose(mSkeleton.bones[i]->offsetTransform *  boneMats[i]);
		}

	}
	else if(mAnimBlending.mBlendAnim && mTransition.mCurrAnimClip != nullptr)
	{
		Update_Section_ToparentTransform(btd, boneMats);
		//mAnimBlending.UpdateBoneSection_ToParentTransform(btd, boneMats);
	}
	else//if no bone or anim
	{
		for (size_t i = 0; i < numBones; i++)
		{
			btd.boneTransform[i] = H::Math::Transpose(mSkeleton.bones[i]->offsetTransform *  boneMats[i]);
		}
	}
	UpdateTransforms(mSkeleton.root, boneMats);
	return btd;
}

void TAnimator::SwitchAnimation(int animIndex)
{
	if (mTransition.mAnimTransitOn)
	{
		//save progress remained by prev anim
		mTransition.mPrevAnimProgress = mAnimTimer;
		//reset progress for current anim
		mAnimTimer = 0.0f;
		//compute animTransit step for animTransit progress
		mTransition.mAnimTransitStep = 1.0f / (mAnimPlayTime - mTransition.mPrevAnimProgress);
		//reset animTransit progress
		mTransition.mAnimTransitProgress = 0.0f;
		//transition start
		mTransition.mAnimInTransition = true;
	}

	//change index
	mCurrChooseAnim_Index = animIndex;

	//reset pointer
	mTransition.mPrevAnimClip = mTransition.mCurrAnimClip;
	mTransition.mCurrAnimClip = mAnimClips[mCurrChooseAnim_Index].get();
	//reset anim timer
	mAnimPlayTime = mTransition.mCurrAnimClip->duration;

}

void TAnimator::UpdateAnimationTimer(float deltatime)
{
	if (mAnimBlending.mBlendAnim)
	{
		mAnimBlending.UpdateTimer(deltatime);
	}
	else
	{
		if (mPlayAnim)
		{
			mAnimTimer += deltatime * mTransition.mCurrAnimClip->tickPerSecond;

			if (mTransition.mAnimInTransition)
			{
				mTransition.mPrevAnimProgress += deltatime * mTransition.mPrevAnimClip->tickPerSecond;
				mTransition.mAnimTransitProgress += deltatime * mTransition.mPrevAnimClip->tickPerSecond * mTransition.mAnimTransitStep;
			}
		}

		//if transition done
		if (mTransition.mAnimTransitProgress >= 1.0f)
			mTransition.mAnimInTransition = false;

		//reset timer
		if (mAnimTimer > mAnimPlayTime)
		{
			mAnimTimer = 0.0f;
			mPlayAnim = mLoopAnim ? true : false;
		}
	}
	
}

void TAnimator::DebugUI()
{
	ImGui::Begin("animation");
	if (ImGui::CollapsingHeader("Import Animation"))
	{
		ImGui::Text("AnimFile path:"); ImGui::SameLine();
		ImGui::InputText("", in_anim_file_path, file_path_buffer_size);
		//ImGui::Button("load");
 	}
	if (ImGui::CollapsingHeader("Animation clips"))
	{
		//for (const auto& ac :mAnimClips)
		for (size_t i = 0; i < mAnimClips.size(); i++)
		{
			const auto& ac = mAnimClips[i];
			if (ImGui::Button( ('[' + std::to_string(i) + ']' + ' ' + ac->name).c_str() ))
			{
				PlayAnim(i);
				//animChanged |= true;
				//mCurrChooseAnim_Index = i;
			}
		}
	}

	if (ImGui::CollapsingHeader("single Play control"))
	{
		if (ImGui::ArrowButton("prev", 0))
		{
			animChanged |= true;
			mCurrChooseAnim_Index = mCurrChooseAnim_Index - 1 < 0 ? mNumAnimClips - 1 : mCurrChooseAnim_Index - 1;

		}
		ImGui::SameLine();

		if (mAnimClips.size() > 0)
			ImGui::Text("curr anim : %s [%d]", mAnimClips[mCurrChooseAnim_Index]->name.c_str(), mCurrChooseAnim_Index);

		ImGui::SameLine();
		if (ImGui::ArrowButton("next", 1))
		{
			animChanged |= true;
			mCurrChooseAnim_Index = mCurrChooseAnim_Index + 1 > mNumAnimClips - 1 ? 0 : mCurrChooseAnim_Index + 1;

		}

		ImGui::Checkbox("play", &mPlayAnim);
		ImGui::Checkbox("loop", &mLoopAnim);
		ImGui::TextDisabled("[---------------------------]");
		ImGui::Checkbox("transitionOn", &mTransition.mAnimTransitOn);
		ImGui::DragFloat("transit start", &mTransition.mAnimTransitStart, 0.05f, 0.0f, 1.0f);

		ImGui::DragFloat("progress", &mAnimTimer, 0.1f, 0.0f, mAnimPlayTime);
		ImGui::Text("prev progress: %f", mTransition.mPrevAnimProgress);
		ImGui::Text("transition progress: %f", mTransition.mAnimTransitProgress);

	}
	
	if (ImGui::CollapsingHeader("Animation Blending"))
	{
		if (ImGui::Checkbox("blend", &mAnimBlending.mBlendAnim))
		{
			if (mAnimBlending.mBlendAnim)
			{
				(mAnimBlending.mSections[0]).mATransition.mCurrAnimClip = mAnimClips[0].get();
				//(mAnimBlending.mSections[0]).mATransition.mPrevAnimClip = mAnimClips[0].get();
				(mAnimBlending.mSections[0]).SwitchAnimation(0);

				(mAnimBlending.mSections[1]).mATransition.mCurrAnimClip = mAnimClips[0].get();
				//(mAnimBlending.mSections[1]).mATransition.mPrevAnimClip = mAnimClips[0].get(); 
				(mAnimBlending.mSections[1]).SwitchAnimation(0);

				(mAnimBlending.mSections[2]).mATransition.mCurrAnimClip = mAnimClips[0].get();
				//(mAnimBlending.mSections[2]).mATransition.mPrevAnimClip = mAnimClips[3].get();
				(mAnimBlending.mSections[2]).SwitchAnimation(0);

				(mAnimBlending.mSections[3]).mATransition.mCurrAnimClip = mAnimClips[0].get();
				(mAnimBlending.mSections[3]).SwitchAnimation(0);

				(mAnimBlending.mSections[4]).mATransition.mCurrAnimClip = mAnimClips[0].get();
				(mAnimBlending.mSections[4]).SwitchAnimation(0);

 			}
 
 		}
		

		for (size_t i = 0; i < mAnimBlending.mSections.size(); i++)
		{
			auto& sec = mAnimBlending.mSections[i];
			bool& secOn = sec.mPlayAnim;
			bool& lopOn = sec.mLoopAnim;
			bool& transiOn = sec.mATransition.mAnimTransitOn;

			if (ImGui::Button(('[' + std::to_string(i) + ']' + ' ' + sec.mName).c_str()))
			{
				mAnimBlending.mCurrentSection_index = i;
			}
			ImGui::SameLine();
			ImGui::Checkbox(("play##"+std::to_string(i)).c_str(), &secOn);
			ImGui::SameLine();
			ImGui::Checkbox(("loop##"+std::to_string(i)).c_str(), &lopOn);
			ImGui::SameLine();
			ImGui::Checkbox(("transition##" + std::to_string(i)).c_str(), &transiOn);

		}
		ImGui::Text("curr section: %d", mAnimBlending.mCurrentSection_index);
		for (size_t i = 0; i < mAnimClips.size(); i++)
		{
			const auto& ac = mAnimClips[i];
			if (ImGui::Button(('[' + std::to_string(i) + ']' + ' ' + ac->name).c_str()))
			{
				auto& s = mAnimBlending.mSections[mAnimBlending.mCurrentSection_index];
				s.animChanged |= true;
				s.mCurrChooseAnim_Index = i;
			}
		}
 
	}

	ImGui::TextDisabled("[---------------------------]");
	if (ImGui::Button("reset"))
	{
		mCurrChooseAnim_Index = 0;
		mAnimTimer = 0.0f;
	}
	ImGui::End();
}

//void TAnimator::CreateSection()
//{
//	mAnimBlending.mSections.push_back(Bone_Section());
//}

void TAnimator::CreateSection(AnimationClips& animClips, std::string name)
{
	mAnimBlending.mSections.emplace_back(Bone_Section(animClips,name));
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

Bone_Section::Bone_Section(AnimationClips& animClips, std::string name)
	:mAnimClipsRef(animClips),mName(name)
{
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
					mBoneRefs[i]->toParentTransform = animTf;
				}
			}

		}
		btd.boneTransform[currBoneIndex] = H::Math::Transpose(mBoneRefs[i]->offsetTransform *  boneMats[currBoneIndex]);
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

void Animator_Blending::UpdateBoneSection_ToParentTransform(BoneTransformData& btd,std::vector<Matrix4>& boneMats)
{
	for (const auto& bs : mSections )
	{
		bs.Update_ToparentTransform(btd,boneMats);
	}
}

void Animator_Blending::UpdateTimer(float deltatime)
{
	for (auto& sec : mSections)
	{
		sec.UpdateAnimTimer(deltatime);
	}
}

void Animator_Blending::Update(float deltatime)
{
	for (auto& sec : mSections)
	{
		sec.Update(deltatime);
	}

}
