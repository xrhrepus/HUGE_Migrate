#ifndef HUGE_GRAPHICS_SHADEREFFECTBUFFERTYPE_H
#define HUGE_GRAPHICS_SHADEREFFECTBUFFERTYPE_H
#include "Common.h"
#include "Light.h"
#include "Material.h"
#include "ConstantBuffer.h"

namespace H::Graphics
{
	struct TransformData
	{
		H::Math::Matrix4 world;
		H::Math::Matrix4 wvp;// combined matrix
		H::Math::Vector3 viewPosition;
		float padding;
	};
	struct OptionsData
	{
		float displacementWeight;
		float useNormal;
		float padding[2];
	};

	struct BoneTransformData
	{
		H::Math::Matrix4 boneTransform[128];
	};

	using TransformBuffer = H::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = H::Graphics::TypedConstantBuffer<H::Graphics::DirectionalLight>;
	using MaterialBuffer = H::Graphics::TypedConstantBuffer<H::Graphics::Material>;
	using OptionsBuffer = H::Graphics::TypedConstantBuffer<OptionsData>;
	using  BoneTransformBuffer = H::Graphics::TypedConstantBuffer<BoneTransformData>;

}

#endif // !HUGE_GRAPHICS_SHADEREFFECTBUFFERTYPE_H

