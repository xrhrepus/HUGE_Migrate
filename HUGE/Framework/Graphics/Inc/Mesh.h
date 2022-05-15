#ifndef HUGE_GRAPHICS_MESH_H
#define HUGE_GRAPHICS_MESH_H

#include "VertexType.h"

namespace H::Graphics
{
 		template<class MeshType>
		struct MeshBase
		{
			using VertexType = MeshType;

			std::vector<MeshType> vertices;
			std::vector<uint32_t> indices;
		};

		using MeshPC = MeshBase<VertexPC>;
		using MeshPNC = MeshBase<VertexPNC>;

		using MeshPX = MeshBase<VertexPX>;//

		using MeshPNX = MeshBase<VertexPNX>;
		using MeshPN = MeshBase<VertexPN>;

		//standard mesh
		using Mesh = MeshBase<Vertex>;
		using BoneMesh = MeshBase<BoneVertex>;


}//  H graphics
 

#endif // !HUGE_GRAPHIC_MESH_H


