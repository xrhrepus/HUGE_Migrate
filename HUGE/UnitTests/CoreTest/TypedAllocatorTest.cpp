#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Core;

namespace CoreTest
{
	TEST_CLASS(TypedAllocatorTest)
	{
	public:

		class Bar
		{
		public:
			static int count;

			Bar() { count = 0; }
			Bar(const Bar&) { count = 1; }
			Bar(Bar&&) { count = 2; }

		};
		class Foo
		{
		public:
			int a, b;
			Foo() :a(42), b(7) {}
			Foo(int q, int w) : a(q), b(w) {}
			Foo(Bar b) :a(123), b(456) {}
			Foo(std::string syr) :a(987), b(654) {}
			~Foo() { a = 0xfeeefeee; b = 0xadeafbee; }

		private:

		};
 
		TEST_METHOD(TestNew)
		{
			Foo f("asdf");
			H::Core::TypedAllocator<Foo> tAlloc1(1);
			Foo* ptr = tAlloc1.New();
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->a = 42);
			Assert::IsTrue(ptr->b = 7);

		}

		TEST_METHOD(TestForwarding)
		{
			TypedAllocator<Foo> tAlloc1(1);
			
			Foo* ptr = tAlloc1.New(Bar());

			Assert::IsNotNull(ptr);
			Assert::AreEqual(Bar::count,2);

		}

		TEST_METHOD(TestDelete)
		{
			TypedAllocator<Foo> tAlloc1(1);
			Foo* ptr = tAlloc1.New();
			Assert::IsNotNull(ptr);

			tAlloc1.Delete(ptr);
			Assert::AreEqual((unsigned int)ptr->a,0xfeeefeee);
			Assert::AreEqual((unsigned int)ptr->b, 0xadeafbee);



		}
	};
	int TypedAllocatorTest::Bar::count = 0;
}