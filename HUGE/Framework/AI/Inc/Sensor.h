#ifndef HUGE_AI_SENSOR_H
#define HUGE_AI_SENSOR_H

#include "MemoryRecord.h"
//class MemoryRecords;

namespace H::AI
{
	class Agent;

	class Sensor
	{
	public:
		//Sensor();
		virtual ~Sensor() = default;
		virtual void Update(Agent& agent, MemoryRecords& memory, float deltatime) = 0;


	};

}//namespace H::AI

 

#endif // !HUGE_AI_SENSOR_H
