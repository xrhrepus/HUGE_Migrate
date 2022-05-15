#include "Precompiled.h"
#include "Animation.h"

using namespace H;
using namespace H::Graphics;
#pragma region privates



template<class T>
T H::Graphics::Animation::Interpolate(const T& fromVal, const T& toVal, float t) const
{
	//if (dynamic_cast<H::Math::Vector3>(fromVal))
	{
		return H::Math::Lerp(static_cast<H::Math::Vector3> (fromVal), static_cast<H::Math::Vector3> (toVal), t);
	}
	//else if (dynamic_cast<H::Math::Quaternion>(fromVal))
	//{
	//	return H::Math::Slerp(static_cast<H::Math::Quaternion> (fromVal), static_cast<H::Math::Quaternion> (toVal), t);
	//}
	return T();
}

H::Math::Quaternion H::Graphics::Animation::Interpolate(const H::Math::Quaternion & fromVal, const H::Math::Quaternion & toVal, float t) const
{
	return H::Math::Slerp(static_cast<H::Math::Quaternion> (fromVal), static_cast<H::Math::Quaternion> (toVal), t);
}

template<class T>
size_t H::Graphics::Animation::FindKeyframeIndex(const Keyframes<T>& keyframes, float time) const
{
	float prevTime = 0;
	size_t prevIndex = 0;
	for (size_t i = 0; i < keyframes.size(); ++i)
	{
		if ( time < keyframes[i].time && time >= prevTime)
		{
			return prevIndex;
		}
		else
		{
			prevTime = keyframes[i].time;
			prevIndex = i;
		}
	}

	return prevIndex;
}


template<class T>
T H::Graphics::Animation::GetValue(const Keyframes<T>& keyframes, float time) const
{
	if (keyframes.empty())
	{
		return T();
	}

	const Keyframe<T>& firstkey = keyframes.front();
	const Keyframe<T>& lastkey = keyframes.back();

	if (time <= firstkey.time)
	{
		return firstkey.key;
	}
	else if (time >= lastkey.time)
	{
		return lastkey.key;
	}

	const size_t index = FindKeyframeIndex(keyframes, time);
	const Keyframe<T>& fromKey = keyframes[index];
	const Keyframe<T>& toKey = keyframes[index + 1];
	const float t = (time - fromKey.time) / (toKey.time - fromKey.time);
	return Interpolate(fromKey.key, toKey.key, t);
}

template<class T>
bool H::Graphics::Animation::SortAnimation_Comp(Keyframe<T> k1, Keyframe<T> k2)
{
	return k1.time < k2.time;
}

#pragma endregion
//privates^^^^^^^


Math::Matrix4 H::Graphics::Animation::GetTransform(float time) const
{
	return   GetScale(time) * GetRotation(time) * GetPosition(time)  ;
}

Math::Vector3 H::Graphics::Animation::GetPositionValue(float time) const
{
	return mPositions.empty() ? Math::Vector3::Zero() : GetValue(mPositions, time);

}

Math::Quaternion H::Graphics::Animation::GetRotationValue(float time) const
{
	return mRotations.empty() ? Math::Quaternion::Zero() : GetValue(mRotations, time);

}

Math::Vector3 H::Graphics::Animation::GetScaleValue(float time) const
{
	
	return mScales.empty() ? Math::Vector3{1.0f} : GetValue(mScales, time);

}

Math::Matrix4 H::Graphics::Animation::GetPosition(float time) const
{
	if (mPositions.empty())
	{
		return { H::Math::Matrix4::Identity() };
	}
	const H::Math::Vector3 transv = GetValue(mPositions, time);
	H::Math::Matrix4 translation;
	translation.Translation(transv);

	return (translation);
}

Math::Matrix4 H::Graphics::Animation::GetRotation(float time) const
{
	if (mRotations.empty())
	{
		return { H::Math::Matrix4::Identity() };
	}

	const H::Math::Quaternion rotq = GetValue(mRotations, time);
	H::Math::Matrix4 rotation =	H::Math::MatrixRotationQuaternion(rotq);

	return (rotation);
}

Math::Matrix4 H::Graphics::Animation::GetScale(float time) const
{
	if (mScales.empty())
		{
			return { H::Math::Matrix4::Identity() };
		}

		const H::Math::Vector3 scalev = GetValue(mScales, time);
		H::Math::Matrix4 scaling;
		scaling.Scaling(scalev);

		return (scaling);
}

void H::Graphics::Animation::AddPosition(const Math::Vector3& pos, float time)
{
	mPositions.push_back({ pos,time });
}

void H::Graphics::Animation::AddRotation(const Math::Quaternion& rot, float time)
{
	mRotations.push_back({ rot,time });

}

void H::Graphics::Animation::AddScale(const Math::Vector3& scale, float time)
{
	mScales.push_back({ scale,time });
}

void H::Graphics::Animation::SortAnimation()
{
	//std::sort(mPositions.begin(), mPositions.end(), SortAnimation_Comp<H::Math::Vector3>);
	std::sort(mPositions.begin(), mPositions.end(),
		[](Keyframe<Math::Vector3> k1, Keyframe < Math::Vector3> k2) { return k1.time < k2.time;});
	std::sort(mRotations.begin(), mRotations.end(),
		[](Keyframe<Math::Quaternion> k1, Keyframe < Math::Quaternion> k2) { return k1.time < k2.time; });
	std::sort(mScales.begin(), mScales.end(),
		[](Keyframe<Math::Vector3> k1, Keyframe < Math::Vector3> k2) { return k1.time < k2.time; });

}

 