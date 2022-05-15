#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Core;

class Car
{
public:
	META_CLASS_DECLARE;

	void Move() { mPosition += 1.0f; }

protected:
	float mPosition = 0.0f;
	bool mAutomatic = true;
};
 
class Tesla : public Car
{
public:
	META_CLASS_DECLARE;

	void Move() { mPosition += 10.0f; }

private:
	bool mGPS = true;
};

META_CLASS_BEGIN(Car)
	META_FIELD_BEGIN
		META_FIELD(mPosition,"Position")
		META_FIELD(mAutomatic, "Automatic")
	META_FIELD_END
META_CLASS_END

META_DERIVED_BEGIN(Tesla, Car)
	META_FIELD_BEGIN
		META_FIELD(mGPS, "GPS")
	META_FIELD_END
META_CLASS_END


 


namespace CoreTest
{
	TEST_CLASS(MetaTest)
	{
	public:
		TEST_METHOD(test_MetaType)
		{
			auto metaType = Meta::DeduceType<int>();
			Assert::AreEqual(metaType->GetName(), "Integer");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Primitive);
			Assert::IsTrue(metaType->GetSize() == sizeof(int));

		}

		TEST_METHOD(test_MetaPointer)
		{
			int* ptr = nullptr;
			auto metaType = Meta::DeduceType<int*>();

			Assert::AreEqual(metaType->GetName(), "Pointer");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Pointer);
			Assert::IsTrue(metaType->GetSize() == sizeof(int*));

			auto metaPtr = static_cast<const Meta::MetaPointer*>(metaType);
			auto metaPtrType = metaPtr->GetPointerType();

			Assert::AreEqual(metaPtrType->GetName(), "Integer");
			Assert::IsTrue(metaPtrType->GetCategory() == Meta::MetaType::Category::Primitive);
			Assert::IsTrue(metaPtrType->GetSize() == sizeof(int*));



		}

		TEST_METHOD(test_MetaArray)
		{
			using IntVec = std::vector<int>;
			auto metaType = Meta::DeduceType<IntVec>();

			Assert::AreEqual(metaType->GetName(), "Array");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Array);
			Assert::IsTrue(metaType->GetSize() == sizeof(IntVec));

			IntVec myInts;
			myInts.push_back(66);
			myInts.push_back(1234);
			myInts.push_back(9);

			auto metaArr = static_cast<const Meta::MetaArray*>(metaType);
			Assert::IsTrue(metaArr->GetElementType() == Meta::DeduceType<int>());
			for (size_t i = 0; i < metaArr->GetCount(&myInts); i++)
			{
				Assert::IsTrue(*(int*)metaArr->GetElement(&myInts, i) == myInts[i]);

			}
		}
		TEST_METHOD(test_MetaClass)
		{
			auto metaType = Meta::DeduceType<Car>();
			Assert::AreEqual(metaType->GetName(), "Car");
			Assert::IsTrue(metaType->GetCategory() == Meta::MetaType::Category::Class);
			Assert::IsTrue(metaType->GetSize() == sizeof(Car));

			auto metaClass = Car::StaticGetMetaClass();
			Assert::IsNull(metaClass->GetParent());
			Assert::IsTrue(metaClass->GetFieldsCount() == 2);

			auto metaField0 = metaClass->GetField(0);
			Assert::IsTrue(metaField0->GetMetaType() == Meta::DeduceType<float>());
			Assert::AreEqual(metaField0->GetName(), "Position");
			Assert::IsTrue(metaField0 == metaClass->FindField("Position"));

			auto metaField1 = metaClass->GetField(1);
			Assert::IsTrue(metaField1->GetMetaType() == Meta::DeduceType<bool>());
			Assert::AreEqual(metaField1->GetName(), "Automatic");
			Assert::IsTrue(metaField1 == metaClass->FindField("Automatic"));
		}

	};
 }