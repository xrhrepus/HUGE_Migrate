#ifndef HUGE_AI_STATE_H
#define HUGE_AI_STATE_H

#include "Common.h"
namespace H::AI
{
	template<class AgentType>
	class State
	{
	public:
		
		virtual ~State() = default;

		virtual void Enter(AgentType& agent) = 0;
		virtual void Update(AgentType& agent, float deltatime)  = 0;
		virtual void Exit(AgentType& agent)  = 0;

		//std::reference_wrapper<AgentType>

	};


	
}//namespace H::AI


#endif // !HUGE_AI_STATE_H
