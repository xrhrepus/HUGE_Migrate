#ifndef HUGE_GRAPHIC_GEOMETRYSHADER_H
#define HUGE_GRAPHIC_GEOMETRYSHADER_H
#include <Graphics\Inc\VertexType.h>
//===2019.10.14

namespace H {
	namespace Graphics {

		class GeometryShader
		{
		public:

			void Initialize( const wchar_t* filename);
			void Terminate();
			void Bind();
			void UnBind();

		private:
			ID3D11GeometryShader* mGeometryShader = nullptr;
 		};

 
	}
}
 
#endif // !GRAPHIC_VERTEXSHADER_H

