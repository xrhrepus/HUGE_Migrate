#include "MyAppState.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	HugeApp::AddState<CheatGame>("cheatGame");
	HugeApp::Run(AppConfig());

	return 0;

}

