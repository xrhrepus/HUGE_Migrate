#include "MyAppState.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
  	HugeApp::AddState<MyAppState>("state1");
	HugeApp::Run(AppConfig());
}