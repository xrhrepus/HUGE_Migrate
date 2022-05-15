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

using TransformBuffer = TypedConstantBuffer<TransformData>;
using LightBuffer = TypedConstantBuffer<DirectionalLight>;
using MaterialBuffer = TypedConstantBuffer<Material>;
using OptionsBuffer = TypedConstantBuffer<OptionsData>;
