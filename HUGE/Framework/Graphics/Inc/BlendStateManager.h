#ifndef HUGE_GRAPHICS_BLENDSTATEMANAGER_H
#define HUGE_GRAPHICS_BLENDSTATEMANAGER_H
#include "BlendState.h"

namespace H::Graphics
{
	namespace Blend
	{
		extern const std::string  Additive ;
		extern const std::string  Opaque ;
		extern const std::string  AlphaBlend;
		extern const std::string  AlphaPremultiplied;

	}
	class BlendStateManager
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static BlendStateManager* Get();

	public:
		BlendStateManager() = default;

		void Initialize();
		void Terminate();


		bool AddBlendState(std::string name, H::Graphics::BlendState::Mode mode);
		H::Graphics::BlendState* GetBlendState(std::string name);

	private:
		std::unordered_map<std::string, std::unique_ptr<H::Graphics::BlendState>> mInventory;

	};

	
}//namespace H::Graphics

 

#endif // !HUGE_GRAPHICS_BLENDSTATEMANAGER_H
