#ifndef HUGE_GRAPHICS_ANIMATIONBUILDER_H
#define HUGE_GRAPHICS_ANIMATIONBUILDER_H

namespace H::Graphics
{
	class AnimationBuilder

	{
	public:
 
		AnimationBuilder& AddPositionKey(H::Math::Vector3 pos, float time);
		AnimationBuilder& AddRotationKey(H::Math::Quaternion rot, float time);
		AnimationBuilder& AddScaleKey(H::Math::Vector3 scale, float time);
		AnimationBuilder& AddKey(H::Math::Vector3 pos, H::Math::Quaternion rot, H::Math::Vector3 scale, float time);

		Animation Build();
		void Reset();
	private:
		 Animation mAnimation;
	};

}


 


#endif // !HUGE_GRAPHICS_ANIMATIONBUILDER_H


