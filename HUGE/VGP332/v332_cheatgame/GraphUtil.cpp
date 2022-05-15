#include "GraphUtil.h"
 
struct GraphUtil::CanOpenNode
{
	bool operator()(Context& context, size_t n)
	{
		if (context.open[n])
		{
			return false;
		}
		if (IsWall_Using_TileMap(n))
		{
			return false;
		}
		return true;
	}
};

struct GraphUtil::CanOpenNode_Astar
{
	bool operator()(Context& context, size_t n)
	{
		if (context.close[n])
		{
			return false;
		}
		if (IsWall_Using_TileMap(n))
		{
			return false;
		}
		return true;
	}
};
struct GraphUtil::GetGCost
{
	float operator()(Context& context, size_t nodeIndex, size_t parentIndex)
	{

		if (parentIndex == InvalidNode)
		{
			return 0.0f;
		}

		size_t fromCol = parentIndex % GRAPH_column;
		size_t fromRow = parentIndex / GRAPH_column;

		size_t toCol = nodeIndex % GRAPH_column;
		size_t toRow = nodeIndex / GRAPH_column;

		return context.g[parentIndex] + ((fromCol != toCol && fromRow != toRow) ? 1.414f : 1.0f);

	}
};

bool GraphUtil::IsWall(size_t n)
{
	int col = n % GRAPH_MAX_COLUMN;
	int row = n / GRAPH_MAX_COLUMN;
	return (col > 1 && col < 10 && col == row) ||
		(col > 1 && col < 10 && col - 1 == row)
		;//.....
}
bool GraphUtil::IsWall_Using_TileMap(size_t n)
{
	return std::find(mWall_In_TileMap.begin(), mWall_In_TileMap.end(), n) != mWall_In_TileMap.end();

}

void GraphUtil::BuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position)//todo
{
	graph.Initialize(row * column);
	//const float size_div_row = (float)GRAPH_SIZE / (float)GRAPH_MAX_ROW;

	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < column; j++)
		{
			MapNode& mapNode = graph.GetNode(j + i * column);

			mapNode.position = {
					position.x + spacing * j,
					position.y + spacing * i,0.0f };
		}
	}
	//add neighbour
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			MapNode& mapNode = graph.GetNode(j + i * column);



			if (j - 1 >= 0)//left
			{
				graph.AddNeighbourhood((j + i * column) - 1, (j + i * column));
			}
			if (j - 1 >= 0 && i - 1 >= 0)//lt
			{
				graph.AddNeighbourhood((j + (i - 1) * column) - 1, (j + (i)* column));
			}

			if (i - 1 >= 0)//top
			{
				graph.AddNeighbourhood((j + (i - 1) * column), (j + (i)* column));
			}
			if (j + 1 < column && i - 1 >= 0)//rt
			{
				graph.AddNeighbourhood((j + (i - 1) * column) + 1, (j + (i)* column));
			}

			if (j + 1 < column)//right
			{
				graph.AddNeighbourhood((j + i * column) + 1, (j + i * column));
			}
			if (j + 1 < column && i + 1 < row)//rb
			{
				graph.AddNeighbourhood((j + (i + 1) * column) + 1, (j + (i)* column));
			}

			if (i + 1 < row)//bot
			{
				graph.AddNeighbourhood((j + (i + 1) * column), (j + (i)* column));
			}
			if (j - 1 >= 0 && i + 1 < row)//lb
			{
				graph.AddNeighbourhood((j + (i + 1) * column) - 1, (j + (i)* column));
			}


		}
	}

}

void GraphUtil::ReadInfoFromTileMap(const TileMap& tilemap)
{
	GRAPH_row = tilemap.GetMapRow();
	GRAPH_column = tilemap.GetMapColumn();
	GRAPH_size = GRAPH_row * GRAPH_column;
	std::vector<size_t> walls;

	size_t wallIndex = 0;
	for (auto t : tilemap.GetTileMap())
	{
		if (t == wallId)//
		{
			walls.push_back(wallIndex);
		}
		wallIndex++;
	}
	mWall_In_TileMap = std::move(walls);

	//tilemap.GetTileMapInfo(GraphSearch::mWall_In_TileMap, GRAPH_row, GRAPH_column);
}

void GraphUtil::UpdateInputForGraph(int& startX, int& startY, int& endX, int& endY, int tileSize)
{

	if (InputSystem::Get()->IsMousePressed(MouseButton::LBUTTON))
	{
		int sx = static_cast<int>(InputSystem::Get()->GetMouseScreenX() / tileSize);
		int sy = static_cast<int>(InputSystem::Get()->GetMouseScreenY() / tileSize);
		startX = sx > GRAPH_column - 1 ? GRAPH_column - 1 : sx;
		startY = sy > GRAPH_row - 1 ? GRAPH_row - 1 : sy;
	}
	if (InputSystem::Get()->IsMousePressed(MouseButton::RBUTTON))
	{
		int sx = static_cast<int>(InputSystem::Get()->GetMouseScreenX() / tileSize);
		int sy = static_cast<int>(InputSystem::Get()->GetMouseScreenY() / tileSize);

		endX = sx > GRAPH_column - 1 ? GRAPH_column - 1 : sx;
		endY = sy > GRAPH_row - 1 ? GRAPH_row - 1 : sy;
	}

}

void GraphUtil::ShowPath(GraphSearch::Context& context, NavGraph& graph)
{
	if (context.nodeCount == graph.GetNodeCount())
	{
		std::list<size_t> path;
		auto next = context.closeList.back();
		while (next != InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}


		for (int i = 0; i < graph.GetNodeCount(); i++)
		{
			const MapNode& cNode = graph.GetNode(i);

			if (context.close[i])//tried
			{
				//SimpleDraw::AddScreenCircle_For_AI_Graph(
				//	GraphUtil_circle_verts,
				//	{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
				//	NODE_TRIED_PATH_COLOR,1);

				SimpleDraw::AddScreenRect(
					{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
					NODE_TRIED_PATH_COLOR);

				//draw connections for tried node
				auto cNode_Parent = context.parent[i];
				if (cNode_Parent != InvalidNode)
				{
					SimpleDraw::AddScreenLine(
						{ cNode.position.x,cNode.position.y },
						{ graph.GetNode(cNode_Parent).position.x,graph.GetNode(cNode_Parent).position.y },
						NODE_TRIED_CONNECT_COLOR
					);
				}
			}
		}
		//draw path
		for (auto i : path)
		{
			const MapNode& cNode = graph.GetNode(i);
			SimpleDraw::AddScreenCircle(
				{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
				NODE_PATH_COLOR);
		}

	}
}

void GraphUtil::ShowGridGraph(GraphSearch::Context& context, NavGraph& graph, bool showConnection)
{


	//compute once
	//SimpleDraw::ComputeCircleVertex(GraphUtil_circle_verts, NODE_RADIUS,1);


	for (size_t i = 0; i < graph.GetNodeCount(); i++)//draw node
	{
		const MapNode& cNode = graph.GetNode(i);

		//SimpleDraw::AddScreenCircle(
		//	{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
		//	NODE_COLOR);
		//SimpleDraw::AddScreenCircle_For_AI_Graph(
		//	GraphUtil_circle_verts,
		//	{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
		//	NODE_COLOR,1);
		SimpleDraw::AddScreenRect(
			{ { cNode.position.x,cNode.position.y } ,NODE_RADIUS },
			NODE_COLOR);


		//draw all connections
		if (cNode.neighbours.size() > 0 && showConnection)
		{
			for (size_t j = 0; j < cNode.neighbours.size(); j++)
			{
				SimpleDraw::AddScreenLine(
					{ cNode.position.x,cNode.position.y },
					{ graph.GetNode(cNode.neighbours[j]).position.x,graph.GetNode(cNode.neighbours[j]).position.y },
					NODE_CONNECT_COLOR
				);
			}
		}
	}


}

void GraphUtil::ShowStartEndPoint(int startX, int startY, int endX, int endY, int tileSize)
{

	SimpleDraw::AddScreenRect(
		{ { static_cast<float>((startX + 0.5f) * tileSize) ,static_cast<float>((startY + 0.5f) * tileSize) } ,NODE_RADIUS },
		NODE_START_COLOR);

	SimpleDraw::AddScreenRect(
		{ { static_cast<float>((endX + 0.5f) * tileSize) ,static_cast<float>((endY + 0.5f) * tileSize) } ,NODE_RADIUS },
		NODE_END_COLOR);

}

void GraphUtil::RebuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position, const TileMap& tilemap)
{
	ReadInfoFromTileMap(tilemap);
	BuildGraph(graph, row, column, spacing, position);
}
