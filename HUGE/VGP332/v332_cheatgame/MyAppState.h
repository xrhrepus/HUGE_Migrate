#pragma once
#include <HUGE\Inc\AppState.h>
#include <HUGE\Inc\HUGE.h>
#include "Student.h"
#include "ExamPaper.h"
#include "Teacher.h"

//#include "GraphUtil.h"

 
using namespace H;
using namespace H::Graphics;
using namespace H::AI;
using namespace H::AI::GraphSearch;
using namespace H::Input;

struct MapNode : public H::AI::Node
{
	H::Math::Vector3 position;
	int terrainType;
};
using NavGraph = H::AI::Graph<MapNode>;

class CheatGame : public H::AppState
{
public:
	 void Initialize();
	 void Terminate();
	 void Render();
	 void Update(float deltatime);
	 void DebugUI();


public:
	void ResetGame();
	void StartGame();
	bool gameStart = false;
	float fps;
private:
	Camera camera;
 
	TileMap tileMap;
	H::AI::AIWorld world;
	const int world_row = 16;
	const int world_col = 30;
	const int world_cellSize = 32;
	Graph<MapNode> graph;
	GraphSearch::Context context;

	std::unique_ptr<Teacher> teacher;
	std::vector<std::unique_ptr<Student>> students;
	int num_of_student = 12;
	std::vector<std::unique_ptr<ExamPaper>> papers;
 
 	static std::vector<Circle> deskObstacle;
	const float radius_of_desk = 16.0f;
	//const int num_of_circle = 20;
 


	H::Graphics::TextureId alphaSheepTexture;

	H::Graphics::TextureId classroom;
	H::Graphics::TextureId dest;
	H::Graphics::TextureId rulerMap;

	//alpha control
	bool aSeek;
	bool aArrive;

	bool aWander;
	bool aObsAvoid;
	bool aSeperation;
	bool aAlignment;
	bool aCohesion;

	//group control
	bool groupSeek;
	bool groupArrive;
	bool groupWander;
	bool groupObsAvoid;
	bool groupSeperation;
	bool groupAlignment;
	bool groupCohesion;

 
	//debug
	int selectedStudent = 0;
 
 


};

