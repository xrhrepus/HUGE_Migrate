#pragma once
#include <HUGE\Inc\HUGE.h>

class Ship : public H::AI::Agent
{
public:
	Ship(H::AI::AIWorld& world)
		:Agent(world)
	{}
	~Ship() = default;

	virtual void Init()  = 0;

	virtual void Update(float deltatime)
	{
		Move(deltatime);
		Aim();
	}

	virtual void Render() = 0;

	virtual void Move(float deltatime) = 0;
	virtual void Aim() = 0;

	virtual void Steer() = 0;
	virtual void Fire() = 0;
	virtual void Die() = 0;
	virtual void InflictDamage() = 0;

	virtual void PassDataPtr(const std::vector<std::unique_ptr<float>>& datas) = 0;

protected:
	std::unique_ptr<H::AI::SteeringModule> mSteeringModule;

	H::Math::Vector2 mTarget;
	float mSteerForce;


	float mAcceleration = 10.0f;


};

 