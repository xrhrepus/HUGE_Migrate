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

		void Initialize(uint32_t buffersize, const void* initData = nullptr);
		void Terminate();
		void Set(const void* data) const;

		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void BindGS(uint32_t slot = 0) const;

		void UnBindVS(uint32_t slot = 0) const;
		void UnBindPS(uint32_t slot = 0) const;
		void UnBindGS(uint32_t slot = 0) const;

	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
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
			ConstantBuffer::Initialize(sizeof(DataType),&data);
		}

		void Set(const DataType& data) const
		{
			ConstantBuffer::Set(&data);
		}


	};


}
}


#endif // !HUGE_GRAPHIC_CONSTANTBUFFER_H
