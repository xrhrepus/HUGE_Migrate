#ifndef HUGE_GRAPHIC_PIXELSHADER_H
#define HUGE_GRAPHIC_PIXELSHADER_H
#include <Graphics\Inc\VertexType.h>

namespace H::Graphics {


	class PixelShader
	{
	public:
		void Initialize();
		void Initialize(const wchar_t* filename, const char* entryFunction);

		void Terminate();
		void Bind() const;
		void UnBind()const ;

	private:
		ID3D11PixelShader* mPixelShader{ nullptr };
	};


 
}



#endif // !HUGE_GRAPHIC_PIXELSHADER_H
