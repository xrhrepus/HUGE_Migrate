#include "SteerBehaviors.h"

H::Math::Vector2 Seek::Calculate(H::AI::Agent& agent)
{
	auto desierdV = H::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed * (*force);
	return (desierdV - agent.velocity * turn)  ;

}