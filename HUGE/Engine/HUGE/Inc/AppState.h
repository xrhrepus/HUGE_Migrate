#ifndef HUGE_APPSTATE_H
#define HUGE_APPSTATE_H

namespace H
{
	class AppState
	{
	public:
		virtual void Initialize() = 0;
		virtual void Terminate() = 0;
		virtual void Render() = 0;
		virtual void Update(float deltatime) = 0;
		virtual void DebugUI() = 0;


	};

}//namespace H

#endif // !HUGE_APPSTATE_H


