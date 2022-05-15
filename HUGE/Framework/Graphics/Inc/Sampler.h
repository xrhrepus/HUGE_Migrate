#ifndef HUGE_GRAPHIC_SAMPLER_H
#define HUGE_GRAPHIC_SAMPLER_H

namespace H::Graphics
{
 

	class Sampler
	{
	public:
		enum class Filter {Point,Linear,Anisotropic};
		enum class AddressMode {Border,Clamp,Mirror,Wrap };

		Sampler() = default;
		~Sampler();

		void Initialize(Filter filter , AddressMode addressMode);
		void Terminate();

		void BindPS(uint32_t slot = 0) const;
		void UnBindPS(uint32_t slot = 0) const;


	private:
		ID3D11SamplerState* mSampler{ nullptr };
	};

}// GRAPHICS
 
#endif // !HUGE_GRAPHIC_SAMPLER_H
