#include "Precompiled.h"
#include "BlendState.h"
#include "D3DUtil.h"
#include "GraphicsSystem.h"

using namespace H;
using namespace H::Graphics;

namespace {

	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case H::Graphics::BlendState::Mode::Opaque:					return D3D11_BLEND_ONE;
			break;
		case H::Graphics::BlendState::Mode::AlphaBlend:				return D3D11_BLEND_SRC_ALPHA;
			break;
		case H::Graphics::BlendState::Mode::AlphaPremultiplied:		return D3D11_BLEND_ONE;
			break;
		case H::Graphics::BlendState::Mode::Additive:				return D3D11_BLEND_SRC_ALPHA;
			break;
		default:
			break;
		}
		return D3D11_BLEND_ONE;
	}
	D3D11_BLEND GetDestBlend(BlendState::Mode mode) 
	{
		switch (mode)
		{
		case H::Graphics::BlendState::Mode::Opaque:					return D3D11_BLEND_ZERO;
			break;
		case H::Graphics::BlendState::Mode::AlphaBlend:				return D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case H::Graphics::BlendState::Mode::AlphaPremultiplied:		return D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case H::Graphics::BlendState::Mode::Additive:				return D3D11_BLEND_ONE;
			break;
		default:
			break;
		}
		return D3D11_BLEND_ONE;
	}


}

void H::Graphics::BlendState::ClearState()
{
	GetContext()->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}


H::Graphics::BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "[BlendState] Blend state not released");
}

void H::Graphics::BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = /*GraphicSystem::Get()->*/GetDevice()->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "[BlendState] Failed to create blend state");
	


}

void H::Graphics::BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void H::Graphics::BlendState::Set()
{
	/*GraphicSystem::Get()->*/GetContext()->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}

//void H::Graphics::BlendState::Clear()
//{
//	/*GraphicSystem::Get()->*/GetContext()->OMSetBlendState(nullptr, nullptr, UINT_MAX);
//}
