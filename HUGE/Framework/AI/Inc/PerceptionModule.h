#ifndef HUGE_AI_PERCEPTIONMODULE_H
#define HUGE_AI_PERCEPTIONMODULE_H
#include "Sensor.h"
#include "MemoryRecord.h"

namespace H::AI
{
	class Agent;

	using  ImportanceCalculator = std::function<void(MemoryRecord&)>;
	

	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent , ImportanceCalculator calculator);
		//~PerceptionModule();
		
		template<class SensorType>
		SensorType* AddSensor(std::string name);

		template<class SensorType>
		SensorType* GetSensor(const std::string& name);

		void Update(float deltatime);

		const MemoryRecords& GetMemoryRecords() const { return mMemory; }
		float mMemorySpan = 2.0f;

	private:
		using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

		Agent& mAgent;
		SensorMap mSensors;
		ImportanceCalculator mCalculator;
		MemoryRecords mMemory;

	};

	template<class SensorType>
	inline SensorType * PerceptionModule::AddSensor(std::string name)
	{
		auto[iter, success] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
		return success ? static_cast<SensorType*>(iter->second.get()) : nullptr;
	}

	template<class SensorType>
	inline SensorType * PerceptionModule::GetSensor(const std::string& name)
	{
		auto iter = mSensors.find(name);
		return iter != mSensors.end() ? static_cast<SensorType*>(iter->second.get()) : nullptr;
	}

}//namespace H::AI


#endif // !HUGE_AI_PERCEPTIONMODULE_H
