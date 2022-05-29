#pragma once
#include <HUGE/Inc/HUGE.h>

struct T_Transform
{
	Vector3 pos;
	Vector3 rot;
	Vector3 scale{ 1.0f,1.0f,1.0f };
	Quaternion rotationQuaternion{ 0.0f,0.0f,0.0f,1.0f };
	Matrix4 computeTransform() const;
	void DebugUI();
};

struct T_Light
{
	DirectionalLight light;
	void DebugUI();
};
