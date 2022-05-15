#include "Precompiled.h"
#include "Sampler.h"
#include "GraphicsSystem.h"
#include "D3DUtil.h"



using namespace H;
using namespace H::Graphics;

namespace
{
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case H::Graphics::Sampler::Filter::Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case H::Graphics::Sampler::Filter::Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case H::Graphics::Sampler::Filter::Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
			break;
		default:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode addressMode)
	{
		switch (addressMode)
		{
		case H::Graphics::Sampler::AddressMode::Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case H::Graphics::Sampler::AddressMode::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case H::Graphics::Sampler::AddressMode::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case H::Graphics::Sampler::AddressMode::Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		default:
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		}
	}

}

H::Graphics::Sampler::~Sampler()
{
	ASSERT(mSampler == nullptr,
		"[Sampler] Terminate must be called before destruction. ");

}

void H::Graphics::Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	D3D11_FILTER d3dFilter = GetFilter(filter);
	D3D11_TEXTURE_ADDRESS_MODE d3dAddressMode = GetAddressMode(addressMode);

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = d3dFilter;
	sampDesc.AddressU = d3dAddressMode;
	sampDesc.AddressV = d3dAddressMode;
	sampDesc.AddressW = d3dAddressMode;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 1.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto hr = GetDevice()->CreateSamplerState(&sampDesc, &mSampler);
	ASSERT(SUCCEEDED(hr), "[Sampler] Failed to create sampler state. ");

}

void H::Graphics::Sampler::Terminate()
{
	SafeRelease(mSampler);
}

void H::Graphics::Sampler::BindPS(uint32_t slot) const
{
	 GetContext()->PSSetSamplers(slot, 1, &mSampler);

}
void H::Graphics::Sampler::UnBindPS(uint32_t slot) const
{
	GetContext()->PSSetSamplers(slot, 0, nullptr);

}
