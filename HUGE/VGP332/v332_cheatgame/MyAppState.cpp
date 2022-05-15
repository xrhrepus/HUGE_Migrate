#include "MyAppState.h"
 
 
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 

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


void CheatGame::Initialize()
{
 
	srand(std::time(nullptr));

	//[ai world] ==========================
	world.Initialize(world_row, world_col, world_cellSize);

	//[graph] ==========================
	tileMap.LoadTileMap(L"../Assets/TileMap/classroom32_20x40.txt", L"../Assets/TileMap/tileset32x32.txt");
	//read from tilemap
	ReadInfoFromTileMap(tileMap);
	BuildGraph(graph, GRAPH_row, GRAPH_column, NODE_SPACING, GRAPH_INIT_POS);
	context.Init(GRAPH_row * GRAPH_column);


 	//[texture] ==========================
	alphaSheepTexture = TextureManager::Get()->LoadTexture(L"alphaSheep64.png");
	dest = TextureManager::Get()->LoadTexture(L"fruit42.png");
	classroom = TextureManager::Get()->LoadTexture(L"classroom900.png");
	rulerMap = TextureManager::Get()->LoadTexture(L"rulermap.jpg");

	
	//init objs
	ResetGame();
 
 
	//[camera] ==========================
	camera.SetPosition({ 0.0f,0.0f, -10.0f });
	camera.SetDirection({ 0.0f,0.0f, 1.0f });

	DepthStencilManager::Get()->AddDepthStencilState("ZTestNever", true, DepthStencilState::DepthWriteMask::Mask_All, DepthStencilState::Comparison::Never,
		false, DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep,
		DepthStencilState::Comparison::Never, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep, DepthStencilState::StencilOperation::Keep);
 
 }

void CheatGame::Terminate()
{
 }

void CheatGame::Render()
{
	//DepthStencilManager::Get()->GetDepthStencilState("ZTestNever")->Set();
	H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(classroom)), { 160.0f, 0.0f });

	//DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();

	SimpleDraw::AddScreenRect({ {200.0f,240.0f},10.0f }, Colors::Aqua);//lt
	SimpleDraw::AddScreenRect({ {200.0f,700.0f},10.0f }, Colors::Aqua);//lb
	SimpleDraw::AddScreenRect({ {950.0f,240.0f},10.0f }, Colors::Aqua);//rt
	SimpleDraw::AddScreenRect({ {950.0f,700.0f},10.0f }, Colors::Aqua);//rb

 
 	for (const auto& p : papers)
	{
		p->Render();
	}
 	//student
	for (const auto& s : students)
	{
		s->Render();
	}
	teacher->Render();

	//DepthStencilManager::Get()->GetDepthStencilState("ZTestAlways")->Set();
	SimpleDraw::Render(camera);
	

}

void CheatGame::Update(float deltatime)
{
	fps = 1.0f / deltatime;
	if (gameStart)
	{
		world.Update(deltatime);
		for (const auto& s : students)
		{
			s->Update(deltatime);
		}
		teacher->Update(deltatime);
	}
 
}

void CheatGame::DebugUI()
{
	//ImGui::ShowDemoWindow();
	teacher->DebugUI();
	ImGui::Begin("Game");
	ImGui::Text("fps: %f", fps);
	ImGui::DragInt("Num students:", &num_of_student, 1.0f, 0, 30);
	if (ImGui::Button("reset"))
	{
		ResetGame();
	}
	if (ImGui::Button("start"))
	{
		StartGame();
	}

	if (ImGui::ArrowButton("-1", ImGuiDir{ 0 }))
	{
		selectedStudent -= selectedStudent > 0 ? 1 : 0;
	}
	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();
	if (ImGui::ArrowButton("+1", ImGuiDir{ 1 }))
	{
		selectedStudent += selectedStudent < num_of_student - 1 ? 1 : 0;
	}

	ImGui::DragInt("select student: ", &selectedStudent,1.0f,0,num_of_student-1);
	
	size_t ssz = students.size();
	for (size_t i = 0; i < ssz; i++)
	{
		students[i]->DebugUI();
	}
 
	SimpleDraw::AddScreenRect({ students[selectedStudent]->position,{40.0f,40.0f} }, Colors::LightCyan);
	SimpleDraw::AddScreenLine(students[selectedStudent]->position, papers[selectedStudent]->mPosition, Colors::LawnGreen);
	SimpleDraw::AddScreenRect({ papers[selectedStudent]->mPosition,{40.0f,40.0f} }, Colors::LightCyan);




  
 	ImGui::End();

}

void CheatGame::ResetGame()
{
	//[init objs]
	const int desk_in_one_row = 4;
	papers.resize(num_of_student);
	//deskObstacle.reserve(num_of_student);
	static bool ObsAdded = false;

	for (int i = 0; i < num_of_student; i++)
	{
		papers[i] = std::make_unique<ExamPaper>();
		Vector2 pos = { (i % desk_in_one_row)*180.0f + 350.0f, (i / desk_in_one_row) *120.0f + 300.0f };
		papers[i]->Init(i, pos);
		//desk obstacle
		//deskObstacle.emplace_back(Circle{ pos, radius_of_desk });
		if (!ObsAdded)
		{
			//world.AddObstabcles(Circle{ {pos.x,pos.y - 32.0f}, radius_of_desk });
		}
 	}
	ObsAdded = true;

	students.resize(num_of_student);
	for (int i = 0; i < num_of_student; i++)
	{
		students[i] = std::make_unique<Student>(world, *(papers[i]));
		students[i]->Init(&papers, i);
	}
	teacher = std::make_unique<Teacher>(world);
	teacher->Init(&students);




	//
	gameStart = false;
}

void CheatGame::StartGame()
{
	gameStart = true;
}
