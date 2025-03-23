#ifndef HUGE_GRAPHICS_STRUCTUREDBUFFER_H
#define HUGE_GRAPHICS_STRUCTUREDBUFFER_H


namespace H::Graphics {

	class StructuredBuffer
	{
	public:
		StructuredBuffer() = default;
		~StructuredBuffer();

		void Initialize(uint32_t typeSize, uint32_t numberOfElements, bool dynamic = false, const void* initData = nullptr);
		void Terminate();
		void Set(const void* data) const;
		void Map(const void* data, uint32_t byteToMap) const;

		void BindPS(uint32_t slot = 0) const;
		void BindVS(uint32_t slot = 0) const;
		void BindGS(uint32_t slot = 0) const;

		void UnBindPS(uint32_t slot = 0) const;
		void UnBindVS(uint32_t slot = 0) const;
		void UnBindGS(uint32_t slot = 0) const;

	private:
		ID3D11Buffer* mStructuredBuffer{ nullptr };
		ID3D11ShaderResourceView* mShaderResourceView{ nullptr };
		bool mIsDynamic = false;
	};

	template<class DataType, uint32_t numberOfElements>
	class TypedStructuredBuffer : public StructuredBuffer
	{
	public:
		void Initialize()
		{
			// StructuredBuffer doesn't have to be 16 Byte aligned
			StructuredBuffer::Initialize(sizeof(DataType), numberOfElements);
		}

		void Set(const DataType& data) const
		{
			StructuredBuffer::Set(&data);
		}
	};

	template<class DataType, uint32_t numberOfElements>
	class TypedDynamicStructuredBuffer : public StructuredBuffer
	{
	public:
		void Initialize()
		{
			// StructuredBuffer doesn't have to be 16 Byte aligned
			StructuredBuffer::Initialize(sizeof(DataType), numberOfElements, true);
		}

		void Set(const DataType& data, uint32_t byteToMap) const
		{
			StructuredBuffer::Map(&data, byteToMap);
		}
	};



}// HUGE GRAPHICS

#endif // !HUGE_GRAPHICS_STRUCTUREDBUFFER_H

