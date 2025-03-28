#include "Precompiled.h"
#include "ConstantBuffer.h"
#include "D3DUtil.h"

using namespace H::Graphics;
using namespace H::Math;

H::Graphics::ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "[ConstantBuffer] buffer not released ");
}
ConstantBuffer::ConstantBuffer(ConstantBuffer&& rhs)
	:mConstantBuffer(rhs.mConstantBuffer)
{
	 rhs.mConstantBuffer = nullptr;
}

ConstantBuffer& ConstantBuffer::operator=(ConstantBuffer&& rhs) {
	rhs.mConstantBuffer = mConstantBuffer;
	mConstantBuffer = nullptr;
	return *this;
}


void ConstantBuffer::Initialize(uint32_t buffersize, bool dynamic, const void* initData)
{
	//=============
	mIsDynamic = dynamic;
	//constant buffer ->(compute shader) -> vertex
	D3D11_BUFFER_DESC cdesc{};
	cdesc.ByteWidth = buffersize;
	cdesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	cdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subResData{};
	subResData.pSysMem = initData;

	GetDevice()->CreateBuffer(&cdesc, initData? &subResData : nullptr, &mConstantBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void  ConstantBuffer::Set(const void* data) const
{
	GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);//download data to VRAM
}

void H::Graphics::ConstantBuffer::Map(const void* data, uint32_t byteToMap) const
{
	// same as in StructuredBuffer
	ASSERT(mIsDynamic, "[TypedDynamicConstantBuffer]:non-dynamic buffer should use Set");
	const bool validDataToCopy = data != nullptr || byteToMap == 0;
	ASSERT(validDataToCopy, "[TypedDynamicConstantBuffer]: invalid data to set");
	if (!validDataToCopy)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = GetContext()->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ASSERT(SUCCEEDED(hr), "[TypedDynamicConstantBuffer]: failed to update buffer");
	if (SUCCEEDED(hr))
	{
		// Copy data into the mapped memory
		memcpy(mappedResource.pData, data, byteToMap);

		// Unmap the buffer to apply changes
		GetContext()->Unmap(mConstantBuffer, 0);
	}
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	GetContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	GetContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}
void ConstantBuffer::BindGS(uint32_t slot) const
{
	GetContext()->GSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::UnBindVS(uint32_t slot) const
{
	GetContext()->VSSetConstantBuffers(slot, 0, nullptr);
}

void ConstantBuffer::UnBindPS(uint32_t slot) const
{
	GetContext()->PSSetConstantBuffers(slot, 0, nullptr);
}
void ConstantBuffer::UnBindGS(uint32_t slot) const
{
	GetContext()->GSSetConstantBuffers(slot, 0, nullptr);
 }
