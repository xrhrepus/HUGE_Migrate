#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Core;

namespace HandlePoolTest
{		
	TEST_CLASS(HandlePoolTest)
	{
	public:
 		class Zoo
		{
		public:
			int a = 0;
			Zoo() : a(4){}
			~Zoo() { a = 31; }

		};


 		TEST_METHOD(test_constructor)
		{
			H::Core::HandlePool<int> handlePool(8);
			H::Core::HandlePool<Zoo> handlePool2(99);

 		}

		TEST_METHOD(test_DefaultHandle)
		{
			Handle<Zoo> zh;
			Assert::IsFalse(zh.IsValid());
		}

		TEST_METHOD(test_Register)
		{
			H::Core::HandlePool<int> handlePool(8);

			H::Core::Handle h = handlePool.Register(new int(314159));

			Assert::IsTrue(*(h.Get()) == 314159);

			H::Core::HandlePool<Zoo> handlePool2(1);
			Zoo z;
			H::Core::Handle zh = handlePool2.Register(&z);
			Assert::IsTrue(zh.IsValid());

 			Assert::IsTrue( (zh.Get())->a == 4);

		}

		TEST_METHOD(test_Register)
		{
			H::Core::HandlePool<int> handlePool(8);

			H::Core::Handle h = handlePool.Register(new int(314159));

			Assert::IsTrue(*(h.Get()) == 314159);

			H::Core::HandlePool<Zoo> handlePool2(1);
			Zoo z;
			H::Core::Handle zh = handlePool2.Register(&z);
			Assert::IsTrue(zh.IsValid());

			Assert::IsTrue((zh.Get())->a == 4);

		}



		TEST_METHOD(test_IsValid)
		{
			H::Core::HandlePool<int> handlePool(8);
			H::Core::Handle h = handlePool.Register(new int(314159));
			Assert::IsTrue(*(h.Get()) == 314159);

			H::Core::Handle<int> h2;
			Assert::IsTrue(handlePool.IsValid(h));
			Assert::IsFalse(handlePool.IsValid(h2));

		}
		TEST_METHOD(test_UnRegister)
		{
			H::Core::HandlePool<int> handlePool(8);
			H::Core::Handle h = handlePool.Register(new int(314159));
			Assert::IsTrue(*(h.Get()) == 314159);

			handlePool.UnRegister(h);

			Assert::IsTrue((h.Get()) == nullptr);

		}

	};
}