#pragma once
#include <HUGE/Inc/HUGE.h>
using namespace H;

class C_Transform
{
public:
 	C_Transform();
	void Initialize()  ;
	void Terminate()  ;
	void Update(float deltatime)  ;
	void Render()  ;
	void DebugUI()  ;
	void ResetTransform();
public:
	Matrix4 GetTransformMatrix() const;

public:
	Vector3 position;
	Vector3 rotation;
	Quaternion rotationQuaternion;
	Vector3 scale;

private:
 
};
 