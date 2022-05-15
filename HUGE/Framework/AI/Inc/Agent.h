#ifndef HUGE_AI_AGENT_H
#define HUGE_AI_AGENT_H

//#include "Common.h"

namespace H::AI
{
	class Agent;
	class AIWorld;
	using Agents = std::vector<Agent*>;

	class Agent
	{
	public:
		//H::Math::Matrix3 LocalToWorld()
		//{
		//	H::Math::Vector2 y = heading;
		//	H::Math::Vector2 x = { heading.y, -heading.x };
		//	H::Math::Vector2 p = position;
		//	return
		//	{
		//		x.x, x.y,0.0f,
		//		y.x, y.y,0.0f,
		//		p.x, p.y,1.0f,
		//	};
		//}


		//NOTE: 190831
		//restrict the order
		//aiworld -> agent
		Agent(AIWorld& world);
		//Agent();
		~Agent();

		H::Math::Matrix4 LocalToWorld() const;
		void GetNeighborFromWorld();

		AIWorld& world;
		Agents neighbors;
		int mTag;


		H::Math::Vector2 position;
		H::Math::Vector2 velocity;
		H::Math::Vector2 destination;
		H::Math::Vector2 heading{0.0f,1.0f};
		float radius = 32.0f;
		float maxSpeed = 1.0f;
		float mass = 1.0f;

		//sensor
		float viewRadius;
		float viewAngle;
		float cosViewAngle;
	};
}


 

#endif // !HUGE_AI_AGENT_H

