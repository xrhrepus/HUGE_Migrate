#ifndef HUGE_CORE_BLOCKALLOCATOR_H
#define HUGE_CORE_BLOCKALLOCATOR_H
namespace H::Core
{

class BlockAllocator
{
public:
	BlockAllocator(uint32_t blockSize,uint32_t capacity);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator&) = delete;
	BlockAllocator& operator=(const BlockAllocator&) = delete;

	BlockAllocator(  BlockAllocator&&) = delete;
	BlockAllocator& operator=(  BlockAllocator&&) = delete;


	void* Allocate();
	void Free(void* ptr);

	uint32_t GetCapacity() const;

private:
	uint8_t* mData;
	uint32_t mBlockSize;
	uint32_t mCapacity;
	std::vector<uint32_t> mFreeSlots;
	//intptr_t mfs
};

}//namespace H::Core

 
#endif // !HUGE_CORE_BLOCKALLOCATOR_H

