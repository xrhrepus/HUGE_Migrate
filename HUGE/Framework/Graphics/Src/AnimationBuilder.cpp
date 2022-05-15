#include "Precompiled.h"
#include "Animation.h"
#include "AnimationBuilder.h"

using namespace H::Graphics;


AnimationBuilder & H::Graphics::AnimationBuilder::AddPositionKey(H::Math::Vector3 pos, float time)
{
	mAnimation.AddPosition(pos, time);
	return *this;
 }

AnimationBuilder & H::Graphics::AnimationBuilder::AddRotationKey(H::Math::Quaternion rot, float time)
{
	mAnimation.AddRotation(rot, time);
	return *this;
}

AnimationBuilder & H::Graphics::AnimationBuilder::AddScaleKey(H::Math::Vector3 scale, float time)
{
	mAnimation.AddScale(scale, time);
	return *this;
 }

AnimationBuilder & H::Graphics::AnimationBuilder::AddKey(H::Math::Vector3 pos, H::Math::Quaternion rot, H::Math::Vector3 scale, float time)
{
	mAnimation.AddPosition(pos, time);
	mAnimation.AddRotation(rot, time);
	mAnimation.AddScale(scale, time);
	return *this;
}

Animation H::Graphics::AnimationBuilder::Build()
{
	mAnimation.SortAnimation();
	return mAnimation;
}

void H::Graphics::AnimationBuilder::Reset()
{
	mAnimation = Animation();
}
