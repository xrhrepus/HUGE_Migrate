#ifndef HUGE_AI_GRAPH_H
#define HUGE_AI_GRAPH_H
#include "Common.h"
//#include <Graphics\Inc\Colors.h>
#include <Graphics\Inc\Graphics.h>
using namespace H::Graphics;

namespace H::AI {

	////======= graph info =======

	////node size
	//const float NODE_RADIUS = 16.0f;
	//const float NODE_SPACING = NODE_RADIUS * 2;

	////graph size
	//const int GRAPH_MAX_ROW = 20;//max size for 32x32 tile screen can hold
	//const int GRAPH_MAX_COLUMN = 40;//max size for 32x32 tile screen can hold
	//const size_t GRAPH_MAX_SIZE = GRAPH_MAX_ROW * GRAPH_MAX_COLUMN;

	//static int GRAPH_row = 0;//actual tilemap size
	//static int GRAPH_column = 0;//actual tilemap size
	//static int GRAPH_size = 0;

	////tilemap infos
	//static H::Graphics::TextureId wallId = 2;

	////graph top left position
	//const Vector2 GRAPH_INIT_POS = { NODE_RADIUS,NODE_RADIUS };


	////colors ==
	//const Color NODE_COLOR = Colors::DeepSkyBlue;
	//const Color NODE_CONNECT_COLOR = Colors::RoyalBlue;

	//const Color NODE_START_COLOR = Colors::SpringGreen;
	//const Color NODE_END_COLOR = Colors::LightYellow;

	//const Color NODE_PATH_COLOR = Colors::NavajoWhite;

	//const Color NODE_TRIED_PATH_COLOR = Colors::Violet;
	//const Color NODE_TRIED_CONNECT_COLOR = Colors::DeepPink;


	////==============



	constexpr size_t InvalidNode = std::numeric_limits<size_t>::max();

	struct Node //base ,to be inherited from
	{
		std::vector<size_t> neighbours;

	};

	template <class NodeType>
	class Graph
	{
	public:

		void Initialize(size_t nodeCount);

		NodeType& GetNode(size_t index);

		const NodeType& GetNode(size_t index) const;
		const size_t GetNodeCount() const;

		//add node
		void AddNode(const NodeType& node);
		//add neighbour
		void AddNeighbourhood(size_t a, size_t b);

	private:
		std::vector<NodeType> mNodes;
	};



	template<class NodeType>
	inline void Graph<NodeType>::Initialize(size_t nodeCount)
	{
		mNodes.resize(nodeCount);
	}

	template<class NodeType>
	inline NodeType & Graph<NodeType>::GetNode(size_t index)
	{
		return mNodes[index];
	}

	template<class NodeType>
	inline const NodeType & Graph<NodeType>::GetNode(size_t index) const
	{
		return mNodes[index];
	}

	template<class NodeType>
	inline const size_t Graph<NodeType>::GetNodeCount() const
	{
		return mNodes.size();
	}

	template<class NodeType>
	inline void Graph<NodeType>::AddNode(const NodeType& node)
	{
		mNodes.push_back(node);
	}

	template<class NodeType>
	inline void Graph<NodeType>::AddNeighbourhood(size_t selfIndex, size_t neighbourIndex)
	{
		mNodes[selfIndex].neighbours.push_back(neighbourIndex);
	}


}//namespace AI


#endif // !HUGE_AI_GRAPH_H
