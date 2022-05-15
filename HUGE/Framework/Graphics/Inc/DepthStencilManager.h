#ifndef HUGE_GRAPHICS_DEPTHSTENCILMANAGER_H
#define HUGE_GRAPHICS_DEPTHSTENCILMANAGER_H
#include "DepthStencilState.h"

namespace H::Graphics
{
	class DepthStencilManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static DepthStencilManager* Get();
		enum DepthStencilEffectType
		{
			ZTestOn,
			ZTestOff,
			ZTestAlways
		};
	public:
		DepthStencilManager() = default;

		void Initialize();
		void Terminate();


		bool AddDepthStencilState(std::string name, bool depthEnable, DepthStencilState::DepthWriteMask depthWriteMask, DepthStencilState::Comparison depthFunc,
			bool stencilEnable, DepthStencilState::Comparison frontStencilFunc, DepthStencilState::StencilOperation frontStencilPass, DepthStencilState::StencilOperation frontStencilFail, DepthStencilState::StencilOperation frontDepthFail,
			DepthStencilState::Comparison backStencilFunc, DepthStencilState::StencilOperation backStencilPass, DepthStencilState::StencilOperation backStencilFail, DepthStencilState::StencilOperation backDepthFail);
		H::Graphics::DepthStencilState* GetDepthStencilState(std::string name);

	private:
		std::unordered_map<std::string, std::unique_ptr<H::Graphics::DepthStencilState>> mInventory;

	};
 
}
#endif // !HUGE_GRAPHICS_DEPTHSTENCILMANAGER_H

