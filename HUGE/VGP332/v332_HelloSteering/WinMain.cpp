#include "MyAppState.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	HugeApp::AddState<SteeringSheep>("state1");
	HugeApp::Run(AppConfig());

	return 0;

}

