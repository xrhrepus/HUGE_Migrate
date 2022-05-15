#ifndef HUGE_CORE_HANDLE_H
#define HUGE_CORE_HANDLE_H

namespace H::Core
{
	template<class DataType>
	class HandlePool;

	template<class DataType>
	class Handle
	{
	public:
		Handle();
		bool IsValid()const;
		void Invalidate();
		DataType* Get()const;
		DataType* operator->() const;

		bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
		bool operator!=(Handle rhs) const {}

	private:
		friend class HandlePool<DataType>;
		static HandlePool<DataType>* sPool;


		//bit packing?
		uint32_t mIndex : 24;
		uint32_t mGeneration : 8;

	};
	template<class DataType>
	HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

	template<class DataType>
	inline Handle<DataType>::Handle()
		:mIndex(0),mGeneration(0)
	{
	}

	template<class DataType>
	inline bool Handle<DataType>::IsValid() const
	{
		return sPool && sPool->IsValid(*this);
	}

	template<class DataType>
	inline void Handle<DataType>::Invalidate()
	{
		*this = Handle();
	}

	template<class DataType>
	inline DataType * Handle<DataType>::Get() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

	template<class DataType>
	inline DataType * Handle<DataType>::operator->() const
	{
		return sPool ? sPool->Get(*this) : nullptr;
	}

}

#endif // !HUGE_CORE_HANDLE_H

