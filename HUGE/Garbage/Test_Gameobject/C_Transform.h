#pragma once
#include <HUGE/Inc/HUGE.h>
using namespace H;

class C_Transform : public H::Component
{
public:
	META_CLASS_DECLARE
	C_Transform();
	void Initialize() override;
	void Terminate() override;
	void Update(float deltatime) override;
	void Render() override;
	void DebugUI() override;

public:
	Matrix4 GetTransformMatrix() const;

public:
	Vector3 position;
	Vector3 rotation;
	Quaternion rotationQuaternion;
	Vector3 scale;

private:
 
};
 