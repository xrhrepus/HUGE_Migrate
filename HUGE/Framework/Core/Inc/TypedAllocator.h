#ifndef HUGE_CORE_TYPEDALLOCATOR_H
#define HUGE_CORE_TYPEDALLOCATOR_H
#include "BlockAllocator.h"

namespace H::Core
{

template <class DataType>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(uint32_t capacity);
		
	//~TypedAllocator();

	template <class... Args>
	DataType* New(Args&&... args);
 

	void Delete(DataType* ptr);

private:

};
 
template<class DataType>
inline TypedAllocator<DataType>::TypedAllocator(uint32_t capacity)
	:BlockAllocator(sizeof(DataType), capacity)
{}

template<class DataType>
template<class ...Args>
inline DataType * TypedAllocator<DataType>::New(Args && ...args)
{
	//void* mem = Allocate();
	//if (mem != nullptr)
	//{
	//	new (mem) DataType(args);
	//}
	//return reinterpret_cast<DataType*>(mem);
	DataType* instance = static_cast<DataType*>(BlockAllocator::Allocate());
	if (instance)
	{
		new (instance) DataType(std::forward<Args>(args)...);
	}
	return instance;
}

template<class DataType>
inline void TypedAllocator<DataType>::Delete(DataType * ptr)
{
	if (ptr == nullptr)
	{
		return;
	}
	ptr->~DataType();
	BlockAllocator::Free(ptr);
}

}
 
#endif // !HUGE_CORE_TYPEDALLOCATOR_H


