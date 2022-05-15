#include "Precompiled.h"
#include "PerceptionModule.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace H::AI;

H::AI::PerceptionModule::PerceptionModule(Agent & agent, ImportanceCalculator calculator)
	:mAgent(agent),
	mCalculator(std::move(calculator))
{
}

void H::AI::PerceptionModule::Update(float deltatime)
{

	for (auto& [name,sensor] : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltatime);
	}

	////todo remove any  records older than mem span
	for (auto iter = mMemory.begin();iter != mMemory.end(); )
	{
		if (iter->lastRecordedTime + mMemorySpan < mAgent.world.runningTime)
		{
			iter = mMemory.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	for (auto& m : mMemory)
	{
		(mCalculator)(m);
	}

	mMemory.sort( ([](MemoryRecord& a, MemoryRecord& b)
	{
		return a.importance > b.importance;
	}) );
}
