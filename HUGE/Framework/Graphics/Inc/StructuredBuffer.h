#ifndef HUGE_GRAPHICS_STRUCTUREDBUFFER_H
#define HUGE_GRAPHICS_STRUCTUREDBUFFER_H


namespace H::Graphics {

	class StructuredBuffer
	{
	public:
		StructuredBuffer() = default;
		~StructuredBuffer();

		void Initialize(uint32_t typeSize, uint32_t numberOfElements, const void* initData = nullptr);
		void Terminate();
		void Set(const void* data) const;

		void BindPS(uint32_t slot = 0) const;
		void BindVS(uint32_t slot = 0) const;
		void BindGS(uint32_t slot = 0) const;

		void UnBindPS(uint32_t slot = 0) const;
		void UnBindVS(uint32_t slot = 0) const;
		void UnBindGS(uint32_t slot = 0) const;

	private:
		ID3D11Buffer* mStructuredBuffer = nullptr;
		ID3D11ShaderResourceView* mShaderResourceView{ nullptr };
	};

	template<class DataType, uint32_t numberOfElements>
	class TypedStructuredBuffer : public StructuredBuffer
	{
	public:
		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedStructuredBuffer]: datatype must be 16 byte aligned");
			StructuredBuffer::Initialize(sizeof(DataType), numberOfElements);
		}
		/*void Initialize(const DataType& data, uint32_t numberOfElements)
		{
			StructuredBuffer::Initialize(sizeof(DataType), numberOfElements, &data);
		}*/

		void Set(const DataType& data) const
		{
			StructuredBuffer::Set(&data);
		}


	};



}// HUGE GRAPHICS

#endif // !HUGE_GRAPHICS_STRUCTUREDBUFFER_H

