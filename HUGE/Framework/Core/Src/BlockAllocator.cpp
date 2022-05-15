#include "Precompiled.h"
#include "BlockAllocator.h"
#include "Debug.h"

H::Core::BlockAllocator::BlockAllocator(uint32_t blockSize, uint32_t capacity)
	:mBlockSize(blockSize),mCapacity(capacity)
{
	mData = (uint8_t*)malloc(mBlockSize * mCapacity);
	//mData = new uint8_t[mBlockSize * mCapacity];
	mFreeSlots.resize(mCapacity);
	for (size_t i = 0; i < mCapacity; ++i)
	{
		mFreeSlots[i] = i;
	}
}

H::Core::BlockAllocator::~BlockAllocator()
{
	if (mData != nullptr)
	{
		free(mData);
		//delete[] mData;
	}
}

void * H::Core::BlockAllocator::Allocate()
{
	if (mFreeSlots.empty())
 		return nullptr;
 
	void * p = mData + (mBlockSize * mFreeSlots.back());
	mFreeSlots.pop_back();
	return p;
}

void H::Core::BlockAllocator::Free(void * ptr)
{
	const ptrdiff_t pd = reinterpret_cast<ptrdiff_t>(ptr);
	const ptrdiff_t mdd = reinterpret_cast<ptrdiff_t>(mData);

	ASSERT((pd >= mdd 
		&& pd < mdd + mBlockSize * mCapacity 
		&& (pd - mdd) % mBlockSize == 0),
		"[H::Core::BlockAllocator] ptr is not valid");

	uint8_t* p = reinterpret_cast<uint8_t*> (ptr);

	*p = 0xfeeefeee;
	mFreeSlots.push_back((mdd - pd) / mBlockSize);
}

uint32_t H::Core::BlockAllocator::GetCapacity() const
{
	return mCapacity;
}
