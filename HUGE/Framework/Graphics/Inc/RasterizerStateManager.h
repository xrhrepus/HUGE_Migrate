#ifndef HUGE_GRAPHICS_RASTERIZESTATEMANAGER_H
#define HUGE_GRAPHICS_RASTERIZESTATEMANAGER_H
#include "RasterizerState.h"

namespace H::Graphics
{
	class RasterizerStateManager
	{
 	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static RasterizerStateManager* Get();

	public:
		RasterizerStateManager() = default;

		void Initialize();
		void Terminate();


		bool AddRasterizerState(std::string name, H::Graphics::RasterizerState::CullMode cmode, H::Graphics::RasterizerState::FillMode fmode);
		H::Graphics::RasterizerState* GetRasterizerState(std::string name);

	private:
		std::unordered_map<std::string, std::unique_ptr<H::Graphics::RasterizerState>> mInventory;
	};


}
 

#endif // !HUGE_GRAPHICS_RASTERIZESTATEMANAGER_H

