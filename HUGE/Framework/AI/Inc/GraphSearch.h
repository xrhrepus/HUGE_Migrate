#ifndef HUGE_AI_GRAPHSEARCH_H
#define HUGE_AI_GRAPHSEARCH_H
#include "Graph.h"

namespace H::AI::GraphSearch
{
	


	struct Context
	{
		void Init(size_t nodes) { nodeCount = nodes; }

		size_t nodeCount;
		std::list<size_t> openList;
		std::list<size_t> closeList;
		std::vector<size_t> parent;
		std::vector<bool> open;
		std::vector<bool> close;
		std::vector<float> g;//190727 dijkstra
		std::vector<float> h;//190803 A*
		size_t start = InvalidNode;
		size_t end = InvalidNode;
		bool found = false;

		void Reset()
		{
			openList.clear();
			closeList.clear();
			parent.clear();
			g.clear();
			h.clear();
			open.clear();
			close.clear();

			parent.resize(nodeCount, InvalidNode);
			g.resize(nodeCount, std::numeric_limits<float>::max());// 0 or max ? 
			h.resize(nodeCount, 0.0f);
			open.resize(nodeCount, false);
			close.resize(nodeCount, false);

			found = false;
		}

	};

	//NOTE 190720 :policy based programming
	template<class GraphType, class OpenPolicy, class NextPolicy, class ValidationPolicy>
	struct BaseSearch
	{
		BaseSearch(const GraphType& graph)
			:graph(graph)
		{}

		void Execute(Context& context)
		{
			if (!context.open[context.start])
			{
				Open(context, context.start, InvalidNode);
				//update g cost

			}

			while (!context.found && !context.openList.empty())
			{
				size_t nodeIndex = Next(context);
				//context.openList.pop_front();

				if (nodeIndex == context.end)
				{
					context.found = true;
				}
				else
				{
					auto& node = graph.GetNode(nodeIndex);
					for (auto& n : node.neighbours)
					{
						if (IsValid(context,n))
						{
							Open(context,n, nodeIndex);
						}

					}
				}
				context.closeList.push_back(nodeIndex);
				context.close[nodeIndex] = true;
			}
		}

		const GraphType& graph;
		OpenPolicy Open;
		NextPolicy Next;
		ValidationPolicy IsValid;
	};

	struct PushBackPolicy
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			context.openList.push_back(nodeIndex);
			context.open[nodeIndex] = true;
			context.parent[nodeIndex] = parentIndex;
		}
	};

	struct PopFrontPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.front();
			context.openList.pop_front();
			return next;
		}
	};

	struct PopBackPolicy
	{
		size_t operator()(Context& context)
		{
			auto next = context.openList.back();
			context.openList.pop_back();
			return next;
		}

	};
	//bool IsWall(size_t n);
	//bool IsWall_Using_TileMap(size_t n);
	//
	//bool IsWall(size_t n)
	//{
	//	int col = n % GRAPH_MAX_COLUMN;
	//	int row = n / GRAPH_MAX_COLUMN;
	//	return (col > 1 && col < 10 && col == row) || 
	//		(col > 1 && col < 10 && col - 1 == row)
	//		;//.....
	//}
	//static bool IsWall_Using_TileMap(size_t n)
	//{
	//	return std::find(mWall_In_TileMap.begin(), mWall_In_TileMap.end(), n) != mWall_In_TileMap.end();
 //
	//}

	//struct CanOpenNode
	//{
	//	bool operator()(Context& context, size_t n)
	//	{
	//		if (context.open[n])
	//		{
	//			return false;
	//		}
	//		if (IsWall_Using_TileMap(n))
	//		{
	//			return false;
	//		}
	//		return true;
	//	}
	//};

	//struct CanOpenNode_Astar
	//{
	//	bool operator()(Context& context, size_t n)
	//	{
	//		if (context.close[n])
	//		{
	//			return false;
	//		}
	//		if (IsWall_Using_TileMap(n))
	//		{
	//			return false;
	//		}
	//		return true;
	//	}
	//};


	//struct GetGCost
	//{
	//	float operator()(Context& context, size_t nodeIndex, size_t parentIndex)
	//	{

	//		if (parentIndex == InvalidNode)
	//		{
	//			return 0.0f;
	//		}

	//		size_t fromCol = parentIndex % GRAPH_column;
	//		size_t fromRow = parentIndex / GRAPH_column;

	//		size_t toCol = nodeIndex % GRAPH_column;
	//		size_t toRow = nodeIndex / GRAPH_column;

	//		return context.g[parentIndex] + ((fromCol != toCol && fromRow != toRow) ? 1.414f : 1.0f);

	//	}
	//};
	////190803 todo
	//template< class HCostPolicy>
	//struct GetHCost
	//{
	//	float operator()(Context& context, size_t nodeIndex)
	//	{

	//		size_t fromCol = nodeIndex % GRAPH_column;
	//		size_t fromRow = nodeIndex / GRAPH_column;

	//		size_t toCol = context.end % GRAPH_column;
	//		size_t toRow = context.end / GRAPH_column;

	//		//float dist = sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
	//		float dist = hcostpolicy(fromCol, fromRow, toCol, toRow);

	//		return /*context.g[parentIndex] +*/ dist;

	//	}
	//	HCostPolicy hcostpolicy;
	//};
	////A*
	//struct Heuristic // algorithm to compute h
	//{
	//	float operator()(size_t fromCol, size_t fromRow, size_t toCol, size_t toRow)
	//	{
	//		return sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
	//	}

	//};


	struct PopBackPolicy_Dijkstra
	{
		size_t operator()(Context& context)
		{
			//auto minIt = std::min_element( context.g.begin(), context.g.end());// with min cost iterator 
			//size_t next = std::distance(context.g.begin(), minIt);// node index with least cost in context.g

			//if (auto oit = std::find(context.openList.begin(), context.openList.end(), next);//should 100% exist
			//	oit != context.openList.end() )//find it in openlist
			//{
			//	context.openList.erase(oit);
			//	return next;

			//}

			//size_t next = InvalidNode;

			//float minCost = std::numeric_limits<float>::max();
			//for (auto o : context.openList)//for each node in openlist
			//{
			//	if ( context.g[o] < minCost)//find the [node index] with min [gcost] according to [context.g]
			//	{
			//		minCost = context.g[o];
			//		next = o;
			//	}
			//}
			//context.openList.remove(next);

			//return next;
			size_t next = context.openList.front();
			context.openList.pop_front();
			return next;
		}

	};

	struct PopBackPolicy_Astar
	{
		size_t operator()(Context& context)
		{
			//size_t next = InvalidNode;

			//float minCost = std::numeric_limits<float>::max();
			//for (auto o : context.openList)//for each node in openlist
			//{

			//	if (float cst = context.g[o] + context.h[o];
			//		cst < minCost)//find the [node index] with min [gcost] according to [context.g]
			//	{
			//		minCost = cst;
			//		next = o;
			//	}
			//}
			//context.openList.remove(next);

			//return next;

			size_t next = context.openList.front();
			context.openList.pop_front();
			return next;

		}

	};

	template<class CostPolicy>
	struct InsertSortedPolicy_Dijkstra
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = Cost(context, nodeIndex, parentIndex);
			//float h =
			//TODO 190727: sort back insertion
			//update g cost
			//-----------------------------------------------------
			if (g < context.g[nodeIndex])
			{
				context.g[nodeIndex] = g;
				context.open[nodeIndex] = true;
				context.parent[nodeIndex] = parentIndex;

				bool don = false;
				for (auto it = context.openList.begin(); it != context.openList.end(); ++it)
				{
					if (context.g[nodeIndex] < context.g[*it]) // less than , push in front of IT
					{
						context.openList.insert(it, nodeIndex);
						don = true;
						break;
					}

				}
				if (!don)
				{
					context.openList.push_back(nodeIndex);
				}
				
				//context.openList.push_back(nodeIndex);// order?
			}
			//context.openList.push_back(nodeIndex);
		}
		CostPolicy Cost;
	};




	template<class GCostPolicy, class HCostPolicy>
	struct InsertSortedPolicy_AStar
	{
		void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
		{
			float g = gCost(context, nodeIndex, parentIndex);

			//float h = hCost(context, nodeIndex, parentIndex);
			//if (parentIndex != InvalidNode)
			//	context.h[parentIndex] = h;
			float h = 0.0f;
				//if (context.h[nodeIndex] == 0.0f)
				//{
				//	h = hCost(context, nodeIndex);
				//	context.h[nodeIndex] = h;
				//}
			//update g cost
			//-----------------------------------------------------
				if (!context.open[nodeIndex])
				{
					context.g[nodeIndex] = g;
					context.open[nodeIndex] = true;
					context.parent[nodeIndex] = parentIndex;

					h = hCost(context, nodeIndex);
					context.h[nodeIndex] = h;

					bool don = false;
					for (auto it = context.openList.begin(); it != context.openList.end(); ++it)
					{
						if (context.g[nodeIndex] + context.h[nodeIndex] < context.g[*it] + context.h[*it]) // less than , push in front of IT
						{
							context.openList.insert(it, nodeIndex);
							don = true;
							break;
						}

					}
					if (!don)
					{
						context.openList.push_back(nodeIndex);
					}

				}
				else if( g < context.g[nodeIndex]) // if opened and has a cheaper g cost
				{
					context.g[nodeIndex] = g; // update g
					//context.open[nodeIndex] = true;
					context.parent[nodeIndex] = parentIndex; // update parent

					auto currIter = std::find(context.openList.begin(), context.openList.end(), nodeIndex);//find the current iter, to correct the order in ol


					bool don = false;
					for (auto it = context.openList.begin(); it != context.openList.end(); ++it)
					{
						if (context.g[nodeIndex] + context.h[nodeIndex] < context.g[*it] + context.h[*it]) // less than , push in front of IT
						{
							context.openList.insert(it, nodeIndex);
							don = true;
							break;
						}

					}
					if (!don)
					{
						context.openList.push_back(nodeIndex);
					}

					context.openList.erase(currIter);//erase the current one

				}


			//if (g + h < context.g[nodeIndex] + context.h[nodeIndex])// g + h
			//{
			//	context.g[nodeIndex] = g;
			//	context.open[nodeIndex] = true;
			//	context.parent[nodeIndex] = parentIndex;

			//	bool don = false;
			//	for (auto it = context.openList.begin(); it != context.openList.end(); ++it)
			//	{
			//		if (context.g[nodeIndex] + context.h[nodeIndex] < context.g[*it] + context.h[*it]) // less than , push in front of IT
			//		{
			//			context.openList.insert(it, nodeIndex);
			//			don = true;
			//			break;
			//		}

			//	}
			//	if (!don)
			//	{
			//		context.openList.push_back(nodeIndex);
			//	}

			//}
		}
		GCostPolicy gCost;
		HCostPolicy hCost;

	};



	//
	template <class GraphType, class ValidationPolicy>
	using BFS = BaseSearch<GraphType, PushBackPolicy, PopFrontPolicy, ValidationPolicy>;
	template <class GraphType, class ValidationPolicy>
	using DFS = BaseSearch<GraphType, PushBackPolicy, PopBackPolicy, ValidationPolicy>;
	//
	template <class GraphType, class ValidationPolicy,class CostPolicy>
	using Dijkstra = BaseSearch<GraphType, InsertSortedPolicy_Dijkstra<CostPolicy>, PopBackPolicy_Dijkstra, ValidationPolicy>;

	template <class GraphType, class ValidationPolicy, class GCostPolicy, class HCostPolicy>
	using Astar = BaseSearch<GraphType, InsertSortedPolicy_AStar<GCostPolicy, HCostPolicy>, PopBackPolicy_Astar, ValidationPolicy>;

}//namespace H AI GRAPHSEARCH



#endif // !HUGE_AI_GRAPHSEARCH_H

