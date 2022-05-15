#pragma once
#include <HUGE\Inc\HUGE.h>

class Teacher;
class ExamPaper;
enum class QuestionType : int;

class Student : public H::AI::Agent
{
public:
	Student(H::AI::AIWorld& world, ExamPaper& paper);
	~Student();

	void Init(std::vector<std::unique_ptr<ExamPaper>>* ap,int index);
	void Update(float);
	void Render() const;

 	void Move(float deltatime);
	void FindPath(std::list<size_t>& path);
	void GetCaught();
	void EscapeFromCatch();
	void PretendInnocent();

	void SolveQuestion();
	void LookAround(float deltatime);
	void TurnAround();
	void RecovePatience(int amount);

	void ChangeState(std::string statename);
	void StartMoving();
	void StopMoving();
	void SetObsAvoid(bool active);

	bool CopyFromOthers(float deltatime);


	void DebugUI();
	//
	inline float GetPatience() const { return mPatience; }
	inline const float& GetScanTime() const { return mScanTime; }
	inline  float& GetScanTimer()  { return mScanTimer; }
	inline  ExamPaper& GetExamPaper() { return mPaper; }
	inline  float& GetCopyProgress() { return mCopyCurrentProgress; }
	inline  float& GetCopyProgressFull() { return mCopyMaxProgress; }
	inline  float GetCopySpeed() const { return mCopySpeed; }
	inline  void  SetPaperCopyFrom(int fromIndex) {  mCopyFrom = (*allPapers)[fromIndex].get(); }
	inline void SetCheat(bool ischeat) { mCheat = ischeat; }
	inline bool IsCheating( ) {  return mCheat; }
	inline bool IsActive() { return mActive; }
	const H::AI::MemoryRecords& GetMemoryRecords() const; 

	std::vector<std::unique_ptr<ExamPaper>>* allPapers;



	float lastMemImportance = 0.0f;
	int mIndex;
	bool stop = false;
	bool debugDraw = false;
	Teacher* seesTeacher;
	int questionsCopied = 0;
	int escapedFromCatch = 0;
	int pretendInnocent = 0;
 

	std::string mCurrentState;
	//[texture]
	TextureId mNormalTextureId;
	TextureId mFinishTextureId;
	TextureId mDeadTextureId;

	TextureId mRedTextureId;
	TextureId mGreenTextureId;
	TextureId mBlueTextureId;

	TextureId mCheatRedDotTextureId;

	TextureId* mCurrentTextureId;

	//log
	std::array<std::string,10> mMemLog;
private:

	void UpdateTimer(float);
	//[ai module]
	std::unique_ptr<H::AI::SteeringModule> mSteeringModule;
	std::unique_ptr<H::AI::StateMachine<Student>> mStateMachine;
	std::unique_ptr<H::AI::PerceptionModule> mPerceptionModule;
	float mPerceptionModuleTime = 0.1f;
	float mPerceptionModuleTimer = 0.0f;

	H::AI::ImportanceCalculator mImportCalc;
	//H::AI::MemoryRecords* mMemory;

	//[graph path]  ==========================
	std::list<size_t> path;



	float mRenderOffset = 16.0f;

	//[game]
	bool mActive;

	//[solve question]
	ExamPaper& mPaper;
	ExamPaper* mCopyFrom;

	float mQuestionSolveTime;//time interval for trying solve
	float mQuestionSolveTimer = 0.0f;
	float mScanTime;//peek around for info
	float mScanTimer = 0.0f;

	float mCopyMaxProgress = 10.0f;//copy from other
	float mCopyCurrentProgress = 0.0f;
	float mCopySpeed;


	float mPatience;
	float mSuccessRate;
	float mSuspicious = 0.0f;
	bool  mCheat = false;



};
//[state]
struct MoveToOwnPaperState : public H::AI::State<Student>
{
	static std::string GetName() { return "MoveToOwn"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 2.0f;
	float scanTimer = 0.0f;


};
struct MoveToDestination : public H::AI::State<Student>
{
	static std::string GetName() { return "MoveToDest"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 2.5f;
	float scanTimer = 0.0f;


};
struct FleeState : public H::AI::State<Student>
{
	static std::string GetName() { return "Flee"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

private:
	//
	H::Math::Vector2 destination;
	float scanTime = 5.0f;
	float scanTimer = 0.0f;


};
struct ThinkState : public H::AI::State<Student>
{
	static std::string GetName() { return "Think"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

	float degree_Flee = H::Math::Constants::Pi * 0.3f;
	float dist_Flee = 80.0f;
private:
	//

};
struct ScanState : public H::AI::State<Student>
{
	static std::string GetName() { return "Scan"; }

 	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

	float scanTime = 0.2f;
	//float scanDuration = 1.0f;
	float scanTimer = 0.0f;

	int scanTurnTimes = 0;
private:
	//

};

struct WorkState : public H::AI::State<Student>
{
	static std::string GetName() { return "Work"; }

 	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

private:
	//
	float patience;

};
struct CheatState : public H::AI::State<Student>
{
	static std::string GetName() { return "Cheat"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

	void SetCopyFrom(Student & agent, ExamPaper* cp);

private:
	//
	float vigilantTime = 0.4f;
	float vigilantTimer = 0.0f;
	//
	float fullProgress;
	float currProgress;

};

struct FinishState : public H::AI::State<Student>
{
	static std::string GetName() { return "Finish"; }

	virtual void Enter(Student & agent) override;

	virtual void Update(Student & agent, float deltatime) override;

	virtual void Exit(Student & agent) override;

private:
	//
	float patience;

};

//
class VisualSensor : public H::AI::Sensor
{
public:
	void AddAllPaperNearby(H::AI::Agent& agent);
	void Update(H::AI::Agent& agent, H::AI::MemoryRecords& memory, float deltatime) override;

public:
	std::vector<std::unique_ptr<ExamPaper>>* nearPaper;
};

class ScanSensor : public H::AI::Sensor
{
public:
	void Update(H::AI::Agent& agent, H::AI::MemoryRecords& memory, float deltatime) override;


};