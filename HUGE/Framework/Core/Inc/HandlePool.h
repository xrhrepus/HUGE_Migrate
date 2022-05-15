#ifndef HUGE_CORE_HANDLEPOOL_H
#define HUGE_CORE_HANDLEPOOL_H

namespace H::Core
{
 	template<class DataType>
	class Handle;

 	template<class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;

		HandlePool(uint32_t capacity);
		~HandlePool();

		HandleType Register(DataType* instance);
		void UnRegister(HandleType handle);
		void Flush();
		bool IsValid(HandleType handle)const;
		DataType* Get(HandleType handle);


	private:
		struct Entry
		{
			DataType* instance = nullptr;
			uint32_t generation = 0;
		};
		
		std::vector<Entry> mEntries;
		std::vector<uint32_t> mFreeSlots;
 	};

	template<class DataType>
	inline HandlePool<DataType>::HandlePool(uint32_t capacity)
	{
		ASSERT(capacity > 0, "[HandlePool] invalide capacity");
		// slot 0 - invalid
		mEntries.resize(capacity + 1);
		mFreeSlots.reserve(capacity);
		for (size_t i = capacity; i > 0; --i)
			mFreeSlots.push_back(i);

		ASSERT(Handle<DataType>::sPool == nullptr, "[HandlePool] this type already exist");

		Handle<DataType>::sPool = this;

	}

	template<class DataType>
	inline HandlePool<DataType>::~HandlePool()
	{
		ASSERT(mFreeSlots.capacity() == mFreeSlots.size(), "[HandlePool] still handle not unregistered");

		ASSERT(Handle<DataType>::sPool == this, "[HandlePool] error");
		Handle<DataType>::sPool = nullptr;
	}

	template<class DataType>
	inline Handle<DataType> HandlePool<DataType>::Register(DataType * instance)
	{
		const uint32_t freeslot = mFreeSlots.back();
		mFreeSlots.pop_back();
		mEntries[freeslot].instance = instance;
		Handle<DataType> hd;
		hd.sPool = this;
		hd.mGeneration = (mEntries[freeslot].generation);
		hd.mIndex = freeslot;
		return hd;
	}

	template<class DataType>
	inline void HandlePool<DataType>::UnRegister(HandleType handle)
	{
		if (IsValid(handle))
		{
			const uint32_t idx = handle.mIndex;
			mEntries[idx].generation++;
 			mFreeSlots.push_back(idx);
		}
	}

	template<class DataType>
	inline void HandlePool<DataType>::Flush()
	{
		for (auto& e : mEntries)
		{
			e.generation++;
		}
		mFreeSlots.clear();
		for (size_t i = mFreeSlots.capacity(); i > 0; --i)
			mFreeSlots.push_back(i);
	}

	template<class DataType>
	inline bool HandlePool<DataType>::IsValid(HandleType handle) const
	{
		return (handle.mIndex != 0) && (handle.mGeneration == mEntries[handle.mIndex].generation);
	}

	template<class DataType>
	inline DataType * HandlePool<DataType>::Get(HandleType handle)
	{
		if (IsValid(handle))
		{
			return mEntries[handle.mIndex].instance;
		}
		return nullptr;
	}

}//namespace H::Core


#endif // !HUGE_CORE_HANDLEPOOL_H

