#pragma once
#include "HMaterial.h"

class HRenderer
{
public:
	HRenderer(const HMaterial& material, const H::Graphics::MeshBuffer& meshBuffer);

	void Render() const;
	void DebugUI();

private:
	const H::Graphics::MeshBuffer& mMeshBuffer;
	const HMaterial& mMaterial;
};
