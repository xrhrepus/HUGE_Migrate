#ifndef HUGE_GRAPHICS_ANIMATION_H
#define HUGE_GRAPHICS_ANIMATION_H
#include "Keyframe.h"

namespace H::Graphics
{
 
	class Animation
	{
		template<class T>
		using Keyframes = std::vector < Keyframe<T>>;

		using PositionKeys = Keyframes<Math::Vector3>;
		using RotationKeys = Keyframes<Math::Quaternion>;
		using ScaleKeys = Keyframes<Math::Vector3>;

	public:

		Math::Matrix4 GetTransform(float time) const;

		Math::Vector3 GetPositionValue(float time) const;
		Math::Quaternion GetRotationValue(float time) const;
		Math::Vector3 GetScaleValue(float time) const;

		Math::Matrix4 GetPosition(float time) const;
		Math::Matrix4 GetRotation(float time) const;
		Math::Matrix4 GetScale(float time) const;


		void AddPosition(const Math::Vector3& pos, float time);
		void AddRotation(const Math::Quaternion& rot, float time);
		void AddScale(const Math::Vector3& scale, float time);

		void SortAnimation();

	//private:

		//friend class AnimationBuilder;
		PositionKeys mPositions;
		RotationKeys mRotations;
		ScaleKeys  mScales;

	private:
		template<class T>
		bool SortAnimation_Comp(Keyframe<T> k1, Keyframe<T> k2);


		template<class T>
		T GetValue(const Keyframes<T>& keyframes, float time) const;

		template<class T>
		size_t FindKeyframeIndex(const Keyframes<T>& keyframes, float time) const;

		template<class T>
		T Interpolate(const T& fromVal, const T& toVal, float t) const;

		H::Math::Quaternion Interpolate(const H::Math::Quaternion& fromVal, const H::Math::Quaternion& toVal, float t)const;
	};

	class BoneAnimation
	{
	public:
		std::string boneName;
		Animation animation;
	};

	using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;

}//H::Graphics


#endif // !HUGE_GRAPHICS_ANIMATION_H
