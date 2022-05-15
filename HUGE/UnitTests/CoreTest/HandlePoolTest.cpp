#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Core;

namespace CoreTest
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
 
 		TEST_METHOD(test_DefaultHandle)
		{
			Handle<Zoo> zh;
			Assert::IsFalse(zh.IsValid());
			Assert::IsNull(zh.Get());

			HandlePool<Zoo> handlePool2(1);
			Assert::IsFalse(handlePool2.IsValid(zh));
			handlePool2.Flush();

		}
		TEST_METHOD(test_Register)
		{
 			 HandlePool<Zoo> handlePool2(1);

			 Handle<Zoo> fake;
			Assert::IsFalse(handlePool2.IsValid(fake));

			Zoo z;
			 Handle zh = handlePool2.Register(&z);
 			Assert::IsTrue( (zh.Get())->a == 4);

			Assert::IsTrue(zh.IsValid());
			Assert::IsTrue(&z == handlePool2.Get(zh));

			Assert::IsTrue(handlePool2.IsValid(zh));
			Assert::IsTrue(&z == zh.Get());
			handlePool2.Flush();

		}
		TEST_METHOD(test_HandleInvalidate)
		{
			HandlePool<Zoo> handlePool(1);
			Zoo z;
			Handle<Zoo> zh = handlePool.Register(&z);

			zh.Invalidate();

			Assert::IsFalse(handlePool.IsValid(zh));
			Assert::IsNull(handlePool.Get(zh));
			Assert::IsFalse(zh.IsValid());
			Assert::IsNull(zh.Get());

			handlePool.Flush();
		}
		TEST_METHOD(test_HandleCopyGet)
		{
			HandlePool<Zoo> handlePool(1);
			Zoo z;
			Handle<Zoo> zh = handlePool.Register(&z);
			Handle<Zoo> zh2 = zh;

			Assert::IsTrue(zh.IsValid());
			Assert::IsTrue(zh.Get() == &z);
			handlePool.Flush();

		}
		TEST_METHOD(test_HandleCopyInvalidate)
		{
			HandlePool<Zoo> handlePool(1);
			Zoo z;
			Handle<Zoo> zh = handlePool.Register(&z);
			Handle<Zoo> zh2 = zh;

			zh.Invalidate();

 			Assert::IsTrue(handlePool.IsValid(zh2));
			Assert::IsNotNull(handlePool.Get(zh2));
			Assert::IsTrue(zh2.IsValid());
			Assert::IsNotNull(zh2.Get());
			handlePool.Flush();

		}

		TEST_METHOD(test_UnRegister)
		{
			HandlePool<Zoo> handlePool(1);
			Zoo z;
			Handle<Zoo> zh = handlePool.Register(&z);

			handlePool.UnRegister(zh);
			
			Assert::IsNull(handlePool.Get(zh));
			Assert::IsFalse(zh.IsValid());
			Assert::IsFalse(handlePool.IsValid(zh));
			handlePool.Flush();

		}

		TEST_METHOD(test_CopyUnRegister)
		{
			HandlePool<Zoo> handlePool(1);
			Zoo z;
			Handle<Zoo> zh = handlePool.Register(&z);
			Handle<Zoo> zh2 = zh;

			handlePool.UnRegister(zh2);

			Assert::IsNull(handlePool.Get(zh));
			Assert::IsFalse(zh.IsValid());
			Assert::IsFalse(handlePool.IsValid(zh));

			Assert::IsNull(handlePool.Get(zh2));
			Assert::IsFalse(zh2.IsValid());
			Assert::IsFalse(handlePool.IsValid(zh2));
			handlePool.Flush();

 		}
	};
}