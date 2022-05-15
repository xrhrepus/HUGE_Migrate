//#include <Core/Inc/Core.h>
#include <HUGE\Inc\HUGE.h>
#include "GraphUtil.h"

using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
using namespace H::AI;
using namespace H::AI::GraphSearch;

#pragma region graph_info
//======= graph info =======
std::vector<size_t> mWall_In_TileMap;
//node size
const float NODE_RADIUS = 16.0f;
const float NODE_SPACING = NODE_RADIUS * 2;

//graph size
const int GRAPH_MAX_ROW = 20;//max size for 32x32 tile screen can hold
const int GRAPH_MAX_COLUMN = 40;//max size for 32x32 tile screen can hold
const size_t GRAPH_MAX_SIZE = GRAPH_MAX_ROW * GRAPH_MAX_COLUMN;

int GRAPH_row = 0;//actual tilemap size
int GRAPH_column = 0;//actual tilemap size
int GRAPH_size = 0;

//tilemap infos
H::Graphics::TextureId wallId = 2;

//graph top left position
const Vector2 GRAPH_INIT_POS = { NODE_RADIUS,NODE_RADIUS };


//colors ==
const Color NODE_COLOR = Colors::DeepSkyBlue;
const Color NODE_CONNECT_COLOR = Colors::RoyalBlue;

const Color NODE_START_COLOR = Colors::SpringGreen;
const Color NODE_END_COLOR = Colors::LightYellow;

const Color NODE_PATH_COLOR = Colors::NavajoWhite;

const Color NODE_TRIED_PATH_COLOR = Colors::Violet;
const Color NODE_TRIED_CONNECT_COLOR = Colors::DeepPink;
//==============

#pragma endregion

#pragma region Graph Functions
bool IsWall(size_t n)
{
	int col = n % GRAPH_MAX_COLUMN;
	int row = n / GRAPH_MAX_COLUMN;
	return (col > 1 && col < 10 && col == row) ||
		(col > 1 && col < 10 && col - 1 == row)
		;//.....
}

	bool IsWall_Using_TileMap(size_t n)
	{
		return std::find(mWall_In_TileMap.begin(), mWall_In_TileMap.end(), n) != mWall_In_TileMap.end();
 
	}

	struct CanOpenNode
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

	struct CanOpenNode_Astar
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


	struct GetGCost
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
	//190803 todo
	template< class HCostPolicy>
	struct GetHCost
	{
		float operator()(Context& context, size_t nodeIndex)
		{

			size_t fromCol = nodeIndex % GRAPH_column;
			size_t fromRow = nodeIndex / GRAPH_column;

			size_t toCol = context.end % GRAPH_column;
			size_t toRow = context.end / GRAPH_column;

			//float dist = sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
			float dist = hcostpolicy(fromCol, fromRow, toCol, toRow);

			return /*context.g[parentIndex] +*/ dist;

		}
		HCostPolicy hcostpolicy;
	};
	//A*
	struct Heuristic // algorithm to compute h
	{
		float operator()(size_t fromCol, size_t fromRow, size_t toCol, size_t toRow)
		{
			return sqrtf((fromCol - toCol)*(fromCol - toCol) + (fromRow - toRow)*(fromRow - toRow));
		}

	};



void BuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position)//todo
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

void ReadInfoFromTileMap(const TileMap& tilemap)
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

void UpdateInputForGraph(int& startX, int& startY, int& endX, int& endY, int tileSize)
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


void ShowPath(GraphSearch::Context& context, NavGraph& graph)
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

void ShowGridGraph(GraphSearch::Context& context, NavGraph& graph, bool showConnection)
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

void ShowStartEndPoint(int startX, int startY, int endX, int endY, int tileSize)
{

	SimpleDraw::AddScreenRect(
		{ { static_cast<float>((startX + 0.5f) * tileSize) ,static_cast<float>((startY + 0.5f) * tileSize) } ,NODE_RADIUS },
		NODE_START_COLOR);

	SimpleDraw::AddScreenRect(
		{ { static_cast<float>((endX + 0.5f) * tileSize) ,static_cast<float>((endY + 0.5f) * tileSize) } ,NODE_RADIUS },
		NODE_END_COLOR);

}

void RebuildGraph(NavGraph& graph, int row, int column, float spacing, Vector2 position, const TileMap& tilemap)
{
	ReadInfoFromTileMap(tilemap);
	BuildGraph(graph, row, column, spacing, position);
}

#pragma endregion



//
Graph<MapNode> graph;
GraphSearch::Context context;
//GraphSearch::BFS<Graph<MapNode>> bfs(graph);
//

//variables =============
//search =============
int startX = 0, startY = 0;
int endX = 0, endY = 0;
float time_cost = 0.0f;
bool showPath = true;
bool showGraph = false;
bool showConnection = false;
bool pick_start = false;
int tilemap_brush_type = -1;

//
float g_cost = 0.0f;
//TextureId imgButton;
//=============
void ShowDebugUi(TileMap& tilemap, float deltatime);
//tilemap ==========================
//TileMap tileMap;
TextureId icon;
InputSystem* inputSys;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	//[set up app window] ==========================
	Window myWindow;
	myWindow.Initialize(hinstance, "Hell0 pathfinding", 1280, 720);

	//[inputsystem] ==========================
	InputSystem::StaticInitialize(myWindow.GetWindowHandle());
	inputSys = InputSystem::Get();


	//[graphic system] init  ==========================
	GraphicSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	//[simple draw] ==========================
	SimpleDraw::StaticInitialize(1000000);

	//[IMGui], last init first catch win msg ==========================
	DebugUI::Initialize(myWindow.GetWindowHandle(), true, false);

	//[Texture manager] ==========================
	TextureManager::StaticInitialize(L"../../Assets/Images/");
	//TextureManager::Get()->SetRootPath(L"../Assets/Images/");
	 //imgButton = TextureManager::Get()->LoadTexture(L"stone32.png");

	//[spriterenderer] ==========================
	SpriteRenderer::StaticInitialize();

	//[camera] ==========================
	Camera cam;
	cam.SetPosition({ 0.0f,0.0f, -10.0f });
	cam.SetDirection({ 0.0f,0.0f, 1.0f });
	


	//[tilemap] ==========================

	TileMap tileMap;
	tileMap.LoadTileMap(L"../../Assets/TileMap/maze32_20x40.txt", L"../../Assets/TileMap/tileset32x32.txt");
	//imgui icon
	icon = TextureManager::Get()->LoadTexture(L"fruit42.png");

	//[graph] ==========================
	//read from tilemap
	ReadInfoFromTileMap(tileMap);
	BuildGraph(graph, GRAPH_row, GRAPH_column, NODE_SPACING, GRAPH_INIT_POS);
	context.Init(GRAPH_row * GRAPH_column);

	//[graph path]  ==========================
	std::list<size_t> path;

	//[loop]
	bool done = false;
	while (!done)
	{
		done = myWindow.ProcessMessage();
		//run logic ==========================
		if (inputSys->IsKeyPressed(KeyCode::ESCAPE)){done = true; 	}
		//time
		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		lastTime = currTime;

		//begin render ==========================
		DebugUI::BeginRender();
		SpriteRenderer::Get()->BeginRender();
		GraphicSystem::Get()->BeginRender();
 

		//tile map ==========================
		tileMap.Render(cam);

		//camera ==========================
		cam.UpdateControl(0.03f);

		//draw graph ==========================
		if (showGraph)
		{
			ShowGridGraph(context, graph, showConnection);
		}
		//draw path
		if (context.found && showPath)
		{
			ShowPath(context, graph);
		}
		ShowStartEndPoint(startX, startY, endX, endY, tileMap.GetTileSize());

		//simple draw ==========================

		//SimpleDraw::AddLine({ 0.0f }, { 1.0f,0.0f,0.0f }, Colors::Red);
		//SimpleDraw::AddLine({ 0.0f }, { 0.0f,1.0f,0.0f }, Colors::Aqua);
		//SimpleDraw::AddLine({ 0.0f }, { 0.0f,0.0f,1.0f }, Colors::LawnGreen);
		//pick start end by mouse
		if (pick_start)
		{
			UpdateInputForGraph(startX, startY, endX, endY, tileMap.GetTileSize());
		}
		//tile map brush
		if (tilemap_brush_type >= 0)
		{
			if (inputSys->IsMouseDown(MouseButton::LBUTTON))
			{
				int sx = static_cast<int>(InputSystem::Get()->GetMouseScreenX() / tileMap.GetTileSize());
				int sy = static_cast<int>(InputSystem::Get()->GetMouseScreenY() / tileMap.GetTileSize());
				if (sx < GRAPH_column && sy < GRAPH_row)// inside map
				{
					tileMap.GetTileMap()[sx + sy * GRAPH_column] = tilemap_brush_type;
				}
			}
		}


		ShowDebugUi(tileMap,deltaTime);
		//ImGui::ShowDemoWindow();


		//end render ==========================
		SpriteRenderer::Get()->EndRender();
		SimpleDraw::Render(cam);
		SimpleDraw::Render2D();

		DebugUI::EndRender();

		GraphicSystem::Get()->EndRender();//flip buffer
		inputSys->Update();
	}

	//terminate ==========================
	GraphicSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	DebugUI::Terminate();
	SimpleDraw::StaticTerminate();
	myWindow.Terminate();

	return 0;

}

void ShowDebugUi(TileMap& tilemap, float deltatime)
{
	//Imgui stuff ==========================
	//ImGui::ShowDemoWindow();

	if (ImGui::CollapsingHeader("Path finding methods"))
	{
		if (ImGui::Button("BFS"))
		{
			context.Reset();
			//context.start = 
			context.start = startX + (startY * GRAPH_column);
			context.end = endX + (endY * GRAPH_column);
			//context.open[context.start] = true;

			GraphSearch::BFS<Graph<MapNode>, CanOpenNode> bfs(graph);

			bfs.Open(context, startX + (startY * GRAPH_column), InvalidNode);
			auto time_start = std::chrono::high_resolution_clock::now();

			bfs.Execute(context);
			auto time_end = std::chrono::high_resolution_clock::now();
			time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0f;

		}
		ImGui::SameLine();

		if (ImGui::Button("DFS"))
		{
			context.Reset();
			//context.start = 
			context.start = startX + (startY * GRAPH_column);
			context.end = endX + (endY * GRAPH_column);

			GraphSearch::DFS<Graph<MapNode>,CanOpenNode> dfs(graph);

			dfs.Open(context, startX + (startY * GRAPH_column), InvalidNode);
			auto time_start = std::chrono::high_resolution_clock::now();

			dfs.Execute(context);
			auto time_end = std::chrono::high_resolution_clock::now();
			time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0f;

		}
		if (ImGui::Button("Dijkstra"))
		{
			context.Reset();
			//context.start = 
			context.start = startX + (startY * GRAPH_column);
			context.end = endX + (endY * GRAPH_column);

			GraphSearch::Dijkstra<Graph<MapNode>, CanOpenNode_Astar, GetGCost> dijkstra(graph);

			dijkstra.Open(context, startX + (startY * GRAPH_column), InvalidNode);
			//time
			auto time_start = std::chrono::high_resolution_clock::now();

			dijkstra.Execute(context);
			auto time_end = std::chrono::high_resolution_clock::now();
			time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0f;

			if (context.found)
			{
				int ied = endX + endY * GRAPH_column;
				g_cost = context.g[ied];
			}

		}
		ImGui::SameLine();

		if (ImGui::Button("A*"))
		{
			context.Reset();
			context.start = startX + (startY * GRAPH_column);
			context.end = endX + (endY * GRAPH_column);

			GraphSearch::Astar<NavGraph,CanOpenNode_Astar,
				GetGCost,
				GetHCost<Heuristic>> astar(graph);

			astar.Open(context, startX + (startY * GRAPH_column), InvalidNode);

			//time
			auto time_start = std::chrono::high_resolution_clock::now();

			astar.Execute(context);

			auto time_end = std::chrono::high_resolution_clock::now();
			time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0f;


			if (context.found)
			{
				int ied = endX + endY * GRAPH_column;
				g_cost = context.g[ied];
			}

		}

	}

	ImGui::TextDisabled("[ --------------------- ] ");
	ImGui::Separator();
	if (ImGui::Button("Reset Context"))
	{
		context.Reset();
		g_cost = 0;
		time_cost = 0.0f;
	}

	//=================
	
	ImGui::TextDisabled("[ --------------------- ] ");
	ImGui::Text("[FPS] : %f", 1.0f / deltatime);
	ImGui::TextDisabled("[ --------------------- ] ");
	ImGui::Text("[Pick point] mouseL start, mouseR end");
	ImGui::Checkbox("[Pick point] On/Off", &pick_start);
	ImGui::TextDisabled("[ --------------------- ] ");

	ImGui::Text("[Start]: [%d , %d]", startX, startY);
	ImGui::Text("[End]: [%d , %d]", endX, endY);

	ImGui::Text("[Mouse]: [ %d , %d ]", inputSys->GetMouseScreenX(), inputSys->GetMouseScreenY());
	ImGui::Text("[Path cost] : %f", g_cost);
	ImGui::Text("[Time cost] : %f", time_cost);

	ImGui::TextDisabled("[ --------------------- ] ");



	ImGui::Checkbox("[Show Graph] On/Off", &showGraph);
	ImGui::Checkbox("[Show Path] On/Off", &showPath);
	ImGui::Checkbox("[Show path connection] On/Off", &showConnection);



	ImGui::TextDisabled("[ --------------------- ] ");
	if (ImGui::CollapsingHeader("Tilemap edit"))
	{
		//tile map edit
		ImGui::Text("[TileMap Brush] current : %d", tilemap_brush_type);
		if (ImGui::Button("[Disable Brush]")) tilemap_brush_type = -1;

		if (tilemap_brush_type >= 0)
		{
			ImGui::Image(GetShaderResourceView(*TextureManager::Get()->GetTexture(tilemap.GetTileSet()[tilemap_brush_type])), { 30.0f,30.0f });
			pick_start = false;
		}

		if (ImGui::ImageButton(GetShaderResourceView(*TextureManager::Get()->GetTexture(tilemap.GetTileSet()[0])), { 30.0f,30.0f }))
		{
			tilemap_brush_type = 0;

			//size_t index = startX + startY * GRAPH_column;
			//(tilemap.GetTileMap())[index] = 0;
		}
		ImGui::SameLine();

		if (ImGui::ImageButton(GetShaderResourceView(*TextureManager::Get()->GetTexture(tilemap.GetTileSet()[1])), { 30.0f,30.0f }))
		{
			tilemap_brush_type = 1;

			//size_t index = startX + startY * GRAPH_column;
			//(tilemap.GetTileMap())[index] = 1;
		}
		ImGui::SameLine();

		if (ImGui::ImageButton(GetShaderResourceView(*TextureManager::Get()->GetTexture(tilemap.GetTileSet()[2])), { 30.0f,30.0f }))
		{
			tilemap_brush_type = 2;
			//size_t index = startX + startY * GRAPH_column;
			//(tilemap.GetTileMap())[index] = 2;
		}
		if (ImGui::Button("[Rebuild Graph]"))
		{
			RebuildGraph(graph, GRAPH_row, GRAPH_column, NODE_SPACING, GRAPH_INIT_POS, tilemap);
			tilemap_brush_type = -1;
		}


	}

	//ImGui::ImageButton(*(TextureManager::Get()->GetTexture(imgButton)), { 32.0f,32.0f });
	



}