#ifndef HUGE_CORE_WINDOWMESSAGEHANDLER_H
#define HUGE_CORE_WINDOWMESSAGEHANDLER_H

namespace H::Core
{
	class WindowMessageHandler
	{
	public:
		using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

		void Hook(HWND window, Callback cb);
		void UnHook();

		LRESULT ForwardMessage(HWND window, UINT message, WPARAM wPara, LPARAM lPara);

	private:
		HWND mWindow{ nullptr };
		Callback mPreviousCallback{ nullptr };

	};



}//namespace H::Core



#endif // !HUGE_CORE_WINDOWMESSAGEHANDLER_H
