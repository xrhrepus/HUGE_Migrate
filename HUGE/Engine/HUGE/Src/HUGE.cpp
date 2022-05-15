#include "Precompiled.h"
#include "HUGE.h"
#include "App.h"

using namespace H;


H::App H::sApp;




void HugeApp::ChangeState(std::string name)
{
	sApp.ChangeState(std::move(name));
}

void HugeApp::Run(H::AppConfig appConfig)
{
	sApp.Run(appConfig);
}
