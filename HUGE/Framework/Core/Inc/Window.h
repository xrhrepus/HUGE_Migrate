#ifndef HUGE_CORE_WINDOW_H
#define HUGE_CORE_WINDOW_H
       //<engine>_<libname>_<filename>
namespace H	::Core	{
	
	class Window
	{
	public:
		//c++ 11
		Window() = default;

		//HINSTANCE/
		//win32 API
		void Initialize(
			HINSTANCE instance, LPCSTR appname,uint32_t width,uint32_t height,bool maximize = false );
		void Terminate();

		bool ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }




	private:
		//default member init
		//for multi constructor overload
		HINSTANCE mInstance{ nullptr };//window refers to
		HWND mWindow{ nullptr };//window redering
		std::string mAppName;

	};

	
}//namespace H::Core
//namespace H

#endif // !HUGE_WINDOW_H

