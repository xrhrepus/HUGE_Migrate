#ifndef HUGE_GRAPHIC_CONSTANTBUFFER_H
#define HUGE_GRAPHIC_CONSTANTBUFFER_H

namespace H {
namespace Graphics {

	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		ConstantBuffer(ConstantBuffer&& rhs);
		ConstantBuffer(const ConstantBuffer& rhs) = delete;
		~ConstantBuffer();

		ConstantBuffer& operator=(const ConstantBuffer& rhs) = delete;
		ConstantBuffer& operator=(ConstantBuffer&& rhs);

		void Initialize(uint32_t buffersize, bool dynamic = false, const void* initData = nullptr);
		void Terminate();
		void Set(const void* data) const;
		void Map(const void* data, uint32_t byteToMap) const;

		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void BindGS(uint32_t slot = 0) const;

		void UnBindVS(uint32_t slot = 0) const;
		void UnBindPS(uint32_t slot = 0) const;
		void UnBindGS(uint32_t slot = 0) const;

	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
		bool mIsDynamic = false;
	};

	template<class DataType>
	class TypedConstantBuffer : public ConstantBuffer
	{
	public:
		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer]: datatype must be 16 byte aligned");
			ConstantBuffer::Initialize(sizeof(DataType));
		}
		void Initialize(const DataType& data)
		{
			ConstantBuffer::Initialize(sizeof(DataType), false, &data);
		}

		void Set(const DataType& data) const
		{
			ConstantBuffer::Set(&data);
		}
	};

	template<class DataType>
	class TypedDynamicConstantBuffer : public ConstantBuffer
	{
	public:
		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedDynamicConstantBuffer]: datatype must be 16 byte aligned");
			ConstantBuffer::Initialize(sizeof(DataType), true);
		}
		void Initialize(const DataType& data)
		{
			ConstantBuffer::Initialize(sizeof(DataType), true, &data);
		}

		void Set(const DataType&) const
		{
			ASSERT(false, "[TypedDynamicConstantBuffer]: dynamic buffer should use Map/UnMap");
		}

		void Map(const DataType& data, uint32_t byteToMap) const
		{
			ConstantBuffer::Map(&data, byteToMap);
		}
	};

}
}


#endif // !HUGE_GRAPHIC_CONSTANTBUFFER_H
