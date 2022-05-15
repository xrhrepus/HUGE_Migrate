#include "FSMObjects.h"
#pragma region BushFunctions
//Bush::Bush(const Bush & other)
//{
//
//}
Bush::Bush(H::Math::Vector2 initpos)
	:mPosition(initpos)
{
	Init();
}

//Bush::~Bush() {}
void Bush::Init()
{
	//mStateMachine = std::make_unique<H::AI::StateMachine<Bush>>(*this);
	//mTexture = TextureManager::Get()->LoadTexture(L"fruit42.png");
}

void Bush::Update(float deltatime)
{
	ProduceFruit(deltatime);
}

void Bush::Render()
{
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(mTexture), mPosition);

}

void Bush::ProduceFruit(float deltatime)
{
	if (mProduceTimer < mProduceTime)
	{
		mProduceTimer += deltatime * mProduceSpeed;
	}
	else if (mCurrentFruitNum < mFruitCapacity)
	{
		mCurrentFruitNum += 1;
		mProduceTimer = 0.0f;
	}
	else
	{
		return;
	}
}

int Bush::Harvest(int num )
{
	
	if (mCurrentFruitNum >= num)
	{
		mCurrentFruitNum -= num;
		return num;
	}
	int ret = mCurrentFruitNum;
	mCurrentFruitNum = 0;
	return ret;
}

void Bush::SetPosition(H::Math::Vector2 pos)
{
	mPosition = pos;
}

void Bush::SetTextureId(H::Graphics::TextureId tId)
{
	mTexture = tId;
}
 
int Bush::GetFruitCapacity() { return mFruitCapacity; }
int Bush::GetCurrentFruitNum() { return mCurrentFruitNum; }
int Bush::GetProduceSpeed() { return mProduceSpeed; }
int Bush::GetProduceTimer() { return mProduceTimer; }

H::Math::Vector2 Bush::GetPosition()
{
	return mPosition;
}

 
#pragma endregion

#pragma region SheepFunctions
Sheep::Sheep()
	:mPosition({ 500.0f,500.0f }), mHomePosition({500.0f,500.0f})
{
	Init();
}

Sheep::~Sheep()
{
}

 
void Sheep::Init()
{
	//mTexture = TextureManager::Get()->LoadTexture(L"sheep.png");


	mStateMachine = std::make_unique<H::AI::StateMachine<Sheep>>(*this);
	mStateMachine->AddState<Sheep_FindState>();
	mStateMachine->AddState<Sheep_MoveState>();
	mStateMachine->AddState<Sheep_WorkState>();
	mStateMachine->AddState<Sheep_BackState>();

	mBushes.reserve(5);
	mBushPriority.reserve(mBushes.size());
}

void Sheep::ChangeState(std::string statename)
{
	mStateMachine->ChangeState(statename);
	mCurrentState = std::move(statename);
}

Bush* Sheep::GetBushWorkingOn()
{
	return mTargetBush;
}

 
void Sheep::Update(float deltatime)
{
	mStateMachine->UpDate(deltatime);
	//Move(deltatime);
}
void Sheep::Render()
{
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(mTexture), mPosition);

}
void Sheep::Move(float deltatime)
{
	if (H::Math::Distance(mDestination, mPosition) > 0.5f)
	{
		mPosition += H::Math::Normalize(mDestination - mPosition) * mMoveSpeed * deltatime;
	}

}

void Sheep::FindClosestBush()
{
	float min_dist = std::numeric_limits<float>::max();

	//if (mTargetBush == nullptr) // no target
	//{
	//	for (Bush* bush : mBushes)
	//	{
	//		float d = H::Math::Distance(mPosition, bush->GetPosition());
	//		if (d < min_dist)
	//		{
	//			min_dist = d;
	//			mDestination = bush->GetPosition();
	//			//mTargetBush = std::make_unique<Bush>(bush);
	//			mTargetBush = (bush);
	//		}
	//	}
	//}
	//else
	{
		H::Math::Vector2 new_destination;
		//Bush* bRef = nullptr;
		for (Bush* bush : mBushes)
		{

			float d = H::Math::Distance(mPosition, bush->GetPosition());
			if (d < min_dist)
			{
				min_dist = d;
				new_destination = bush->GetPosition();
				//bRef = &bush;
				mTargetBush = (bush);

			}
		}
		if (min_dist < H::Math::Distance(mPosition, mDestination))
		{
			mDestination = new_destination;
			//mTargetBush = std::make_unique<Bush>(*bRef);
			//mTargetBush = (bRef);


		}

	}


}

void Sheep::ComputePriority()
{
	//avaliable fruit ?
	for (int i = 0; i < mBushes.size() ; ++i)
	{
		//if ((mBushes[i])->GetCurrentFruitNum() > 3 ) 
		//{
		//	mBushPriority[i] += Priority::L_4;
		//}
		//else if ((mBushes[i])->GetCurrentFruitNum() > 0)
		//{
		//	mBushPriority[i] += Priority::L_3;
		//}
		//else if((mBushes[i])->GetProduceTimer() > 5.0f)
		//{
		//	mBushPriority[i] += Priority::L_2;
		//}
		//else
		//{
		//	mBushPriority[i] += Priority::L_0;
		//}
		switch (mBushes[i]->GetCurrentFruitNum())
		{
		case 0: mBushPriority[i] += Priority::L_0; break;
		case 1: mBushPriority[i] += Priority::L_1; break;
		case 2: mBushPriority[i] += Priority::L_2; break;
		case 3: mBushPriority[i] += Priority::L_3; break;
		case 4: mBushPriority[i] += Priority::L_4; break;
		case 5: mBushPriority[i] += Priority::L_5; break;

		default:
			break;
		}

		//in progress?
		if ((mBushes[i])->GetProduceTimer() > 5.0f)
			{
				mBushPriority[i] += Priority::L_1;
			}
	}

	//distance?

	float min_dist = std::numeric_limits<float>::max();

 		//H::Math::Vector2 new_destination;
		//Bush* bRef = nullptr;
		int bIndex = -1;
		for (int i = 0; i < mBushes.size();++i)
		{

			float d = H::Math::Distance(mPosition, mBushes[i]->GetPosition());
			if (d < min_dist)
			{
				min_dist = d;
				bIndex = i;
				//new_destination = bush->GetPosition();
				//bRef = &bush;
				//mTargetBush = (bush);

			}
		}
		if ((min_dist < H::Math::Distance(mPosition, mDestination)) && (bIndex > -1))
		{
			mBushPriority[bIndex] += Priority::L_1;
			//mDestination = new_destination;
 		}
 


	//if (mTargetBush == nullptr)
	{
		int max = (int)Priority::L_0;
		int bestIndex = -1;
		for (int i = 0; i < mBushPriority.size(); i++)
		{
			if ((int)mBushPriority[i] > max)
			{
				bestIndex = i;
				max = (int)mBushPriority[i];
			}
		}
		if (bestIndex > -1)
		{
			mTargetBush = mBushes[bestIndex];
			mDestination = mTargetBush->GetPosition();
		}
	}


}

void Sheep::ResetPriority()
{
	for (auto& p : mBushPriority)
	{
		p = Priority::L_0;
	}
}

bool Sheep::ReachedDestination()
{
	return H::Math::Distance(mPosition,mDestination) < 5.0f;
}

void Sheep::SetTextureId(H::Graphics::TextureId tId)
{
	mTexture = tId;
}

void Sheep::SetMoveSpeed(float spd)
{
	mMoveSpeed = spd;
}

float & Sheep::GetMoveSpeed()
{
	return mMoveSpeed;
}

void Sheep::AddTargetBush(Bush* bush)
{
	mBushes.push_back(bush);
	mBushPriority.push_back(Sheep::Priority::L_0);
}

void Sheep::AddFruitOwned(int n)
{
	mFruitOwned += n;
}

void Sheep::StoreAllFruits( )
{
	mTargetBush = nullptr;
	mFruitAtHome += mFruitOwned;
	mFruitOwned = 0;
}

H::Math::Vector2& Sheep::GetDestination()
{
	return mDestination;
}

H::Math::Vector2 & Sheep::GetPosition()
{
	return mPosition;
}

H::Math::Vector2 Sheep::GetHomePosition()
{
	return mHomePosition;
}

int Sheep::GetFruitAtHome()
{
	return mFruitAtHome;
}

int Sheep::GetFruitOwned()
{
	return mFruitOwned;
}

std::string Sheep::GetCurrentState()
{
	return mCurrentState;
}

std::vector<int>& Sheep::GetBushPriority()
{
	return mBushPriority;
}

 

//================

void Sheep_MoveState::Enter(Sheep & agent)
{
	//destination = agent.GetDestination();
	refindTimer = 0.0f;


}

void Sheep_MoveState::Update(Sheep & agent, float deltatime)
{
	agent.Move(deltatime);
	if (agent.ReachedDestination())
	{
		agent.ChangeState("Work");
	}
	else
	{
		if (refindTimer < refindTime)
		{
			refindTimer += deltatime;
		}
		else
		{
			agent.ChangeState("Find");
			refindTimer = 0.0f;
		}

	}

	

}

void Sheep_MoveState::Exit(Sheep & agent)
{
}


//================
void Sheep_WorkState::Enter(Sheep & agent)
{
	//mBushWorkingOn = std::make_unique<Bush>(agent.GetBushWorkingOn());
	mBushWorkingOn =  (agent.GetBushWorkingOn());

	//workProgress = mBushWorkingOn->GetProduceTimer();
	workProgress = 0.0f;

}

void Sheep_WorkState::Update(Sheep & agent, float deltatime)
{
	if (mBushWorkingOn->GetCurrentFruitNum() <= 0)
	{
		//wait
		return;

		//find another bush



	}
	else if (workProgress < 100.0f)
	{
		workProgress++;
	}
	else
	{
		agent.AddFruitOwned(mBushWorkingOn->Harvest(2));
		agent.ChangeState("Back");
	}
}

void Sheep_WorkState::Exit(Sheep & agent)
{
	workProgress = 0.0f;

}

#pragma endregion

//
void Sheep_FindState::Enter(Sheep & agent)
{
	agent.ResetPriority();
 }

void Sheep_FindState::Update(Sheep & agent, float deltatime)
{
	//agent.FindClosestBush();
	agent.ComputePriority();

	if (agent.GetBushWorkingOn() != nullptr)
	{
		agent.ChangeState("Move");
	}

}

void Sheep_FindState::Exit(Sheep & agent)
{
}

//=
void Sheep_BackState::Enter(Sheep & agent)
{
	//destination = agent.GetHomePosition();
	agent.GetDestination() = agent.GetHomePosition();
}

void Sheep_BackState::Update(Sheep & agent, float deltatime)
{
	agent.Move(deltatime);
	if (agent.ReachedDestination())
	{
		agent.StoreAllFruits();
		
		agent.ChangeState("Find");
	}
}

void Sheep_BackState::Exit(Sheep & agent)
{
}
