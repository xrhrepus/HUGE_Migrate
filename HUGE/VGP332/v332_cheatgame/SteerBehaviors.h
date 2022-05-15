#pragma once
#include <HUGE\Inc\HUGE.h>

//[Steering]==================================================
 
class Seek : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;
	float force = 1.0f;

	//float force = 3.0f;
private:
};
class Arrive : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

private:
	float arriveDistance = 50.0f;
};
class Wander : public Seek
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

	H::Math::Vector2 localWT;
	float* force;
private:
	float wRad = 50.0f;
	float wDist = 50.0f;
	float wJitter = 10.0f;

};
class ObsAvoid : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

	std::vector<Circle>* cPtrs;
	//Circle* c;
	//float* half_width;
	float avoidDist = 40.0f;
	//float* d1;
	//float* d2;
	//float* d3;
	//float* d4;

private:
};
class Separation : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

	//float force = 1.0f;
	float* force;
private:
};
class Alignment : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

	//float force = 1.0f;
	float* force;
private:

};
class Cohesion : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

	//float force = 1.0f;
	float* force;

private:
};


//[Steering]==================================================


