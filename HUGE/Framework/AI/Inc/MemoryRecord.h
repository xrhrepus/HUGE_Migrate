#ifndef HUGE_AI_MEMORYRECORD_H
#define HUGE_AI_MEMORYRECORD_H


namespace H::AI
{
	using Property = std::variant<int, float, H::Math::Vector2>;

	struct MemoryRecord
	{
		//H::Math::Vector2 lastSeenPosition;
		std::unordered_map<std::string, Property> properties;
		float lastRecordedTime = 0.0f;
		float importance = 0.0f;
 
	};
	
	using MemoryRecords = std::list<MemoryRecord>;

}

#endif // !HUGE_AI_MEMORYRECORD_H

