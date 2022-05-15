#pragma once
#include <HUGE\Inc\HUGE.h>

class Sheep : public H::AI::Agent
{
public:
	Sheep(H::AI::AIWorld& world);
	~Sheep() = default;
	void Init(std::vector<Circle>* cptrs);
	void PassDataPtr();
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
	std::unique_ptr<H::AI::PerceptionModule> mPerceptionModule;


	float data1;
	float data2;
	float data3;
	float data4;

private:
	H::Graphics::TextureId mTextureId;

 };
#pragma region steer



//[Steering]==================================================
class Seek : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

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
private:
	float wRad = 50.0f;
	float wDist = 50.0f;

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

 private:
	 float force = 150.0f;
};
class Alignment : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

private:
	float force = 150.0f;

};
class Cohesion : public H::AI::SteeringBehavior
{
public:
	H::Math::Vector2 Calculate(H::AI::Agent& agent) override;

private:
	float force = 150.0f;
};


//[Steering]==================================================
#pragma endregion

#pragma region Sensor

class VisualSensor : public H::AI:: Sensor
{
public:
	void Update(H::AI::Agent& agent, H::AI::MemoryRecords& memory, float deltatime) override;

	float viewRange = 0.0f;
	float viewAngle = 0.0f;

};


#pragma endregion
