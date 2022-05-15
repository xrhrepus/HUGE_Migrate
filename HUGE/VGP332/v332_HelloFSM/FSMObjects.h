#pragma once

#include <HUGE\Inc\HUGE.h>
class Bush
{
public:
	Bush() = default;
	//Bush(const Bush& other);
	Bush(H::Math::Vector2 initpos);

	~Bush() = default;
	void Init();
	void Update(float deltatime);
	void Render();
	void ProduceFruit(float deltatime);
	int Harvest(int num );

	void SetPosition(H::Math::Vector2 pos);
	void SetTextureId(H::Graphics::TextureId tId);

	int GetFruitCapacity();
	int GetCurrentFruitNum();
	int GetProduceSpeed();
	int GetProduceTimer();
	//float GetCurrentWorkingProgress();
	//int SetPosition();
	H::Math::Vector2 GetPosition();
private:
	//std::unique_ptr<H::AI::StateMachine<Bush>> mStateMachine;
	//
	int mFruitCapacity = 5;
	int mCurrentFruitNum  = 0;
	int mProduceSpeed = 1;
	//
	float mProduceTimer = 0;
	float mProduceTime = 10;
	//
	H::Math::Vector2 mPosition;
	H::Graphics::TextureId mTexture;

};



class Sheep
{
public:
	enum Priority
	{
		L_0 = 0,
		L_1 = 1,
		L_2 = 2,
		L_3  = 3,
		L_4 = 4,
		L_5 = 5,
	};
 
	Sheep();
	~Sheep();

 	void Init();
	void ChangeState(std::string statename);
	void Update(float deltatime);
	void Render();


	void Move(float deltatime);

	void FindClosestBush();
	void ComputePriority();
	void ResetPriority();

	bool ReachedDestination();

	void SetTextureId(H::Graphics::TextureId tId);
	void SetMoveSpeed(float spd);
	float& GetMoveSpeed();

	Bush* GetBushWorkingOn();
	//int GetTargetBushIndex();
	void AddTargetBush(Bush* bush);
	void AddFruitOwned(int n);
	void StoreAllFruits( );

	H::Math::Vector2& GetDestination();
	H::Math::Vector2& GetPosition();
	H::Math::Vector2 GetHomePosition();
	int GetFruitAtHome();
	int GetFruitOwned();
	std::string GetCurrentState();
	std::vector<int>& GetBushPriority();


 
private:
	std::unique_ptr<H::AI::StateMachine<Sheep>> mStateMachine;
	std::string mCurrentState;

	H::Graphics::TextureId mTexture;
	//
	H::Math::Vector2 mHomePosition;

	H::Math::Vector2 mPosition;
	H::Math::Vector2 mDestination;
	float mMoveSpeed = 300.0f;
	int mFruitOwned = 0;
	int mFruitAtHome = 0;
	//float mWorkProgress;
	//
	//int mTargetBush = -1;
	//std::unique_ptr<Bush> mTargetBush;
	Bush* mTargetBush;

	std::vector<Bush*> mBushes;
	//std::vector<Priority> mBushPriority;
	std::vector<int> mBushPriority;


	//float mWorkProgress_2 = 0.0f;

};
struct Sheep_FindState : public H::AI::State<Sheep>
{
	static std::string GetName() { return "Find"; }

	// Inherited via State
	virtual void Enter(Sheep & agent) override;

	virtual void Update(Sheep & agent, float deltatime) override;

	virtual void Exit(Sheep & agent) override;

private:
	//
	H::Math::Vector2 destination;

};

struct Sheep_MoveState : public H::AI::State<Sheep>
{
	static std::string GetName() { return "Move"; }

	// Inherited via State
	virtual void Enter(Sheep & agent) override;

	virtual void Update(Sheep & agent, float deltatime) override;

	virtual void Exit(Sheep & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float refindTimer = 0.0f;
	float refindTime = 1.0f;


};


struct Sheep_WorkState : public H::AI::State<Sheep>
{
	static std::string GetName() { return "Work"; }

	// Inherited via State
	virtual void Enter(Sheep & agent) override;

	virtual void Update(Sheep & agent, float deltatime) override;

	virtual void Exit(Sheep & agent) override;

private:
	float workProgress;
	//std::unique_ptr<Bush> mBushWorkingOn;
	Bush* mBushWorkingOn;

};

struct Sheep_BackState : public H::AI::State<Sheep>
{
	static std::string GetName() { return "Back"; }

	// Inherited via State
	virtual void Enter(Sheep & agent) override;

	virtual void Update(Sheep & agent, float deltatime) override;

	virtual void Exit(Sheep & agent) override;

private:
	H::Math::Vector2 destination;
};

