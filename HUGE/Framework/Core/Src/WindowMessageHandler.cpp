#include "Precompiled.h"
#include "WindowMessageHandler.h"
#include "Debug.h"//?

using namespace H::Core;

void WindowMessageHandler::Hook(HWND window, Callback cb)
{
	mWindow = window;
	mPreviousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);
}
void WindowMessageHandler::UnHook()
{
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
	mWindow = nullptr;
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wPara, LPARAM lPara)
{
	ASSERT(mPreviousCallback, "[WindowMessageHandler] no callback is hooked");
	return CallWindowProcA((WNDPROC)mPreviousCallback, window, message, wPara, lPara);


}

