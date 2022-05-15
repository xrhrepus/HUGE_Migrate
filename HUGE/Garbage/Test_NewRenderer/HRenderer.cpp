#include "HRenderer.h"

HRenderer::HRenderer(const HMaterial & material, const H::Graphics::MeshBuffer & meshBuffer)
	:mMaterial(material),mMeshBuffer(meshBuffer)
{
}

void HRenderer::Render() const
{
	// shader bind
	// meshbuffer draw
}

void HRenderer::DebugUI()
{
}
