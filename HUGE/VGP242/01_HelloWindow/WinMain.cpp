#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	H::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hell0Window", 1280, 720);

	bool done = false;

	while (!done)
	{
		done = myWindow.ProcessMessage();

		//run game logic below
	}

	myWindow.Terminate();

	return 0;
}