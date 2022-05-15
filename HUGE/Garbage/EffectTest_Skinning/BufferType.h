#pragma once
#include <HUGE/Inc/HUGE.h>
struct TransformData
{
	Matrix4 world;
	Matrix4 wvp;// combined matrix
	Vector3 viewPosition;
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
	Matrix4 boneTransform[128];
};

using TransformBuffer = TypedConstantBuffer<H::Graphics::TransformData>;
using LightBuffer = TypedConstantBuffer<H::Graphics::DirectionalLight>;
using MaterialBuffer = TypedConstantBuffer<H::Graphics::Material>;
using OptionsBuffer = TypedConstantBuffer<H::Graphics::OptionsData>;
using  BoneTransformBuffer = TypedConstantBuffer<H::Graphics::BoneTransformData>;
