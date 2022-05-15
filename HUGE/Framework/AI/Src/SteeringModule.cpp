 #include "Precompiled.h"
#include "SteeringModule.h"

using namespace H::AI;

H::AI::SteeringModule::SteeringModule(Agent& agent)
	:mAgent(agent)
{}

H::Math::Vector2 H::AI::SteeringModule::Calculate()
{
	H::Math::Vector2 total;
	for (auto& [name,behavior] : mBehaviors)
	{
		if (behavior->IsActive())
		{
			total += behavior->Calculate(mAgent);
		}
	}

	return total;
}
