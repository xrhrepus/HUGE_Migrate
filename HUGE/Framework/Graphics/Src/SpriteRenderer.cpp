#include "Precompiled.h"
#include "SpriteRenderer.h"
#include "GraphicsSystem.h"
#include "Texture.h"
#include "D3DUtil.h"
#include <DirectXTK\Inc\SpriteBatch.h>

using namespace H;
using namespace H::Graphics;

namespace 
{
	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[Graphic::SpriteRenderer] system already initialized!");
	sSpriteRenderer = std::make_unique<SpriteRenderer>();
	sSpriteRenderer->Initialize();

}
void SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}

}

SpriteRenderer* SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "[Graphic::SpriteRenderer] instance not created");
	return sSpriteRenderer.get();
}

SpriteRenderer::~SpriteRenderer()
{
	ASSERT(mSpriteBatch == nullptr, "[Graphic::SpriteRenderer]  not freed");//?

}

void SpriteRenderer::Initialize()
{
	ASSERT(mSpriteBatch == nullptr, "[Graphic::SpriteRenderer]  already initialized!");
 	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(GetContext());

}
void SpriteRenderer::Terminate()
{
	ASSERT(mSpriteBatch != nullptr, "[Graphic::SpriteRenderer]  already Terminated!");
	mSpriteBatch.reset();
}

void SpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "[Graphic::SpriteRenderer]  not initialized!");
	mSpriteBatch->Begin();
}
void SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[Graphic::SpriteRenderer]  not initialized!");
	mSpriteBatch->End();
}

void SpriteRenderer::Draw(const Texture& texture, const H::Math::Vector2& pos)
{
	ASSERT(mSpriteBatch != nullptr, "[Graphic::SpriteRenderer]  not initialized!");
	mSpriteBatch->Draw( GetShaderResourceView(texture), DirectX::XMFLOAT2{ pos.x,pos.y });
}
