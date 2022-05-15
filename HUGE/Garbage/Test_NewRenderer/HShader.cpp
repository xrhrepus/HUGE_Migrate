#include "HShader.h"

HShader::HShader(const std::filesystem::path& path)
	:mPath(path)
{
}
HShader::HShader(std::filesystem::path&& path)
	:mPath(std::move(path))
{
}

void HShader::AddConstantBuffer(const ConstantBufferIndexPair& constBuffer)
{
	mConstantBuffers.push_back(constBuffer);
}

void HShader::AddTexture(const TextureIndexPair& texture)
{
	mTextures.push_back(texture);
}

void HShader::SetDepthState(const H::Graphics::DepthStencilState::DepthState & state)
{
	mDepthState = state;
}

void HShader::SetBlendMode(const H::Graphics::BlendState::Mode mode)
{
	mBlendMode = mode;
}

void HShader::SetStencilState(const H::Graphics::DepthStencilState::StencilState & state)
{
	mStencilState = state;
}

void HShader::Bind() const
{
	//bind all const buffer and texture
}
