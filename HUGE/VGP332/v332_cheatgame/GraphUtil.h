#ifndef HUGE_AI_GRAPHUTIL_H
#define HUGE_AI_GRAPHUTIL_H
#include <HUGE\Inc\HUGE.h>

//=============
using namespace H::AI;
using namespace H::AI::GraphSearch;
using namespace H::Input;


struct MapNode : public H::AI::Node
{
	H::Math::Vector3 position;
	int terrainType;
};
using NavGraph = H::AI::Graph<MapNode>;

//#pragma region graph_info
////======= graph info =======
//std::vector<size_t> mWall_In_TileMap;
////node size
//const float NODE_RADIUS = 16.0f;
//const float NODE_SPACING = NODE_RADIUS * 2;
//
////graph size
//const int GRAPH_MAX_ROW = 20;//max size for 32x32 tile screen can hold
//const int GRAPH_MAX_COLUMN = 40;//max size for 32x32 tile screen can hold
//const size_t GRAPH_MAX_SIZE = GRAPH_MAX_ROW * GRAPH_MAX_COLUMN;
//
//int GRAPH_row = 0;//actual tilemap size
//int GRAPH_column = 0;//actual tilemap size
//int GRAPH_size = 0;
//
////tilemap infos
//H::Graphics::TextureId wallId = 2;
//
////graph top left position
//const Vector2 GRAPH_INIT_POS = { NODE_RADIUS,NODE_RADIUS };
//
//
////colors ==
//const Color NODE_COLOR = Colors::DeepSkyBlue;
//const Color NODE_CONNECT_COLOR = Colors::RoyalBlue;
//
//const Color NODE_START_COLOR = Colors::SpringGreen;
//const Color NODE_END_COLOR = Colors::LightYellow;
//
//const Color NODE_PATH_COLOR = Colors::NavajoWhite;
//
//const Color NODE_TRIED_PATH_COLOR = Colors::Violet;
//const Color NODE_TRIED_CONNECT_COLOR = Colors::DeepPink;
////==============
//
//#pragma endregion
//namespace GraphUtil
//{
//
//#pragma region Graph Functions
//
//	bool IsWall(size_t n);
//
//
//	bool IsWall_Using_TileMap(size_t n);
//
//	struct CanOpenNode;
// 
//	struct CanOpenNode_Astar;
//
//
//	struct GetGCost;
//
//	template< class HCostPolicy>
//	struct GetHCost
//	{
//		float operator()(Context& context, size_t nodeIndex)
//		{
//
//			size_t fromCol = nodeIndex % GRAPH_column;
//			size_t fromRow = nodeIndex / GRAPH_column;
//
//			size_t toCol = context.end % GRAPH_column;
//			size_t toRow = context.end / GRAPH_column;
//
//			//float dist = sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
//			float dist = hcostpolicy(fromCol, fromRow, toCol, toRow);
//
//			return /*context.g[parentIndex] +*/ dist;
//
//		}
//		HCostPolicy hcostpolicy;
//	};
//	//A*
//	struct Heuristic // algorithm to compute h
//	{
//		float operator()(size_t fromCol, size_t fromRow, size_t toCol, size_t toRow)
//		{
//			return sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
//		}
//
//	};
//
//
//
//	void BuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position);
//
//	void ReadInfoFromTileMap(const TileMap& tilemap);
//
//	void UpdateInputForGraph(int& startX, int& startY, int& endX, int& endY, int tileSize);
//
//
//	void ShowPath(GraphSearch::Context& context, NavGraph& graph);
//
//	void ShowGridGraph(GraphSearch::Context& context, NavGraph& graph, bool showConnection);
//
//
//	void ShowStartEndPoint(int startX, int startY, int endX, int endY, int tileSize);
//
//	void RebuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position, const TileMap& tilemap);
//#pragma endregion
//}



#endif // !HUGE_AI_GRAPHUTIL_H
