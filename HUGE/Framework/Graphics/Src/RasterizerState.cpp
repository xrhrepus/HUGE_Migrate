#include "Precompiled.h"
#include "RasterizerState.h"
#include "D3DUtil.h"
#include "GraphicsSystem.h"

using namespace H::Graphics;

namespace {
	D3D11_CULL_MODE GetCullMode(RasterizerState::CullMode mode)
	{
		switch (mode)
		{
			case H::Graphics::RasterizerState::CullMode::Front:		return D3D11_CULL_FRONT;
 			case H::Graphics::RasterizerState::CullMode::Back:		return D3D11_CULL_BACK;
 			case H::Graphics::RasterizerState::CullMode::None:		return D3D11_CULL_NONE;
 		}
		return D3D11_CULL_NONE;
	}
	D3D11_FILL_MODE GetFillMode(RasterizerState::FillMode mode)
	{
		switch (mode)
		{
			case H::Graphics::RasterizerState::FillMode::Solid:			return D3D11_FILL_SOLID;
 			case H::Graphics::RasterizerState::FillMode::WireFrame:		return D3D11_FILL_WIREFRAME;
 		}
		return D3D11_FILL_WIREFRAME;
	}


}

H::Graphics::RasterizerState::~RasterizerState()
{
	ASSERT(mRasterizerState == nullptr, "[RasterizerState] Rasterizer State not released");

}

void H::Graphics::RasterizerState::Initialize(CullMode cullMode, FillMode fillMode)
{
	D3D11_RASTERIZER_DESC desc{};

	desc.CullMode = GetCullMode(cullMode);
	desc.FillMode = GetFillMode(fillMode);
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &mRasterizerState);
	ASSERT(SUCCEEDED(hr), "[RasterizerState] Failed to create Rasterizer State");

}

void H::Graphics::RasterizerState::Terminate()
{
	SafeRelease(mRasterizerState);

}

void H::Graphics::RasterizerState::Set()
{
	GetContext()->RSSetState(mRasterizerState);
}

void H::Graphics::RasterizerState::Clear()
{
	GetContext()->RSSetState(nullptr);

}
