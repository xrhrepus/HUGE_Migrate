#pragma once
#include <HUGE/Inc/HUGE.h>
struct TDirectionalLight
{
public:
	void Initialize();
	void Update(float dt);
	void DebugDraw() const;
	void DebugUI();
public:
	
	H::Graphics::DirectionalLight mLight;
	H::Graphics::Camera mLightCam;
	H::Math::Vector3 mPosition{0.0f,50.0f,0.0f};
	H::Math::Vector3 mDirection;
 
};

 