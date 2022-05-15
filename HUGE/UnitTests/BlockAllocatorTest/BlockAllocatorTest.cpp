#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BlockAllocatorTest
{
	TEST_CLASS(BlockAllocatorTest)
	{
	public:
		
		TEST_METHOD(oneblocktest)
		{
			H::Core::BlockAllocator blockAlloc(16,1);
			void* ptr = blockAlloc.Allocate();
			Assert::IsNotNull(ptr);
		}
		TEST_METHOD(twoblocktest)
		{
			H::Core::BlockAllocator blockAlloc(16, 2);
			void* ptr1 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr1);
			void* ptr2 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr2);
			Assert::IsFalse(ptr1 == ptr2);
			void* ptr3 = blockAlloc.Allocate();
			Assert::IsNull(ptr3);		}
		TEST_METHOD(freetest)
		{
			H::Core::BlockAllocator blockAlloc(16, 1);
			void* ptr1 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr1);
			void* ptr2 = blockAlloc.Allocate();
			Assert::IsNull(ptr2);
			blockAlloc.Free(ptr1);
			ptr2 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr1 == ptr2);
		}
		TEST_METHOD(freetest2)
		{
			H::Core::BlockAllocator blockAlloc(16, 1);
			void* ptr1 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr1);
			void* ptr2 = blockAlloc.Allocate();
			Assert::IsNull(ptr2);

			int* ptr365 = new int[3];
			blockAlloc.Free(ptr365);

			blockAlloc.Free(ptr1);
			ptr2 = blockAlloc.Allocate();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr1 == ptr2);
		}


	};
}
