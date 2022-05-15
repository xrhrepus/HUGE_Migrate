#include "Precompiled.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace H;
using namespace H::AI;
//
H::AI::Agent::Agent(AIWorld & _world)
	:world(_world)
{
	world.ResigterAgent(this);
}
//
//H::AI::Agent::Agent()
//{
//
//}
H::AI::Agent::~Agent()
{
	world.UnResigterAgent(this);
}
H::Math::Matrix4 Agent::LocalToWorld() const
{
	H::Math::Vector3 l = { 0.0f,0.0f, 1.0f };
	H::Math::Vector3 u = heading;
	H::Math::Vector3 r = H::Math::Cross(u, l);
	H::Math::Vector3 p = position;

	return
	{
		r.x,r.y,r.z,0.0f,
		u.x,u.y,u.z,0.0f,
		l.x,l.y,l.z,0.0f,
		p.x,p.y,p.z,1.0f
	};


}
void Agent::GetNeighborFromWorld()
{
	neighbors = world.GetNeighborhood({position,radius});
}
