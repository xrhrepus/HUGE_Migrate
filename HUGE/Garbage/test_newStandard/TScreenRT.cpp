#include "TScreenRT.h"

void TScreenRT::Initialize()
{
	mRT.
		Initialize(GraphicSystem::Get()->GetBackBufferWidth(), GraphicSystem::Get()->GetBackBufferHeight(), H::Graphics::RenderTarget::Format::RGBA_U32);

}

void TScreenRT::Terminate()
{
	mRT.Terminate();
}

void TScreenRT::Begin()
{
	mRT.BeginRender();
}

void TScreenRT::End()
{
	mRT.EndRender();
}

void TScreenRT::DebugUI()
{
}
