#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "Common.h"

using namespace H;
using namespace H::Graphics;

#pragma region singleton
namespace
{
	std::unique_ptr<GraphicSystem> sGraphicSystem = nullptr;
	Core::WindowMessageHandler sWindowMessageHandler;
}

LRESULT CALLBACK Graphics::GraphicSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicSystem)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
			const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
			sGraphicSystem->Resize(width, height);
			break;
		}
		default:
			break;
		}
	}
	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}
GraphicSystem::~GraphicSystem()
{
	ASSERT(mD3DDevice == nullptr, "[Graphic::GraphicSystem] Terminate() must be called to clean up!");
}
void GraphicSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicSystem == nullptr, "[Graphic::GraphicSystem] system already initialized!");
	sGraphicSystem = std::make_unique<GraphicSystem>();
	sGraphicSystem->Initialize(window, fullscreen);
}

void GraphicSystem::StaticTerminate()
{
	if (sGraphicSystem != nullptr)
	{
		sGraphicSystem->Terminate();
		sGraphicSystem.reset();
		//NOTE:4.30
		//Release pointer
		//Releases ownership of its stored pointer, by returning its value and replacing it with a null pointer.
	}
}

GraphicSystem* GraphicSystem::Get()
{
	ASSERT(sGraphicSystem != nullptr, "[Graphic::GraphicSystem] instance not created");
	return sGraphicSystem.get();

}

#pragma endregion



void GraphicSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect = {};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	//passing window
	//NOTE:4.30
	//swapchain description
	mSwapChainDesc = {  };
	mSwapChainDesc.BufferCount = 1;// front / back
	mSwapChainDesc.BufferDesc.Width = width;
	mSwapChainDesc.BufferDesc.Height = height;
	//NOTE:4.30 pixel - wid,hei,depth(32 bit color)
	mSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//NOTE:4.30 R8G8B8A8: 8bit for R,G,B,A = 8*4 = 32
	mSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	mSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	mSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	mSwapChainDesc.OutputWindow = window;
	mSwapChainDesc.SampleDesc.Count = 1;
	mSwapChainDesc.SampleDesc.Quality = 0;
	mSwapChainDesc.Windowed = !fullscreen;

	const D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&mSwapChainDesc,
		&mSwapChain,
		&mD3DDevice,
		nullptr,
		&mImmediateContext	
	);

	ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to create device or swap chain");


	//cache swap chain desc
	mSwapChain->GetDesc(&mSwapChainDesc);

	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	sWindowMessageHandler.Hook(window, GraphicSystemMessageHandler);

	//cache wid , hei
	//mWidth = width;
	//mHeight = height;

	
}

void  GraphicSystem::BeginRender( )
{
	//const FLOAT clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void  GraphicSystem::EndRender()
{
	mSwapChain->Present(mVSync ?  1 : 0, 0);

}

void GraphicSystem::ClearDepthBuffer()
{
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void  GraphicSystem::ToggleFullscreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}
void GraphicSystem::Resize(uint32_t width, uint32_t height)
{
	mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to resize swap chain buffer");

		//cache swapchainDesc
		mSwapChain->GetDesc(&mSwapChainDesc);

	}

	//create a render target view
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to access swap chain buffer");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to create render target view");

	// create depth stencil texture
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = GetBackBufferWidth();
	depthBufferDesc.Height = GetBackBufferHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;

	//32bit depth , 24for depth, 8 for stencil
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&depthBufferDesc, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to create Depth Stencil Buffer");

	//create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = depthBufferDesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[Graphic::GraphicSystem] Failed to create Depth Stencil view");


	//
	//set render target view and depth stencil view
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//setup viewport
	mViewport.Width = static_cast<FLOAT>(GetBackBufferWidth());
	mViewport.Height = static_cast<FLOAT>(GetBackBufferHeight());
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &mViewport);


	//cache wid n hei
	//mWidth = GetBackBufferWidth();
	//mHeight = GetBackBufferHeight();
}

void H::Graphics::GraphicSystem::SetClearColor(Color color)
{
	mClearColor = color;
}

void H::Graphics::GraphicSystem::SetVSync(bool vsync)
{
	mVSync = vsync;
}

uint32_t GraphicSystem::GetBackBufferWidth() const
{
	return mSwapChainDesc.BufferDesc.Width;
}
uint32_t GraphicSystem::GetBackBufferHeight() const
{
	return mSwapChainDesc.BufferDesc.Height;
}

void H::Graphics::GraphicSystem::ResetRenderTarget()
{
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void H::Graphics::GraphicSystem::ResetViewport()
{
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicSystem::Terminate()
{
	sWindowMessageHandler.UnHook();

	SafeRelease(mD3DDevice);
	SafeRelease(mImmediateContext);
	SafeRelease(mSwapChain);
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mDepthStencilView);
}

 
