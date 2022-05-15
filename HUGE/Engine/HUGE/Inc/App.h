#ifndef HUGE_APP_H
#define HUGE_APP_H

namespace H
{
	class AppState;

	struct AppConfig
	{
		AppConfig() = default;
		AppConfig(std::string name)
			:appName (std::move(name))
		{}
		std::string appName = "HUGE";
		std::filesystem::path assetDirectory = L"../../Assets";//from solution 1 up
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		bool maximize = false;
		bool imguiDocking = false;
	};

	class App
	{
	public:
		void Initialize(AppConfig appConfig);

		template<typename StateType>
		void AddState(std::string name);

		void ChangeState(std::string name);


		void Run(AppConfig appConfig);
		void ShutDown();

	private:
		AppConfig mAppConfig;
		H::Core::Window mWindow;
		std::map<std::string, std::unique_ptr< AppState>> mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
	};


	template<typename StateType>
	void App::AddState(std::string name)
	{
		//NOTE190808 structure binding c++17
		auto[newState, success] = mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		if (!mCurrentState)
		{
			mCurrentState = (newState->second).get();
		}


	}


}

#endif // !HUGE_APP_H

