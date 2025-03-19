#include "Precompiled.h"
 #include "MeshBuffer.h"
#include "D3DUtil.h"

using namespace H::Graphics;


void MeshBuffer::Initialize(const void* vertexData, uint32_t NumOfVert, uint32_t vertexSize, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = NumOfVert;
	InitVertexBuffer(vertexData, NumOfVert, vertexSize, dynamic);
}


void MeshBuffer::Initialize(const void* vertexData, uint32_t NumOfVert, uint32_t vertexSize, const uint32_t* indexData, uint32_t numOfIndex, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = NumOfVert;
	mIndexCount = numOfIndex;;
	InitVertexBuffer(vertexData,NumOfVert,vertexSize, dynamic);
	InitIndexBuffer(indexData,numOfIndex, dynamic);
}
void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}
void MeshBuffer::SetTopology(Topology topo)
{
	switch (topo)
	{
	case H::Graphics::MeshBuffer::Topology::Points:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case H::Graphics::MeshBuffer::Topology::Lines:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case H::Graphics::MeshBuffer::Topology::Triangles:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	default:
		break;
	}
}


void MeshBuffer::Update(const void* vertexData, uint32_t numOfVert)
{
	mVertexCount = numOfVert;

	auto context = GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numOfVert * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}
void MeshBuffer::Update(const void* vertexData, uint32_t numOfVert, const void* indexData, uint32_t numOfIndex)
{
	mVertexCount = numOfVert;
	mIndexCount = numOfIndex;

	auto context = GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numOfVert * mVertexSize);
	context->Unmap(mVertexBuffer, 0);

	D3D11_MAPPED_SUBRESOURCE index_resource;
	context->Map(mIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &index_resource);
	memcpy(index_resource.pData, indexData, numOfIndex * sizeof(uint32_t));
	context->Unmap(mIndexBuffer, 0);
}

void MeshBuffer::Render() const//to fix
{
	UINT stride = mVertexSize;
	UINT offset = 0;
	auto context = GetContext();
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(mTopology);



	if (mIndexBuffer!=nullptr)
	{
		context->IASetIndexBuffer(mIndexBuffer,DXGI_FORMAT_R32_UINT , 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		context->Draw(mVertexCount, 0);
	}
}
void MeshBuffer::RenderInstanced(UINT instanceCount) const
{
	UINT stride = mVertexSize;
	UINT offset = 0;
	auto context = GetContext();
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(mTopology);



	if (mIndexBuffer != nullptr)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexedInstanced(mIndexCount, instanceCount, 0, 0, 0);
	}
	else
	{
		//todo d instanced
		context->Draw(mVertexCount, 0);
	}
}
void MeshBuffer::InitVertexBuffer(const void* vertexData, uint32_t NumOfVert, uint32_t vertexSize, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = NumOfVert;

	//download data  from RAM into VRAM( allocate MEM in VRAM for vertexBuf and inited vertices)
	D3D11_BUFFER_DESC vertexBufferDESC{};
	vertexBufferDESC.ByteWidth = NumOfVert * vertexSize;
	vertexBufferDESC.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDESC.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDESC.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDESC.MiscFlags = 0;
	vertexBufferDESC.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertexData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
 
 	 
	HRESULT hr = GetDevice()->CreateBuffer(&vertexBufferDESC, vertexData ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "[MeshBuffer] Failed to create vertexBuffer.");

 

}
void MeshBuffer::InitIndexBuffer( const uint32_t* indexData, uint32_t numOfIndex, bool dynamic)
{
	mIndexCount = numOfIndex;

	//download data  from RAM into VRAM( allocate MEM in VRAM for vertexBuf and inited vertices)
	D3D11_BUFFER_DESC indexBufferDESC{};
	indexBufferDESC.ByteWidth = numOfIndex * sizeof(uint32_t);
	//indexBufferDESC.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDESC.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	indexBufferDESC.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDESC.CPUAccessFlags = 0;
	indexBufferDESC.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

	indexBufferDESC.MiscFlags = 0;
	indexBufferDESC.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indexData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
 
  	HRESULT hr = GetDevice()->CreateBuffer(&indexBufferDESC, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "[MeshBuffer] Failed to create indexBuffer.");

 
}
