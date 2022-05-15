#ifndef HUGE_GRAPHICS_GRAPHICSYSTEM_H
#define HUGE_GRAPHICS_GRAPHICSYSTEM_H
// manage swap chain
#include "Colors.h"

namespace H {
namespace Graphics {
	class GraphicSystem
	{
	public:
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static GraphicSystem* Get();

		~GraphicSystem();

	public:
		GraphicSystem() = default;
		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender( );
		void EndRender();

		void ClearDepthBuffer();

		void ToggleFullscreen();
		void Resize(uint32_t width, uint32_t height);

		void SetClearColor(Color color); 
		void SetVSync(bool vsync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;

		void ResetRenderTarget();
		void ResetViewport();
 
	private:

		friend LRESULT CALLBACK GraphicSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		friend ID3D11Device* GetDevice();
		friend ID3D11DeviceContext* GetContext();

		Color mClearColor = Colors::DarkSlateGray;
		bool mVSync = true;

		ID3D11Device* mD3DDevice{nullptr}; // mem management
		ID3D11DeviceContext* mImmediateContext{ nullptr }; // draw call

		IDXGISwapChain* mSwapChain{ nullptr };
		ID3D11RenderTargetView* mRenderTargetView{ nullptr };

		ID3D11Texture2D* mDepthStencilBuffer{nullptr};
		ID3D11DepthStencilView* mDepthStencilView{nullptr};

		DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
		D3D11_VIEWPORT mViewport;

		//window wid height
		//uint32_t mWidth;
		//uint32_t mHeight;


	};




}// namespace GRAPHIC
}// namespace HUGE

#endif // !HUGE_GRAPHICS_GRAPHICSYSTEM_H

