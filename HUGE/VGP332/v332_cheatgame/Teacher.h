#pragma once
#include <HUGE\Inc\HUGE.h>
class Student;

class Teacher : public H::AI::Agent
{
public:
	Teacher(H::AI::AIWorld& world);
	//~Teacher ();

	void Init(std::vector<std::unique_ptr<Student>>* allstudent);
	void Update(float deltatime);
	void Render();

	void Move(float deltatime);
	void LookAround(float deltatime);
	void TurnAround();
	void Patrol();
	bool Catch();

	void ChangeState(std::string statename);
 	void StartMoving();
	void StopMoving();
	void SetObsAvoid(bool active);

	const H::AI::MemoryRecords& GetMemoryRecords() const;
 
	bool SetChasingTarget(int index);
	
	
	void StopChasing();

	void DebugUI();


	void Scan();
	void Suspect();

	std::string mCurrentState;
	Student* currChasing = nullptr;
	int killCount = 0;

	std::vector<size_t> studentInsight;
	bool stop = false;
private:
	void UpdateTimer(float);
	//[game]
	const int max_partol_point = 6;
	std::vector<H::Math::Vector2 > mPartolPts;
	std::vector<std::unique_ptr<Student>>* allStudents;



	int currPatrolPt = 0;
	//[ai module]
	std::unique_ptr<H::AI::SteeringModule> mSteeringModule;
	std::unique_ptr<H::AI::StateMachine<Teacher>> mStateMachine;
	std::unique_ptr<H::AI::PerceptionModule> mPerceptionModule;

	float mPatrolTime;
	float mPatrolTimer = 0.0f;

	float mScanTime;
	float mScanTimer;
	TextureId mTextureId;
	TextureId mHuntingMarkTextureId;
	TextureId mPistoTextureId;




};
#pragma region States


struct TeacherScanState : public H::AI::State<Teacher>
{
	static std::string GetName() { return "Scan"; }

	virtual void Enter(Teacher & agent) override;

	virtual void Update(Teacher & agent, float deltatime) override;

	virtual void Exit(Teacher & agent) override;

	float scanTime = 0.5f;
	//float scanDuration = 1.0f;
	float scanTimer = 0.0f;

	int scanTwice = 0;
private:
	//

};
struct TeacherThinkState : public H::AI::State<Teacher>
{
	static std::string GetName() { return "Think"; }

	virtual void Enter(Teacher & agent) override;

	virtual void Update(Teacher & agent, float deltatime) override;

	virtual void Exit(Teacher & agent) override;
	float time = 0.8f;
 	float timer = 0.0f;


 private:
	//

};

struct TeacherMoveToDestination : public H::AI::State<Teacher>
{
	static std::string GetName() { return "MoveToDest"; }

	virtual void Enter(Teacher & agent) override;

	virtual void Update(Teacher & agent, float deltatime) override;

	virtual void Exit(Teacher & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 2.0f;
	float scanTimer = 0.0f;


};
struct TeacherPatrol : public H::AI::State<Teacher>
{
	static std::string GetName() { return "Patrol"; }

	virtual void Enter(Teacher & agent) override;

	virtual void Update(Teacher & agent, float deltatime) override;

	virtual void Exit(Teacher & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 4.0f;
	float scanTimer = 0.0f;


};

struct TeacherHunting : public H::AI::State<Teacher>
{
	static std::string GetName() { return "Hunt"; }

	virtual void Enter(Teacher & agent) override;

	virtual void Update(Teacher & agent, float deltatime) override;

	virtual void Exit(Teacher & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 8.0f;
	float scanTimer = 0.0f;

	float catchTime = 0.5f;
	float catchTimer = 0.0f;

};

#pragma endregion

#pragma region Sensor

class TeacherVisualSensor : public H::AI::Sensor
{
public:
	void Update(H::AI::Agent& agent, H::AI::MemoryRecords& memory, float deltatime) override;

	//float viewRange = 0.0f;
	//float viewAngle = 0.0f;

};


#pragma endregion
