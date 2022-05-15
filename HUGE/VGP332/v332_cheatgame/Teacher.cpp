#include "Teacher.h"
#include "SteerBehaviors.h"
#include "Student.h"

Teacher::Teacher(H::AI::AIWorld & world)
	:Agent(world)
{
	mPatrolTime = 3.0f;
	mPatrolTimer = 0.0f;

	mTag = 1;
	viewAngle = H::Math::Constants::Pi * 0.2f;
	viewRadius = 300.0f;
	cosViewAngle = cosf(viewAngle);
	position = { 120.0f,110.0f };
	destination = { 0.0f,0.0f };
	maxSpeed = 100.0f;
}

void Teacher::Init(std::vector<std::unique_ptr<Student>>* allstudent)
{

	mTextureId = TextureManager::Get()->LoadTexture(L"sheepScholarOL_64.png");
	mHuntingMarkTextureId = TextureManager::Get()->LoadTexture(L"aim64.png");
	mPistoTextureId = TextureManager::Get()->LoadTexture(L"pisto32.png");
	mPartolPts.resize(max_partol_point);
	for (int i = 0; i < max_partol_point; i++)
	{
		mPartolPts[i] = { (i % 3) * 350.0f + 250.0f,(i / 3)  * 350.0f + 250.0f };
	}
	destination = mPartolPts[0];

	allStudents = allstudent;
	studentInsight.reserve(allStudents->size());

	//[SteeringModule]

	mSteeringModule = std::make_unique<H::AI::SteeringModule>(*this);
	mSteeringModule->AddBehavior<Seek>("Seek")->SetActive(true);
	mSteeringModule->AddBehavior<Arrive>("Arrive")->SetActive(true);
	mSteeringModule->AddBehavior<ObsAvoid>("ObsAvoid")->SetActive(true);

	//[StateMachine]
	mStateMachine = std::make_unique<H::AI::StateMachine<Teacher>>(*this);
	mStateMachine->AddState<TeacherScanState>();
	mStateMachine->AddState<TeacherThinkState>();
	mStateMachine->AddState<TeacherMoveToDestination>();
	mStateMachine->AddState<TeacherHunting>();
	mStateMachine->AddState<TeacherPatrol>();
	//mStateMachine->ChangeState("Patrol");
	ChangeState("Patrol");


	auto mImportCalc = [this](H::AI::MemoryRecord& mem)
	{
		//auto it = mem.properties.find("CheatStudent");
 		//if (it != mem.properties.end())
		//{
			auto it = mem.properties.find("CheatStudentLocation");

			if (it != mem.properties.end())
			{
				float dist = H::Math::Distance(position, std::get<H::Math::Vector2>(it->second));
				mem.importance += 500.0f - dist;
			}
			//CheatStudentActive
			it = mem.properties.find("CheatStudentActive");

			if (it != mem.properties.end())
			{
				bool active =  (std::get<int>(it->second)) == 1 ? true : false;
				if (active == false)
				{
					mem.importance = -1000.0f;
				}
				//mem.importance = active ? mem.importance : 0.0f;
			}
 		//}

	};

	mPerceptionModule = std::make_unique<H::AI::PerceptionModule>(*this, mImportCalc);
	mPerceptionModule->AddSensor<TeacherVisualSensor>("Vision");
	mPerceptionModule->mMemorySpan = 5.0f;
}
bool Teacher::Catch()
{
	if (!currChasing)
	{
		return false;
	}

	destination = currChasing->position;
	if (H::Math::Distance(position,currChasing->position) <= 5.0f )
	{
		if (!currChasing->IsCheating() || !currChasing->IsActive())
		{
			currChasing->PretendInnocent();
			return false;
		}
 
		currChasing->GetCaught();
 		killCount++;
		//return false;
	}
	return true;
}

void Teacher::Move(float deltatime)
{
	auto force = mSteeringModule->Calculate();
	auto a = force / mass;
	velocity += a * deltatime;
 	position += velocity * deltatime;

	if (H::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = H::Math::Normalize(velocity);
	}

}
void Teacher::Patrol()
{
	if (mPatrolTimer >= mPatrolTime)
	{
		//currPatrolPt = (currPatrolPt + 1) % (max_partol_point - 1);
		currPatrolPt =  (rand() % (max_partol_point - 1)) % (max_partol_point - 1);

		destination = mPartolPts[currPatrolPt];
		mPatrolTimer = 0.0f;
	}
}

void Teacher::Update(float deltatime)
{
	if (!stop)
	{
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

		mStateMachine->UpDate(deltatime);
		UpdateTimer(deltatime);
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
	}
	//Move(deltatime);
	//Patrol();
}
void Teacher::LookAround(float deltatime)
{
	//	Agents nearby = world.GetNeighborhood({position,viewRadius});
	mPerceptionModule->Update(deltatime);

}
void Teacher::TurnAround()
{
	const static float rad60 = H::Math::Constants::Pi / 3.0f;
	const static float cos60 = cosf(rad60);
	const static float sin60 = sinf(rad60);
	Vector2 des1{ heading.x * cos60 - heading.y * sin60, heading.x * sin60 + heading.y * cos60 };

	destination = des1 * 55.0f + position;
  }
void Teacher::ChangeState(std::string statename)
{
	mStateMachine->ChangeState(statename);
	mCurrentState = std::move(statename);
}
void Teacher::StartMoving()
{
	mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(true);
	mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(true);
}

void Teacher::StopMoving()
{
	mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(false);
	mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(false);
}
void Teacher::SetObsAvoid(bool active)
{
	mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid")->SetActive(active);
}
const H::AI::MemoryRecords& Teacher::GetMemoryRecords() const { return mPerceptionModule->GetMemoryRecords(); }

bool Teacher::SetChasingTarget(int index)
{
	currChasing = (*allStudents)[index].get();
	if (currChasing->IsActive() && currChasing->IsCheating())
	{
		currChasing->debugDraw = true;
		return true;
	}
	StopChasing();
	return false;
 }

 

void Teacher::StopChasing()
{
	currChasing->debugDraw = false;
	currChasing = nullptr; 
}

void Teacher::DebugUI()
{
	if (ImGui::CollapsingHeader("Teacher"))
	{
		ImGui::Checkbox("stop##teacher", &stop);
		ImGui::Text("pos[ %f,%f]",position.x, position.y);
		ImGui::Text("dest[ %f,%f]",destination.x, destination.y);
		ImGui::Text("curr state:[ %s ]", mCurrentState.c_str());
		if (currChasing != nullptr)
		{
			ImGui::Text("curr target index :[ %d ]", currChasing->mIndex);
			ImGui::Text("curr target pos:[ %f ]",  currChasing->position.x,  currChasing->position.y);
 			currChasing->DebugUI();
		}
		ImGui::Separator();
		ImGui::Text("kill %d", killCount);
		if (ImGui::TreeNode("t Memory : "))
		{

			int countten = 0;
			ImGui::Columns(2);
			for (const auto& m : mPerceptionModule->GetMemoryRecords())
			{
				ImGui::Text("Memory at time :[%f ]", m.lastRecordedTime);
				ImGui::Text("Importance :[%f ]", m.importance);
				for (const auto& p : m.properties)
				{
					ImGui::Text("Props :[%s]", p.first.c_str());
				}
				ImGui::Spacing();
				ImGui::NextColumn();
				if (countten++ >= 3)
					break;
			}

			ImGui::TreePop();
		}

	}


}

void Teacher::Render()
{
	for (auto& p : mPartolPts)
	{
		SimpleDraw::AddScreenRect({ p,{20.0f,20.0f} }, Colors::OrangeRed);
	}
	
	//view frust
	float cosviewhalf = cosf(viewAngle/**0.5f*/);
	float sinviewhalf = sinf(viewAngle/**0.5f*/);
	Vector2 vf1{ heading.x * cosviewhalf - heading.y * sinviewhalf, heading.x * sinviewhalf + heading.y * cosviewhalf };
	SimpleDraw::AddScreenLine(position, position + vf1 * viewRadius, Colors::Red);
	float n_cosviewhalf = cosf(-viewAngle /** 0.5f*/);
	float n_sinviewhalf = sinf(-viewAngle /** 0.5f*/);
	Vector2 vf2{ heading.x * n_cosviewhalf - heading.y * n_sinviewhalf, heading.x * n_sinviewhalf + heading.y * n_cosviewhalf };
	SimpleDraw::AddScreenLine(position, position + vf2 * viewRadius, Colors::Red);
	//SimpleDraw::AddScreenLine(position + vf1 * viewRadius, position + vf2 * viewRadius, Colors::Red);
	Vector2 mid = position + heading * viewRadius;
	SimpleDraw::AddScreenLine(position + vf1 * viewRadius, mid, Colors::Red);
	SimpleDraw::AddScreenLine(mid, position + vf2 * viewRadius, Colors::Red);


	//student saw
	for (const auto sindex : studentInsight)
	{
		Vector2 sp = (*allStudents)[sindex]->position;
		SimpleDraw::AddScreenLine(position, sp, Colors::LightPink);
		SimpleDraw::AddScreenRect({ sp,30.0f }, Colors::LightPink);
	}
	//dest
	SimpleDraw::AddScreenRect({ destination,25.0f }, Colors::DarkMagenta);
	SimpleDraw::AddScreenLine(position, destination, Colors::DarkMagenta);
 
	

	H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), { position.x - 32.0f, position.y - 32.0f });
	if (currChasing != nullptr)
	{
		H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mHuntingMarkTextureId)), { currChasing->position.x - 32.0f, currChasing->position.y - 32.0f});
		H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mPistoTextureId)), { position.x + 40.0f, position.y - 40.0f });
		
	}
}


void Teacher::UpdateTimer(float deltatime)
{
 
	if (mPatrolTimer < mPatrolTime)
	{
		mPatrolTimer += deltatime;
	}
	 
}

void TeacherVisualSensor::Update(H::AI::Agent & agent, H::AI::MemoryRecords & memory, float deltatime)
{
	
	reinterpret_cast<Teacher*>(&agent)->studentInsight.clear();

	agent.neighbors = agent.world.GetAllAgents();
	for (auto n : agent.neighbors)
	{
		if (n == &agent)
			continue;

		float dist = H::Math::Distance(n->position, agent.position);

		if (dist < agent.viewRadius)
		{
			if (dist < 40.0f)
			{
				reinterpret_cast<Teacher*>(&agent)->studentInsight.push_back(reinterpret_cast<Student*>(n)->mIndex);

				if (reinterpret_cast<Student*>(n)->IsCheating())
				{

					SimpleDraw::AddScreenLine(agent.position, n->position, Colors::Yellow);
					SimpleDraw::AddScreenCircle({ n->position,30.0f }, Colors::Yellow);
 					H::AI::MemoryRecord mem;
					mem.lastRecordedTime = agent.world.runningTime;
					mem.properties["CheatStudent"] = reinterpret_cast<Student*>(n)->mIndex;
					mem.properties["CheatStudentLocation"] = (n)->position;
					mem.properties["CheatStudentActive"] = (reinterpret_cast<Student*>(n)->IsActive()) ? 1 : 0;
					memory.push_back(mem);
				}

			}
			else
			{

				if (H::Math::Dot(H::Math::Normalize(n->position - agent.position), agent.heading)
			> agent.cosViewAngle)
				{
					reinterpret_cast<Teacher*>(&agent)->studentInsight.push_back(reinterpret_cast<Student*>(n)->mIndex);

					if (reinterpret_cast<Student*>(n)->IsCheating())
					{

						//SimpleDraw::AddScreenLine(agent.position, n->position, Colors::Yellow);
  						H::AI::MemoryRecord mem;
						mem.lastRecordedTime = agent.world.runningTime;
						mem.properties["CheatStudent"] = reinterpret_cast<Student*>(n)->mIndex;
						mem.properties["CheatStudentLocation"] = (n)->position;
						mem.properties["CheatStudentActive"] = (reinterpret_cast<Student*>(n)->IsActive()) ? 1 : 0;
						memory.push_back(mem);
					}
				}

			}
		}
	}

}

void TeacherScanState::Enter(Teacher & agent)
{
	agent.viewAngle *= 1.5f;
	agent.viewRadius *= 2.0f;
	agent.cosViewAngle = cosf(agent.viewAngle);

 	scanTimer = 0.0f;
	scanTwice = 0;
	agent.TurnAround();
	agent.SetObsAvoid(true);

}

void TeacherScanState::Update(Teacher & agent, float deltatime)
{

	if (scanTimer >= scanTime)
	{
		agent.TurnAround();
		scanTimer = 0.0f;
		scanTwice++;
	}
	else
	{
		scanTimer += deltatime;
	}

	if (scanTwice == 2)
	{
		agent.ChangeState("Think");
		return;
	}
	agent.LookAround(deltatime);
	agent.Move(deltatime);

}

void TeacherScanState::Exit(Teacher & agent)
{
	agent.viewRadius *= 0.5f;
	agent.viewAngle /= 1.5f;
	agent.cosViewAngle = cosf(agent.viewAngle);

}

void TeacherThinkState::Enter(Teacher & agent)
{
	timer = 0.0f;
	agent.SetObsAvoid(false);

}

void TeacherThinkState::Update(Teacher & agent, float deltatime)
{
	const H::AI::MemoryRecords& mems = agent.GetMemoryRecords();
	if (mems.size() > 0)
	{
		auto& importantMem = mems.front();

		auto indexIt = importantMem.properties.find("CheatStudent");

		if (indexIt != importantMem.properties.end())
		{
			auto locIt = importantMem.properties.find("CheatStudentLocation");
			if (locIt != importantMem.properties.end())
			{
				auto activeIt = importantMem.properties.find("CheatStudentActive");
				if (activeIt != importantMem.properties.end())
				{
					if ((std::get<int>(activeIt->second)) == 1 )
					{
						//agent.destination = std::get<H::Math::Vector2>(it->second);
						if (agent.SetChasingTarget(std::get<int>(indexIt->second)))
						{
 							agent.ChangeState("Hunt");
							return;
						}
						else
						{
							agent.ChangeState("Scan");
							return;
						}
 
					}
				}

				
			}
		}
	}
	if (timer >= time)
	{
 		agent.ChangeState("Patrol");
		return;
	}
	else
	{
		timer += deltatime;
	}

	//agent.ChangeState("Patrol");

}

void TeacherThinkState::Exit(Teacher & agent)
{
}

void TeacherMoveToDestination::Enter(Teacher & agent)
{
	//agent.destination = agent.GetExamPaper().mPosition;
	agent.StartMoving();
	scanTimer = 0.0f;
	agent.viewRadius *= 0.5f;
	agent.viewAngle *= 1.2f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.SetObsAvoid(true);

}

void TeacherMoveToDestination::Update(Teacher & agent, float deltatime)
{

	agent.LookAround(deltatime);
	agent.Move(deltatime);


	if (scanTimer >= scanTime)
	{
		scanTimer = 0.0f;

 		agent.ChangeState("Scan");
 	}
	else
	{
		scanTimer += deltatime;
	}

}

void TeacherMoveToDestination::Exit(Teacher & agent)
{
	agent.viewRadius *= 2.0f;
	agent.viewAngle /= 1.2f;
	agent.cosViewAngle = cosf(agent.viewAngle);

}

void TeacherHunting::Enter(Teacher & agent)
{
	agent.viewAngle *= 0.5f;
	agent.viewRadius *= 2.0f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.maxSpeed *= 2.5f;
	scanTimer = 0.0f;
	catchTimer = 0.0f;
	//agent.destination = agent.currChasing->position;
	agent.StartMoving();
	agent.SetObsAvoid(true);

}

void TeacherHunting::Update(Teacher & agent, float deltatime)
{
	agent.LookAround(deltatime);
	agent.Move(deltatime);
	if (catchTimer >= catchTime)
	{
		catchTimer = 0.0f;
		agent.SetObsAvoid(false);
		if (agent.Catch() == false)
		{
			//agent.StopChasing();
			agent.ChangeState("Think");
			return;
		}
  	}
	else
	{
		catchTimer += deltatime;
	}

	if (scanTimer >= scanTime)
	{
		scanTimer = 0.0f;
		if (!agent.currChasing)
		{
			agent.currChasing->EscapeFromCatch();
		}
		agent.ChangeState("Think");
		return;
	}
	else
	{
		scanTimer += deltatime;
	}

}

void TeacherHunting::Exit(Teacher & agent)
{
	agent.StopChasing();
	agent.viewAngle *= 2.0f;
	agent.viewRadius *= 0.5f;
	agent.cosViewAngle = cosf(agent.viewAngle);
	agent.maxSpeed /= 2.5f;
}



void TeacherPatrol::Enter(Teacher & agent)
{
	scanTimer = 0.0f;
	agent.StartMoving();
	agent.SetObsAvoid(true);

}

void TeacherPatrol::Update(Teacher & agent, float deltatime)
{
	agent.Patrol();
	agent.Move(deltatime);
	agent.LookAround(deltatime);
	if (scanTimer >= scanTime)
	{
		scanTimer = 0.0f;
		agent.ChangeState("Scan");
	}
	else
	{
		scanTimer += deltatime;
	}


}

void TeacherPatrol::Exit(Teacher & agent)
{
}
