#include "Precompiled.h"
#include "Texture.h"
#include "GraphicsSystem.h"
#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace H;
using namespace H::Graphics;


Texture::~Texture()
{
	Terminate();// ????????????
	ASSERT(mShaderResourceView == nullptr,
		"[Texture] Terminate must be called before destruction. ");

}

void H::Graphics::Texture::Initialize(const std::filesystem::path filename)
{
 
	HRESULT hr = DirectX::CreateWICTextureFromFile(GetDevice(), GetContext(), filename.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture %ls. ",filename);
	   	
}

void H::Graphics::Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void H::Graphics::Texture::BindPS(uint32_t slot) const
{
	 GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);

}

void H::Graphics::Texture::BindVS(uint32_t slot) const
{
	 GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);

}

void H::Graphics::Texture::UnBindPS(uint32_t slot) const
{
	GetContext()->PSSetShaderResources(slot, 0, nullptr);

}

void H::Graphics::Texture::UnBindVS(uint32_t slot) const
{
	GetContext()->VSSetShaderResources(slot, 0, nullptr);
}

