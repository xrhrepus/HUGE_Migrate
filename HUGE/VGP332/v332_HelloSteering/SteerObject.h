#pragma once
#include <HUGE\Inc\HUGE.h>

class Sheep : public H::AI::Agent
{
public:
	Sheep(H::AI::AIWorld& world);
	~Sheep() = default;
	void Init(std::vector<Circle>* cptrs);
	void PassDataPtr(float* a, float* b, float* c, float* d);
	void Update(float deltatime);
	void Render();

	//void SetTargetSheep(Sheep* sheep);
	void SetDestination(H::Math::Vector2 dest);
	void SetTextureId(H::Graphics::TextureId tid);
	void Move(float deltatime);
	float half_width = 32;
	float maxAvoidDist = 100.0f;
	float avoidDist = 50.0f;


	std::unique_ptr<H::AI::SteeringModule> mSteeringModule;

	float data1;
	float data2;
	float data3;
	float data4;

private:
	H::Graphics::TextureId mTextureId;
	float mRenderOffset = 16.0f;

 };
//[Steering]==================================================
class Seek : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;
	float* force;

	//float force = 3.0f;
private:
};
class Arrive : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

private:
	float arriveDistance = 100.0f;
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
	float* half_width;
	float* avoidDist;
	float* d1;
	float* d2;
	float* d3;
	float* d4;

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

