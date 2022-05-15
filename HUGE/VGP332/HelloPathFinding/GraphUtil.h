#ifndef HUGE_AI_GRAPHUTIL_H
#define HUGE_AI_GRAPHUTIL_H

//=============
using namespace H::AI;
struct MapNode : public H::AI::Node
{
	H::Math::Vector3 position;
	int terrainType;
};
using NavGraph = H::AI::Graph<MapNode>;

#endif // !HUGE_AI_GRAPHUTIL_H
