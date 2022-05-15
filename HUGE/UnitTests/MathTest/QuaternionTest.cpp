#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Math;
namespace MathTest
{		
	TEST_CLASS(QuaternionTest)
	{
	public:
#pragma region Constructor&StaticFunc
		TEST_METHOD(QuaternionDefaultConstructor)
		{
			Quaternion q;

 			Assert::AreEqual(q.w, 0.0f);
			Assert::AreEqual(q.x, 0.0f);
			Assert::AreEqual(q.y, 0.0f);
			Assert::AreEqual(q.z, 0.0f);
 
		}
		TEST_METHOD(QuaternionConstructor)
		{
			Quaternion q(1.0f,12.0f,123.0f,1234.0f);

			Assert::AreEqual(q.x, 1.0f);
			Assert::AreEqual(q.y, 12.0f);
			Assert::AreEqual(q.z, 123.0f);
			Assert::AreEqual(q.w, 1234.0f);

 
		}
		TEST_METHOD(QuaternioStaticZero)
		{
			Quaternion q(1.0f, 12.0f, 123.0f, 1234.0f);

			Assert::AreEqual(q.x, 1.0f);
			Assert::AreEqual(q.y, 12.0f);
			Assert::AreEqual(q.z, 123.0f);
			Assert::AreEqual(q.w, 1234.0f);
			q = Quaternion::Zero();

			Assert::AreEqual(q.x, 0.0f);
			Assert::AreEqual(q.y, 0.0f);
			Assert::AreEqual(q.z, 0.0f);
			Assert::AreEqual(q.w, 0.0f);
		}

		TEST_METHOD(QuaternioStaticIdentity)
		{
			Quaternion q(1.0f, 12.0f, 123.0f, 1234.0f);

			Assert::AreEqual(q.x, 1.0f);
			Assert::AreEqual(q.y, 12.0f);
			Assert::AreEqual(q.z, 123.0f);
			Assert::AreEqual(q.w, 1234.0f);
			q = Quaternion::Identity();

			Assert::AreEqual(q.w, 1.0f);
			Assert::AreEqual(q.x, 0.0f);
			Assert::AreEqual(q.y, 0.0f);
			Assert::AreEqual(q.z, 0.0f);
		}
#pragma endregion
#pragma region Operators
		TEST_METHOD(QuaternionAddition)
		{
			Quaternion q1(1.0f, 20.0f, 300.0f, 4000.0f);
			Assert::AreEqual(q1.x, 1.0f);
			Assert::AreEqual(q1.y, 20.0f);
			Assert::AreEqual(q1.z, 300.0f);
			Assert::AreEqual(q1.w, 4000.0f);

			Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
			Assert::AreEqual(q2.x, 1.0f);
			Assert::AreEqual(q2.y, 2.0f);
			Assert::AreEqual(q2.z, 3.0f);
			Assert::AreEqual(q2.w, 4.0f);

			Quaternion q3;
			q3 = q1 + q2;
			Assert::AreEqual(q3.x, 2.0f);
			Assert::AreEqual(q3.y, 22.0f);
			Assert::AreEqual(q3.z, 303.0f);
			Assert::AreEqual(q3.w, 4004.0f);


		}


#pragma endregion

 
	};
}