#pragma once
#include <HUGE/Inc/HUGE.h>

class TPlane
{
public:
	void Init();
	void Update(float dt);
	void Render();
	void CheckIntersec(H::Math::Ray ray);
	void DebugUI();

private:
	H::Math::Plane mPlane{ 0.0f,1.0f,0.0f,0.0f };
	H::Math::Vector3 mLastIntersec{ H::Math::Vector3::Zero()};
	float mIntersecDistance{0.0f};
	bool mIntersec{false};
};
 