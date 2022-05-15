#ifndef HUGE_AI_STEERINGMODULE_H
#define HUGE_AI_STEERINGMODULE_H
#include "SteeringBehavior.h"

namespace H::AI
{
	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);
		//~SteeringModule();

		template<class BehaviorType>
		BehaviorType* AddBehavior(std::string name);

		template<class BehaviorType>
		BehaviorType* GetBehavior(std::string name);

		H::Math::Vector2 Calculate();

	private:
		using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<H::AI::SteeringBehavior>>;

		Agent& mAgent;
		BehaviorMap mBehaviors;
	};

	template<class BehaviorType>
	inline BehaviorType * SteeringModule::AddBehavior(std::string name)
	{
		auto[iter, success] = mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
		return success ? static_cast<BehaviorType*>( iter->second.get()) : nullptr;
	}

	template<class BehaviorType>
	inline BehaviorType * SteeringModule::GetBehavior(std::string name)
	{
		auto iter = mBehaviors.find(name);
		return iter != mBehaviors.end() ? static_cast<BehaviorType*>(iter->second.get()) : nullptr;
	}

}

 


#endif // !HUGE_AI_STEERINGMODULE_H
