#ifndef HUGE_GRAPHIC_MESHBUFFER_H
#define HUGE_GRAPHIC_MESHBUFFER_H
#include <Graphics\Inc\VertexType.h>

namespace H {
namespace Graphics {

	class MeshBuffer
	{
	public:
		enum class Topology {Points,Lines,Triangles};

		//void Initialize(const Vertex vertices[],size_t NumOfVert);
		template<class MeshType>
		void Initialize(const MeshType& mesh, bool dynamic = false);
		//{
		//	Initialize(mesh.vertices.data(), mesh.vertices.size(), sizeof(VertexPC), mesh.indices.data(), mesh.indices.size());
		//}

		void Initialize(const void* vertexData, uint32_t NumOfVert, uint32_t vertexSize,bool dynamic = false);
		void Initialize(const void* vertexData, uint32_t NumOfVert,uint32_t vertexSize,const uint32_t* indexData,uint32_t numOfIndex, bool dynamic = false);
		void Terminate();

		void SetTopology(Topology topo);

		void Update(const void* vertexData, uint32_t numOfVert);
		void Update(const void* vertexData, uint32_t numOfVert, const void* indexData, uint32_t numOfIndex);

		void Render() const;//render const

	private:
		ID3D11Buffer* mVertexBuffer{ nullptr };//main verts
											   //need indexBuffer
		ID3D11Buffer*mIndexBuffer{ nullptr };

		uint32_t mVertexSize{ 0 };
		uint32_t mVertexCount{ 0 };
		uint32_t mIndexCount{ 0 };
		//vert size/count index size

		void InitVertexBuffer(const void* vertexData, uint32_t NumOfVert, uint32_t vertexSize, bool dynamic);
		void InitIndexBuffer( const uint32_t* indexData, uint32_t numOfIndex, bool dynamic);
		D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	};

	template<class MeshType>
	void MeshBuffer::Initialize(const MeshType& mesh, bool dynamic)
	{
		Initialize(
			mesh.vertices.data(),
			static_cast<uint32_t>(mesh.vertices.size()),
			sizeof(MeshType::VertexType),
			mesh.indices.data(), 
			static_cast<uint32_t>(mesh.indices.size()),
			dynamic);
	}


}
}

#endif // !HUGE_GRAPHIC_MESHBUFFER_H
