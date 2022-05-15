#pragma once
#include <HUGE\Inc\HUGE.h>
#include <HUGE\Inc\AppState.h>
#include "FSMObjects.h"


using namespace H;
using namespace H::Graphics;

class WorkingSheep : public H::AppState
{
public:
	 void Initialize();
	 void Terminate();
	 void Render();
	 void Update(float deltatime);
	 void DebugUI();

private:
 
	std::vector<Bush> bushes;
	Sheep sheep;

	H::Graphics::TextureId house;
};

