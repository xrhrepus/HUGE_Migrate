#include "MyAppState.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	HugeApp::AddState<NavalGame>("navalGame");
	HugeApp::Run(AppConfig());

	return 0;

}

