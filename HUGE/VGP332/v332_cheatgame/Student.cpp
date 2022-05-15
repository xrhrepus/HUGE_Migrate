#include "Student.h"
#include "ExamPaper.h"
#include "SteerBehaviors.h"
#include "Teacher.h"
#pragma region Function


using namespace H::AI;
Student::Student(H::AI::AIWorld& world, ExamPaper& paper)
	:Agent(world),mPaper(paper)
{
	viewAngle = H::Math::Constants::Pi * 0.2f;
	viewRadius = 100.0f;
	cosViewAngle = cosf(viewAngle);
	heading = { 1.0f,0.0f };
	mTag = 0;

	position = { 600.0f,600.0f };
	destination = { 0.0f,0.0f };
	maxSpeed = 200.0f;
	mActive = true;
}

Student::~Student()
{
}

void Student::Init(std::vector<std::unique_ptr<ExamPaper>>* ap, int index)
{
 
	allPapers = ap;
	mIndex = index;

 	mNormalTextureId = TextureManager::Get()->LoadTexture(L"sheepOL_32.png");
	mFinishTextureId = TextureManager::Get()->LoadTexture(L"kingSheep32.png");
	mRedTextureId = TextureManager::Get()->LoadTexture(L"alphaSheep32_Red.png");
	mGreenTextureId = TextureManager::Get()->LoadTexture(L"alphaSheep32_Green.png");
	mBlueTextureId = TextureManager::Get()->LoadTexture(L"alphaSheep32_Blue.png");
	mCurrentTextureId = &mNormalTextureId;
	mCheatRedDotTextureId = TextureManager::Get()->LoadTexture(L"RedDot32.png");
	mDeadTextureId = TextureManager::Get()->LoadTexture(L"tomb32.png");
 	//[props]
	//mPatience = rand() % 150 + 20;
	mPatience = rand() % 15 + 15;

	mScanTime = rand() % 15 + 5;
	mScanTimer = mScanTime;
	
	mQuestionSolveTime = 3.0f * (static_cast<float>(rand() % 30 + 10));
	mQuestionSolveTimer = mQuestionSolveTime;

	mCopyMaxProgress = 10.0f;
	mCopyCurrentProgress = 0.0f;
	mCopySpeed = rand() % 5 + 1;

	//[SteeringModule]

	mSteeringModule = std::make_unique<H::AI::SteeringModule>(*this);
	mSteeringModule->AddBehavior<Seek>("Seek")->SetActive(true);
	mSteeringModule->AddBehavior<Arrive>("Arrive")->SetActive(true);
	mSteeringModule->AddBehavior<ObsAvoid>("ObsAvoid")->SetActive(true);
	
	//[StateMachine]
	mStateMachine = std::make_unique<H::AI::StateMachine<Student>>(*this);
	mStateMachine->AddState<MoveToOwnPaperState>();
	mStateMachine->AddState<MoveToDestination>();
	mStateMachine->AddState<FleeState>();
	mStateMachine->AddState<WorkState>();
	mStateMachine->AddState<ScanState>();
	mStateMachine->AddState<ThinkState>();
	mStateMachine->AddState<CheatState>();
	mStateMachine->AddState<FinishState>();

	mStateMachine->ChangeState("MoveToOwn");
	//
	mImportCalc =  [this](H::AI::MemoryRecord& mem)
	{
		auto it = mem.properties.find("Question");
		if (it != mem.properties.end())
		{
			mem.importance += static_cast<float> (std::get<int>(it->second)) * 5.0f;
		}
		it = mem.properties.find("QuestionLocation");
		if (it != mem.properties.end())
		{
			float dist = H::Math::Distance(position, std::get<H::Math::Vector2>(it->second));
			mem.importance -= (dist / 5.0f);
		}

 
		it = mem.properties.find("TeacherLocation");
		Vector2 tloc;
		if (it != mem.properties.end())
		{
			tloc = std::get<H::Math::Vector2>(it->second);
			float dist = H::Math::Distance(position, tloc);
			mem.importance += 800.0f - (dist);
			//mem.importance += 0.0f;
		}
		it = mem.properties.find("TeacherSight");

		if (it != mem.properties.end())
		{
			Vector2 facing = std::get<H::Math::Vector2>(it->second);
			float theta = H::Math::Dot(H::Math::Normalize(position - tloc), facing); // 1 ~ - ..

			mem.importance += 800.0f * (theta - 0.7f);
		}

 

	};
	mPerceptionModule = std::make_unique<H::AI::PerceptionModule>(*this,mImportCalc);
	
	mPerceptionModule->AddSensor<VisualSensor>("Vision");
 
}

void Student::Update(float deltatime)
{
	if (!stop)
	{
		if (mActive == false)
		{
			SetCheat(false);
			return;
		}
		UpdateTimer(deltatime);
		//SolveQuestion();
		//Move(deltatime);

		//destination = mPaper.mPosition;
		mStateMachine->UpDate(deltatime);
		if (position.x < 200.0f)
		{
			position.x = 200.0f;
		}
		else if (position.x > 1000.0f)
		{
			position.x = 1000.0f;
		}
		if (position.y < 240.0f)
		{
			position.y = 240.0f;
		}
		else if (position.y > 700.0f)
		{
			position.y = 700.0f;
		}

	}
	else
	{
		auto isys = H::Input::InputSystem::Get();
		if (isys->IsMouseDown(H::Input::MouseButton::LBUTTON))
		{
			destination.x = isys->GetMouseScreenX();
			destination.y = isys->GetMouseScreenY();
			Move(deltatime);
		}
		if (isys->IsMousePressed(H::Input::MouseButton::RBUTTON))
		{
			LookAround(deltatime);
		}
		UpdateTimer(deltatime);

	}


}

void Student::Render() const 
{
	if (mActive == false)
	{
		H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mDeadTextureId)), { position.x - mRenderOffset, position.y - mRenderOffset });
	}
	else
	{
		if (debugDraw)
		{
			float cosviewhalf = cosf(viewAngle/**0.5f*/);
			float sinviewhalf = sinf(viewAngle/**0.5f*/);
			Vector2 vf1{ heading.x * cosviewhalf - heading.y * sinviewhalf, heading.x * sinviewhalf + heading.y * cosviewhalf };
			SimpleDraw::AddScreenLine(position, position + vf1 * viewRadius, Colors::Cyan);
			float n_cosviewhalf = cosf(-viewAngle /** 0.5f*/);
			float n_sinviewhalf = sinf(-viewAngle /** 0.5f*/);
			Vector2 vf2{ heading.x * n_cosviewhalf - heading.y * n_sinviewhalf, heading.x * n_sinviewhalf + heading.y * n_cosviewhalf };
			SimpleDraw::AddScreenLine(position, position + vf2 * viewRadius, Colors::Cyan);

			//SimpleDraw::AddScreenLine(position + vf1 * viewRadius, position + vf2 * viewRadius, Colors::Cyan);
			Vector2 mid = position + heading * viewRadius;
			SimpleDraw::AddScreenLine(position + vf1 * viewRadius, mid, Colors::Cyan);
			SimpleDraw::AddScreenLine(mid, position + vf2 * viewRadius, Colors::Cyan);


			//SimpleDraw::AddScreenLine(position, position + heading * viewRadius, Colors::Magenta);

			if (seesTeacher)
			{
				SimpleDraw::AddScreenLine(position, seesTeacher->position, Colors::Cyan);
				SimpleDraw::AddScreenRect({ seesTeacher->position, 40.0f }, Colors::Cyan);
			}
			SimpleDraw::AddScreenRect({ position, 30.0f }, Colors::MediumBlue);
			SimpleDraw::AddScreenLine(position, destination, Colors::MediumBlue);
			SimpleDraw::AddScreenRect({ destination, 40.0f }, Colors::MediumBlue);

		}

		H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(*mCurrentTextureId)), { position.x - mRenderOffset, position.y - mRenderOffset });
		if (mCheat)
		{
			H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mCheatRedDotTextureId)), { position.x - mRenderOffset, position.y - mRenderOffset - 20.0f });

		}

	}

}


void Student::Move(float deltatime)
{
	auto force = mSteeringModule->Calculate();
	auto a = force / mass;
	velocity += a * deltatime;

	//Vector2 vunit = H::Math::Normalize(velocity);
	//float vmag = H::Math::Magnitude(velocity);

	//position += (vunit * (vmag> maxSpeed? maxSpeed:vmag) ) * deltatime;
	position += velocity * deltatime;

	if (H::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = H::Math::Normalize(velocity);
	}

}
void Student::FindPath(std::list<size_t>& _path)
{
	path = _path;
}
void Student::LookAround(float deltatime)
{
//	Agents nearby = world.GetNeighborhood({position,viewRadius});
	if (mPerceptionModuleTimer >= mPerceptionModuleTime)
	{
		mPerceptionModule->Update(deltatime);
		mPerceptionModuleTimer = 0.0f;
	}
}
void Student::TurnAround()
{
	const static float rad60 = H::Math::Constants::Pi / 3.0f;
	const static float cos60 = cosf(rad60);
	const static float sin60 = sinf(rad60);
	Vector2 des1{ heading.x * cos60 - heading.y * sin60, heading.x * sin60 + heading.y * cos60 };

	destination = des1 * 20.0f + position;
 }
void Student::RecovePatience(int amount)
{
	mPatience += (rand() % amount + (amount - 10));
}

void Student::SolveQuestion()
{
	if (mQuestionSolveTimer >= mQuestionSolveTime && !mPaper.IsCompeleted())
	{
		if (mPaper.IsCompeleted())
		{
			return;
		}
		//bool succeed = (rand() % 10) % 2 == 0 ;
		bool succeed = (rand() % 6)  == 3;

		
		if (succeed)
		{
			int quesToSolve = rand() % ExamPaper::totalQuestion;
			if (mPaper.mQuestions[quesToSolve] != true)
			{
				quesToSolve = rand() % ExamPaper::totalQuestion;
				mPaper.Solve(quesToSolve);
			}
		}
		else
		{
			mPatience -= 2;
		}

		mQuestionSolveTimer = 0.0f;
	}

}

void Student::UpdateTimer(float deltatime)
{
	if ((mQuestionSolveTimer < mQuestionSolveTime)/* && (H::Math::Distance(position, mPaper.mPosition) < 1.0f)*/);
	{
		if ((H::Math::Distance(position, mPaper.mPosition) < 1.0f))
		{
			mQuestionSolveTimer++;
		}
	}

	if (mScanTimer < mScanTime)
	{
		mScanTimer += deltatime;
	}
	if (mPatience >= 0.0f)
	{
		mPatience -= deltatime;
	}

	//========
	if (mPerceptionModuleTimer < mPerceptionModuleTime)
	{
		mPerceptionModuleTimer += deltatime;
	}
 
}
void Student::GetCaught()
{
	mActive = false;
	radius = 0.0f;
}

void Student::EscapeFromCatch()
{
	escapedFromCatch++;
}

void Student::PretendInnocent()
{
	pretendInnocent++;
}

void Student::ChangeState(std::string statename)
{
	mStateMachine->ChangeState(statename);
	mCurrentState = std::move(statename);
}
void Student::StartMoving()
{
	mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(true);
	mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(true);
}

void Student::StopMoving()
{
	mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(false);
	mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(false);
}
void Student::SetObsAvoid(bool active)
{
	mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid")->SetActive(active);
}
bool Student::CopyFromOthers(float deltatime)
{
	bool worth = false;
	if (mCopyCurrentProgress >= mCopyMaxProgress)
	{
		for (int i = 0; i < ExamPaper::totalQuestion; i++)
		{
			if (mCopyFrom->mQuestions[i] == true && mPaper.mQuestions[i] == false)
			{
				worth = true;
				mPaper.mQuestions[i] = true;
				questionsCopied++;
				mCopyCurrentProgress = 0.0f;
				break;
			}
		}
		return worth;
	}
	else
	{
		mCopyCurrentProgress += mCopySpeed * deltatime;
	}
	return true;
}
void Student::DebugUI()
{
	std::string h = ("Student: " + std::to_string(mIndex));
	if(ImGui::CollapsingHeader( h.c_str()))
	{
		ImGui::Checkbox(("stop##" + h).c_str(), &stop);
		ImGui::Checkbox(("debugline##" + h).c_str(), &debugDraw);
		ImGui::Separator();
		ImGui::Text("alive:[  %s ]", (mActive ? "true" : "false"));
		ImGui::Text("curr state: [ %s ]", (mCurrentState.c_str()));
		ImGui::Text("patience:[  %f ]", (GetPatience()));
		ImGui::Text("destination:[  %f,%f ]", destination.x,destination.y);
		ImGui::Text("cheating: [ %s ]", (IsCheating() ? "true" : "false"));
		ImGui::Checkbox(("cheating##" + h).c_str(), &mCheat);
		ImGui::Separator();
		ImGui::Text("Que solved by cheat: [ %d ]", questionsCopied);
		ImGui::Text("Escaped from catch: [ %d ]", escapedFromCatch);
		ImGui::Text("Pretend Innocent: [ %d ]", pretendInnocent);
		ImGui::Separator();


		if (IsCheating())
		{
			ImGui::ProgressBar(mCopyCurrentProgress / mCopyMaxProgress);
		}
		GetExamPaper().DebugUI();

		if(ImGui::TreeNode(("Memory##: " + std::to_string(mIndex)).c_str()))
		{
 			int countten = 0;
			ImGui::Columns(2);
			for (const auto& m : mPerceptionModule->GetMemoryRecords())
			{
				ImGui::Text("Memory at time :[%f ]", m.lastRecordedTime);
				ImGui::Text("Importance :[%f ]", m.importance);
				for (const auto& p : m.properties)
				{
					ImGui::Text("Props :[%s] : [%f]", p.first.c_str(), p.second);
 				}
				
				ImGui::Spacing();
				ImGui::NextColumn();
				if (countten++ >= 3)
					break;
			}
			ImGui::Columns(1);
			ImGui::TreePop();
		}
 
 
		//for (size_t i = 0; i < ExamPaper::totalQuestion; i++)
		//{
		//	//ImGui::Text("[%d],", (GetExamPaper().mQuestions[i]));
		//	//ImGui::SameLine();
		//}
		ImGui::Separator();
 	}

}
const H::AI::MemoryRecords& Student::GetMemoryRecords() const { return mPerceptionModule->GetMemoryRecords(); }
#pragma endregion

//states
#pragma region movetoown

void MoveToOwnPaperState::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mGreenTextureId;

	agent.destination = agent.GetExamPaper().mPosition;
	agent.StartMoving();
	agent.SetObsAvoid(false);
	agent.maxSpeed *= 3.0f;
	//scanTime = agent.GetScanTime();
	//scanTimer = agent.GetScanTimer();
	scanTimer = 0.0f;


}

void MoveToOwnPaperState::Update(Student & agent, float deltatime)
{

	agent.LookAround(deltatime);
	agent.Move(deltatime);
	if (H::Math::Distance(agent.position, agent.GetExamPaper().mPosition) < 1.0f)
	{
		agent.ChangeState("Work");
		return;
	}

	if (scanTimer >= scanTime)
	{
		if (agent.IsCheating())
		{
			scanTimer = 0.0f;
			agent.ChangeState("Scan");
			return;
		}
	}
	else
	{
		scanTimer += deltatime;
	}

}

void MoveToOwnPaperState::Exit(Student & agent)
{
	agent.maxSpeed /= 3.0f;
}

#pragma endregion
void MoveToDestination::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mGreenTextureId;
	agent.SetCheat(true);
 	agent.StartMoving();
	agent.SetObsAvoid(true);
	agent.radius /= 4.0f;
	//scanTime = agent.GetScanTime();
	//scanTimer = agent.GetScanTimer();
	scanTimer = 0.0f;

}

void MoveToDestination::Update(Student & agent, float deltatime)
{
	agent.LookAround(deltatime);
	agent.Move(deltatime);
	if (H::Math::Distance(agent.position, agent.destination) < 1.0f)
	{
		agent.ChangeState("Cheat");
		return;
	}

	if (scanTimer >= scanTime)
	{
		agent.ChangeState("Scan");
  	}
	else
	{
		scanTimer += deltatime;
	}


}

void MoveToDestination::Exit(Student & agent)
{
	agent.radius *= 4.0f;
}
void FleeState::Enter(Student & agent)
{
	agent.RecovePatience(10.0f);
	(agent.mCurrentTextureId) = &agent.mGreenTextureId;
	agent.maxSpeed *= 3.0f;
	agent.StartMoving();
	agent.SetObsAvoid(true);

	//scanTime = agent.GetScanTime();
	//scanTimer = agent.GetScanTimer();
	scanTimer = 0.0f;
}

void FleeState::Update(Student & agent, float deltatime)
{
	agent.LookAround(deltatime);
 	agent.Move(deltatime);
	SimpleDraw::AddScreenRect({ agent.destination, {20.0f,20.0f} }, Colors::LightYellow);
	SimpleDraw::AddScreenLine( agent.destination,agent.position , Colors::LightYellow);
	SimpleDraw::AddScreenRect({ agent.position,{20.0f,20.0f} }, Colors::LightYellow);
	

	if (H::Math::Distance(agent.position, agent.destination) < 1.0f)
	{
		agent.ChangeState("Scan");
		return;
	}
	if (scanTimer >= scanTime)
	{
		agent.ChangeState("Scan");
		return;
	}
	else
	{
		scanTimer += deltatime;
	}


}

void FleeState::Exit(Student & agent)
{
	agent.maxSpeed /= 3.0f;
}

void ThinkState::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mBlueTextureId;
	agent.SetObsAvoid(false);

 }

void ThinkState::Update(Student & agent, float deltatime)
{

	if (agent.GetExamPaper().IsCompeleted())
	{
		agent.ChangeState("MoveToOwn");
		return;

	}

	const H::AI::MemoryRecords& mems = agent.GetMemoryRecords();
	if (mems.size() > 0)
	{
		auto& importantMem = mems.front();
		agent.lastMemImportance = importantMem.importance;//

		auto it = importantMem.properties.find("TeacherLocation");
		if (it != importantMem.properties.end()/* && agent.IsCheating()*/)
		{
			H::Math::Vector2 tLoc = std::get<H::Math::Vector2>(it->second);
			float dest = H::Math::Distance(tLoc, agent.position);
			if (agent.IsCheating() && dest < 250.0f)
			{
				float destToOwn = H::Math::Distance(agent.GetExamPaper().mPosition, agent.position);
				if (destToOwn < dest * 1.0f)
				{
					agent.destination = agent.GetExamPaper().mPosition;
 					agent.ChangeState("MoveToOwn");
					return;

				}
				if (dest > 100.0f)
				{
					auto sight_it = importantMem.properties.find("TeacherSight");
					if (sight_it != importantMem.properties.end())
					{
						H::Math::Vector2 tSight = std::get<H::Math::Vector2>(sight_it->second);
						sight_it = importantMem.properties.find("TeacherViewAngle");
						float va = std::get < float>(sight_it->second);
						//TeacherViewAngle
						float theta = H::Math::Dot(H::Math::Normalize(agent.position - tLoc), tSight); //< agent.cosViewAngle)
						if (theta > va)
						{
 							Vector2 fleeLoc = H::Math::Normalize((agent.position - tLoc) + RandomVec2(-10.0f,10.0f));
							fleeLoc *= dist_Flee * 5.0f;
							fleeLoc += agent.position;
							agent.destination = fleeLoc;
							agent.ChangeState("Flee");
							return;

						}

					}
				}
				else
				{
					Vector2 fleeLoc = H::Math::Normalize((agent.position - tLoc) + RandomVec2());
					fleeLoc *= dist_Flee * 3.0f;
					fleeLoc += agent.position;
					agent.destination = fleeLoc;
					agent.ChangeState("Flee");
					return;
				}
			}

			//}
			//TeacherSight
		}

		 it = importantMem.properties.find("QuestionIndex");
		int indexToCopy = 0;
		if (it != importantMem.properties.end())
		{
			indexToCopy = std::get<int>(it->second);
			if (indexToCopy == agent.mIndex)
			{
				agent.RecovePatience(10);
				agent.ChangeState("MoveToOwn");
				return;

			}
			else
			{
				it = importantMem.properties.find("QuestionLocation");
				if (it != importantMem.properties.end())
				{
					agent.destination = std::get<H::Math::Vector2>(it->second);
					agent.SetPaperCopyFrom(indexToCopy);
					agent.ChangeState("MoveToDest");
					return;

				}
			}
		}
 


	}//if mem > 0

 	agent.ChangeState("MoveToOwn");
	return;
}

void ThinkState::Exit(Student & agent)
{
	//depends on memory
}

void ScanState::Enter(Student & agent)
{
	agent.viewRadius *= 4.0f;
	agent.viewAngle *= 1.2f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.maxSpeed *= 3.0f;
	(agent.mCurrentTextureId) = &agent.mBlueTextureId;
	scanTimer = 0.0f;
	scanTurnTimes = 0;
	agent.TurnAround();
	agent.SetObsAvoid(false);

}

void ScanState::Update(Student & agent, float deltatime)
{
	agent.LookAround(deltatime);

 	if (scanTimer >= scanTime )
	{
		agent.TurnAround();
		scanTimer = 0.0f;
		scanTurnTimes++;
	}
	else
	{
		scanTimer += deltatime;
	}

	if (scanTurnTimes >= 2)
	{
		agent.ChangeState("Think");
		return;

	}
	agent.Move(deltatime);

}

void ScanState::Exit(Student & agent)
{
	agent.viewRadius /= 4.0f;
	agent.viewAngle /= 1.2f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.maxSpeed /= 3.0f;

	//depends on memory
}
#pragma region work
void WorkState::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mNormalTextureId;
	agent.SetCheat(false);
	agent.SetObsAvoid(false);

}

void WorkState::Update(Student & agent, float deltatime)
{
 
	if (H::Math::Distance(agent.position, agent.GetExamPaper().mPosition) > 5.0f)
	{
		agent.ChangeState("MoveToOwn");// depends on mem, move to others or move to its paper
	}
	else if (agent.GetExamPaper().IsCompeleted())
	{
		agent.ChangeState("Finish");
		return;

	}
	else
	{
		agent.SolveQuestion();
	}

	if (agent.GetPatience() <= 0.0f)
	{
		agent.ChangeState("Scan");
		return;

	}
}

void WorkState::Exit(Student & agent)
{
}

#pragma endregion

void CheatState::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mRedTextureId;
	vigilantTimer = 0.0f;
	fullProgress = agent.GetCopyProgressFull();
	currProgress = agent.GetCopyProgress();
	agent.viewRadius *= 2.0f;
	agent.viewAngle *= 2.0f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.SetObsAvoid(false);

}

void CheatState::Update(Student & agent, float deltatime)
{
	agent.LookAround(deltatime);
	if (vigilantTimer >= vigilantTime)
	{
		agent.RecovePatience(5);
		vigilantTimer = 0.0f;
		agent.ChangeState("Scan");
		return;

	}
	else
	{
		vigilantTimer += deltatime;
	}
	//agent.CopyFromOthers(deltatime);
 
	if (!agent.CopyFromOthers(deltatime))
	{
		agent.RecovePatience(15);
		//agent.destination = agent.GetExamPaper().mPosition;
		agent.ChangeState("MoveToOwn");
		return;

	}

	//if (H::Math::Distance(agent.position, agent.GetExamPaper().mPosition) > 5.0f)
	//{
	//	// depends on mem, move to others or move to its paper
	//}
 
}

void CheatState::Exit(Student & agent)
{
	agent.viewRadius /= 2.0f;
	agent.viewAngle /= 2.0f;
	agent.cosViewAngle = cosf(agent.viewAngle);
}
void FinishState::Enter(Student & agent)
{
	(agent.mCurrentTextureId) = &agent.mFinishTextureId;
	agent.SetObsAvoid(false);

}

void FinishState::Update(Student & agent, float deltatime)
{
	SimpleDraw::AddScreenRect({ agent.position,{40.0f,40.0f} }, Colors::Yellow);
}

void FinishState::Exit(Student & agent)
{
}
void VisualSensor::AddAllPaperNearby(H::AI::Agent& agent)
{
	nearPaper = (reinterpret_cast<Student*>(&agent))->allPapers;
}

void VisualSensor::Update(Agent& agent, MemoryRecords& memory, float deltatime)
{
	reinterpret_cast<Student*>(&agent)->seesTeacher = nullptr;
	//depends on memory
	agent.neighbors = agent.world.GetNeighborhood({ agent.position,agent.viewRadius });

	for (auto& n : agent.neighbors)
	{
		if (n == &agent && n->mTag != 1)
			continue;

		float dist = H::Math::Distance(n->position, agent.position);
		if (dist < agent.viewRadius )
		{
			if (dist < 130.0f)
			{
				MemoryRecord mem;
				mem.lastRecordedTime = agent.world.runningTime;

				if (n->mTag == 1) //teacher
				{
					mem.properties["TeacherLocation"] = n->position;
					mem.properties["TeacherSight"] = H::Math::Normalize( agent.position - n->position );
					mem.properties["TeacherViewAngle"] = n->cosViewAngle;
					reinterpret_cast<Student*>(&agent)->seesTeacher = reinterpret_cast<Teacher*>(n);

				}
				memory.push_back(mem);

			}
			else
			{
				if (H::Math::Dot(H::Math::Normalize(n->position - agent.position), agent.heading) > agent.cosViewAngle)
				{
					MemoryRecord mem;
					mem.lastRecordedTime = agent.world.runningTime;

					if (n->mTag == 1) //teacher
					{
						mem.properties["TeacherLocation"] = n->position;
						mem.properties["TeacherSight"] = n->heading;
						mem.properties["TeacherViewAngle"] = n->cosViewAngle;
						reinterpret_cast<Student*>(&agent)->seesTeacher = reinterpret_cast<Teacher*>(n);

					}
					memory.push_back(mem);

				}

			}


		}

 	}

	for (auto& p : *(reinterpret_cast<Student*>(&agent))->allPapers)
	{
		if (H::Math::Distance(p->mPosition, agent.position) < agent.viewRadius)
		{
			//if (reinterpret_cast<Student*>(&agent)->debugDraw)
			//{
			//	SimpleDraw::AddScreenLine(agent.position, p->mPosition, Colors::Cyan);
			//	SimpleDraw::AddScreenRect({ p->mPosition,{40.0f,40.0f} }, Colors::Cyan);
			//}
			MemoryRecord mem;
			mem.lastRecordedTime = agent.world.runningTime;
			mem.properties["Question"] = p->QuestionAnswerd();
			mem.properties["QuestionIndex"] = p->mIndex;
			mem.properties["QuestionLocation"] = p->mPosition;

			memory.push_back(mem);
		}
	}


}