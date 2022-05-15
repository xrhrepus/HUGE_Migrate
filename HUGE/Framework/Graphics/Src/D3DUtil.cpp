#include "Precompiled.h"
#include "D3DUtil.h"
#include "Texture.h"
#include "GraphicsSystem.h"

ID3D11ShaderResourceView * H::Graphics::GetShaderResourceView(const Texture & texture)
{
	return texture.mShaderResourceView;
}
ID3D11Device* H::Graphics::GetDevice()
{
	return H::Graphics::GraphicSystem::Get()->mD3DDevice;
}

ID3D11DeviceContext* H::Graphics::GetContext()
{
	return H::Graphics::GraphicSystem::Get()->mImmediateContext;

}

