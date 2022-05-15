#pragma once
#include <HUGE/Inc/HUGE.h>
using namespace H::Graphics;

class TScreenRT
{
public:
	void Initialize();
	void Terminate();
	void Begin();
	void End();
 	void DebugUI();
	RenderTarget mRT;

private:

};
 