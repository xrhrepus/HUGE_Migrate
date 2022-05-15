#pragma once
#include <HUGE/Inc/HUGE.h>

class HShader
{
public:
	typedef uint8_t RegisterIndex;
	typedef std::pair<H::Graphics::ConstantBuffer&, RegisterIndex> ConstantBufferIndexPair;
	typedef std::pair<H::Graphics::Texture&, RegisterIndex> TextureIndexPair;

public:
	HShader(const std::filesystem::path& path);
	HShader(std::filesystem::path&& path);

	void AddConstantBuffer(const ConstantBufferIndexPair& constBuffer);
	void AddTexture(const TextureIndexPair& texture);

	void SetDepthState(const H::Graphics::DepthStencilState::DepthState& state);
	void SetBlendMode(const H::Graphics::BlendState::Mode mode);
	void SetStencilState(const H::Graphics::DepthStencilState::StencilState& state);

	void Bind() const;
private:
	std::filesystem::path mPath;
	H::Graphics::VertexShader mVertexShader;
	H::Graphics::PixelShader mPixelShader;

	// Buffer and texture
	std::vector<ConstantBufferIndexPair> mConstantBuffers;
	std::vector<TextureIndexPair> mTextures;
	// States
	H::Graphics::BlendState::Mode mBlendMode;
	H::Graphics::DepthStencilState::DepthState mDepthState;
	H::Graphics::DepthStencilState::StencilState mStencilState;
};