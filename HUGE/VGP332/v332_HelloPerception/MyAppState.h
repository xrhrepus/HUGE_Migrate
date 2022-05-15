#pragma once
#include <HUGE\Inc\HUGE.h>
#include <HUGE\Inc\AppState.h>
#include "SteerObject.h"

using namespace H;
using namespace H::Graphics;
using namespace H::AI;


class SteeringSheep : public H::AppState
{
public:
	 void Initialize();
	 void Terminate();
	 void Render();
	 void Update(float deltatime);
	 void DebugUI();

private:
	Camera camera;
 	std::unique_ptr<Sheep> alphaSheep;

	H::AI::AIWorld world;
	const int world_row = 16;
	const int world_col = 25;
	const int world_cellSize = 50;

	std::vector<std::unique_ptr<Sheep>> sheeps;
	const int num_of_sheep = 20;

 	std::vector<Circle> circles;
 
	H::Graphics::TextureId alphaSheepTexture;

	H::Graphics::TextureId house;
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
	int neighborsize = 0;

	float* data1;
	float* data2;
	float* data3;
	float* data4;



};

