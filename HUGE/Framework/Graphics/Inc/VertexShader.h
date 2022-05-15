#ifndef HUGE_GRAPHIC_VERTEXSHADER_H
#define HUGE_GRAPHIC_VERTEXSHADER_H
#include <Graphics\Inc\VertexType.h>

namespace H {
namespace Graphics {

	class VertexShader
	{
	public:

		//if vertex format = 0, use reflection
		void Initialize(uint32_t vertexformat, const wchar_t* filename);
		void Initialize(const wchar_t* filename);

		void Terminate();
		void Bind() const;
		void UnBind() const;

	private:
		
		ID3D11VertexShader* mVertexShader{ nullptr };
		ID3D11InputLayout* mInputLayout{ nullptr };
	};

	//clean up winmain , no d3d11 
	//add vertex/pixel shader V
	//add vertexTypy.h move vertex  V
	//add meshbuffer class  	ID3D11VertexShader* vertexShader = nullptr;

	//add new proj 03_helloshapes
		//draw heart,triforce,more than 5 tris
}
}
//const D3D11_INPUT_ELEMENT_DESC elementDescs[] =
//{
//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
//	{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
//};

#endif // !GRAPHIC_VERTEXSHADER_H

