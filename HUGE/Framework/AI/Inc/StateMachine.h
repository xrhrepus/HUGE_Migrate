#ifndef HUGE_AI_STATEMACHINE_H
#define HUGE_AI_STATEMACHINE_H
#include "State.h"

namespace H::AI
{
	template<class AgentType>
	class StateMachine
	{
	public:
		using StateType = State<AgentType>;

		StateMachine(AgentType& agent);
//		~StateMachine();

		template<class NewStateType>
		void AddState();

		//void Purge();

		void UpDate(float deltatime);

		void ChangeState(std::string stateName);

	private:
		using StateMap = std::unordered_map<std::string, std::unique_ptr<StateType>>;

		AgentType& mAgent;
		StateType* mCurrentState = nullptr;
		StateMap mStates;


	};

	template<class AgentType>
	inline StateMachine<AgentType>::StateMachine(AgentType & agent)
		:mAgent(agent)
	{
	}


	template<class AgentType>
	template<class NewStateType>
	void StateMachine<AgentType>::AddState()
	{
		//NOTE 190810 : static_assert : compile time
		static_assert(std::is_base_of<StateType, NewStateType>::value,
			"[StateMachine] can only add state for agenttype");

		auto[state, success] = mStates.emplace(NewStateType::GetName(), std::make_unique<NewStateType>());
		if (!mCurrentState)
		{
			mCurrentState = state->second.get();
		}
	}

	template<class AgentType>
	void StateMachine<AgentType>::UpDate(float deltatime)
	{
		mCurrentState->Update(mAgent, deltatime);
	}

	template<class AgentType>
	void StateMachine<AgentType>::ChangeState(std::string stateName)
	{
		auto iter = mStates.find(stateName);
		if (iter == mStates.end())
			return;

		mCurrentState->Exit(mAgent);
		mCurrentState = iter->second.get();
		mCurrentState->Enter(mAgent);
	}
}


#endif // !HUGE_AI_STATEMACHINE_H

