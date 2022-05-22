#ifndef HUGE_GRAPHICS_EFFECTMANAGER_H
#define HUGE_GRAPHICS_EFFECTMANAGER_H
#include "ShaderEffect.h"

namespace H::Graphics
{
	class ShaderEffectManager
	{
	public:
		using ShaderEffectInventory = std::unordered_map<std::string, std::unique_ptr<ShaderEffect>>;
		static void StaticInitialize();
		static void StaticTerminate();
		static ShaderEffectManager* Get();

	public:
		ShaderEffectManager() = default;

		void Initialize();
		void Terminate();

		bool AddEffect(std::unique_ptr<ShaderEffect>&& shaderEffect, const std::string& name);

		const ShaderEffect& GetEffect(const std::string& name) const;

	private:
		ShaderEffectInventory mInventory;
	};
}


#endif // !HUGE_GRAPHICS_EFFECTMANAGER_H

