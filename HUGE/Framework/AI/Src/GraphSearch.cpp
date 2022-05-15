#include "Precompiled.h"
#include "GraphSearch.h"
using namespace H::AI::GraphSearch;

//
//bool IsWall(size_t n)
//{
//	int col = n % GRAPH_MAX_COLUMN;
//	int row = n / GRAPH_MAX_COLUMN;
//	return (col > 1 && col < 10 && col == row) ||
//		(col > 1 && col < 10 && col - 1 == row)
//		;//.....
//}
//
//bool IsWall_Using_TileMap(size_t n)
//{
//	return std::find(mWall_In_TileMap.begin(), mWall_In_TileMap.end(), n) != mWall_In_TileMap.end();
//
//}
