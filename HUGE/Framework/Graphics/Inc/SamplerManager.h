#ifndef HUGE_GRAPHICS_SAMPLERMANAGER_H
#define HUGE_GRAPHICS_SAMPLERMANAGER_H

#include "Sampler.h"

namespace H::Graphics
{
	namespace SamplerType
	{
		extern const std::string  PointClamp;
		extern const std::string  PointWrap;
		extern const std::string  LinearClamp;
		extern const std::string  LinearWrap;
		extern const std::string  AnisotropicClamp;
		extern const std::string  AnisotropicWrap;
	}
	class SamplerManager
	{
	public:
		static void StaticInitialize( );
		static void StaticTerminate();
		static SamplerManager* Get();

	public:
		SamplerManager() = default;
		//~SamplerManager();

		void Initialize();
		void Terminate();

		bool AddSampler(std::string name, Sampler::Filter filter, Sampler::AddressMode addressMode);
		Sampler* GetSampler(std::string name);

	private:
		std::unordered_map<std::string, std::unique_ptr< Sampler>> mInventory;
 	};
 
	
}//namespace H::Graphics

#endif // !HUGE_GRAPHICS_SAMPLERMANAGER_H

