#ifndef HUGE_GRAPHICS_EFFECTMANAGER_H
#define HUGE_GRAPHICS_EFFECTMANAGER_H
#include "ShaderEffect.h"

namespace H::Graphics
{
	class ShaderEffectManager
	{
		enum class EffectType
		{
			Texturing,Standard,Skinning,Toon
		};
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ShaderEffectManager* Get();

	public:
		ShaderEffectManager() = default;

		void Initialize();
		void Terminate();


		bool AddEffect(EffectType type ,std::string name);
		//H::Graphics::ShaderEffect* GetEffect(std::string name);

		//ShaderEffect* GetEffect(std::string name);


		template<class ShaderEffectType>
		ShaderEffectType* GetEffect(std::string name);

	private:
		//const std::filesystem::path path_Texturing = L"../../Assets/Shaders/Texturing.fx";
		//const std::filesystem::path path_Standard = L"../../Assets/Shaders/Standard.fx";
		//const std::filesystem::path path_Skinning = L"../../Assets/Shaders/Skinning.fx";

		std::unordered_map<std::string, std::unique_ptr<H::Graphics::ShaderEffect>> mInventory;
	};


	template<class ShaderEffectType>
	inline ShaderEffectType * ShaderEffectManager::GetEffect(std::string name)
	{
		auto iter = mInventory.find(name);
		if (iter != mInventory.end())
		{
			ShaderEffectType* p = dynamic_cast<ShaderEffectType*>(iter->second.get());
			if (p != nullptr)
			{
				return p;
			}

		}
		return nullptr;
	}

}


#endif // !HUGE_GRAPHICS_EFFECTMANAGER_H

