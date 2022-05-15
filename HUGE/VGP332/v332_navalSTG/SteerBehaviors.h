#pragma once
#include <HUGE\Inc\HUGE.h>

//[Steering]==================================================
class Seek : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;
	float* force;
	float turn = 0.1f;

	//float force = 3.0f;
private:
};

