#include "EditState.h"
#include "MainState.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
	H::AppConfig config;
	config.appName = "Huge Editor";
	config.maximize = true;
	config.imguiDocking = true;

	HugeApp::AddState<MainState>("MainState");
	HugeApp::Run(std::move(config));
}