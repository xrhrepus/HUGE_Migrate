#include "Precompiled.h"
#include "StructuredBuffer.h"
#include "D3DUtil.h"

using namespace H::Graphics;
using namespace H::Math;

H::Graphics::StructuredBuffer::~StructuredBuffer()
{
	ASSERT(mShaderResourceView == nullptr, "[StructuredBuffer] buffer not released ");
	ASSERT(mStructuredBuffer == nullptr, "[StructuredBuffer] buffer not released ");
}

void StructuredBuffer::Initialize(uint32_t typeSize, uint32_t numberOfElements, bool dynamic, const void* initData)
{
	mIsDynamic = dynamic;
	//=============
//constant buffer ->(compute shader) -> vertex
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = typeSize * numberOfElements;
	desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	//?
	desc.StructureByteStride = typeSize;
	//?
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA subResData{};
	subResData.pSysMem = initData;

	HRESULT hr = GetDevice()->CreateBuffer(&desc, initData ? &subResData : nullptr, &mStructuredBuffer);
	ASSERT(SUCCEEDED(hr), "");

	//?
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // StructuredBuffer doesn't have a format
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = numberOfElements;

	// Create SRV
	HRESULT hr2 = GetDevice()->CreateShaderResourceView(mStructuredBuffer, &srvDesc, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr2), "");
}

void StructuredBuffer::Terminate()
{
	SafeRelease(mShaderResourceView);
	SafeRelease(mStructuredBuffer);
}

void StructuredBuffer::Set(const void* data) const
{
	ASSERT(!mIsDynamic, "[StructuredBuffer] dynamic buffer should use Map/UnMap");
	GetContext()->UpdateSubresource(mStructuredBuffer, 0, nullptr, data, 0, 0);
}

void StructuredBuffer::Map(const void* data, uint32_t byteToMap) const
{
	ASSERT(mIsDynamic, "[StructuredBuffer] non-dynamic buffer should use Set");
	const bool validDataToCopy = data != nullptr || byteToMap == 0;
	ASSERT(validDataToCopy, "[StructuredBuffer] invalid data to set");
	if (!validDataToCopy)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = GetContext()->Map(mStructuredBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ASSERT(SUCCEEDED(hr), "[StructuredBuffer] failed to update buffer");
	if (SUCCEEDED(hr))
	{
		// Copy data into the mapped memory
		memcpy(mappedResource.pData, data, byteToMap);

		// Unmap the buffer to apply changes
		GetContext()->Unmap(mStructuredBuffer, 0);
	}
}

void StructuredBuffer::BindVS(uint32_t slot) const
{
	GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void StructuredBuffer::BindPS(uint32_t slot) const
{
	GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}
void StructuredBuffer::BindGS(uint32_t slot) const
{
	GetContext()->GSSetShaderResources(slot, 1, &mShaderResourceView);
}

void StructuredBuffer::UnBindVS(uint32_t slot) const
{
	GetContext()->VSSetShaderResources(slot, 0, nullptr);
}

void StructuredBuffer::UnBindPS(uint32_t slot) const
{
	GetContext()->PSSetShaderResources(slot, 0, nullptr);
}
void StructuredBuffer::UnBindGS(uint32_t slot) const
{
	GetContext()->GSSetShaderResources(slot, 0, nullptr);
 }
