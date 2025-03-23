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


void ConstantBuffer::Initialize(uint32_t buffersize, const void* initData)
{
	//=============
	//constant buffer ->(compute shader) -> vertex
	D3D11_BUFFER_DESC cdesc{};
	cdesc.ByteWidth = buffersize;
	cdesc.Usage = D3D11_USAGE_DEFAULT;
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
