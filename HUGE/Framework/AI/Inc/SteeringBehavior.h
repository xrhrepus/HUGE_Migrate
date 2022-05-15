#ifndef HUGE_AI_STEERINGBEHAVIOR_H	
#define HUGE_AI_STEERINGBEHAVIOR_H
#include "Common.h"

namespace H::AI
{
	class Agent;

	class SteeringBehavior
	{
	public:
		SteeringBehavior() = default;
		virtual ~SteeringBehavior() = default;

		virtual H::Math::Vector2 Calculate(Agent& agent) = 0;
		//virtual const char* GetName() const = 0;

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive;	}

	private:
		bool mActive = false;
	};

 

}//namespace H::AI


#endif // !HUGE_AI_STEERINGBEHAVIOR_H

