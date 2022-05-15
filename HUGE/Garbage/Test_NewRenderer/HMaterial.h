#pragma once
#include "HShader.h"

class HMaterial
{
public:
	HMaterial();
	~HMaterial();

private:
	HShader& mShader;
	std::vector<HShader::ConstantBufferIndexPair> mConstantBuffers;
	std::vector<HShader::TextureIndexPair> mTextures;
};
