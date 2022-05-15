#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Math;
namespace MathTest
{		
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			Vector3 v0;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);

			Vector3 v1(1.0f);
			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 1.0f);
			Assert::AreEqual(v1.z, 1.0f);

			Vector3 v2(1.0f,11.0f,111.0f);
			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 11.0f);
			Assert::AreEqual(v2.z, 111.0f);

		}
		TEST_METHOD(TestAddition)
		{
			Vector3 v0(1.0f);
			Vector3 v1(1.0f, 11.0f, 111.0f);
			Vector3 v2 = v0 + v1;
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 11.0f);
			Assert::AreEqual(v1.z, 111.0f);

			Assert::AreEqual(v2.x, 2.0f);
			Assert::AreEqual(v2.y, 12.0f);
			Assert::AreEqual(v2.z, 112.0f);
		}
		TEST_METHOD(TestSubtraction)
		{
			Vector3 v0(1.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			Vector3 v2 = v0 - v1;
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			Assert::AreEqual(v2.x, 0.0f);
			Assert::AreEqual(v2.y, -9.0f);
			Assert::AreEqual(v2.z, -99.0f);
		}
		TEST_METHOD(TestMultiply)
		{
			Vector3 v0(2.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			Vector3 v2 = v0 * v1;
			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 2.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			Assert::AreEqual(v2.x, 2.0f);
			Assert::AreEqual(v2.y, 20.0f);
			Assert::AreEqual(v2.z, 200.0f);
		}
		TEST_METHOD(TestDivision)
		{
			Vector3 v0(200.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			Vector3 v2 = v0 / v1;
			Assert::AreEqual(v0.x, 200.0f);
			Assert::AreEqual(v0.y, 200.0f);
			Assert::AreEqual(v0.z, 200.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			Assert::AreEqual(v2.x, 200.0f);
			Assert::AreEqual(v2.y, 20.0f);
			Assert::AreEqual(v2.z, 2.0f);
		}
		TEST_METHOD(TestAddEqual)
		{
			Vector3 v0(200.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			
			Assert::AreEqual(v0.x, 200.0f);
			Assert::AreEqual(v0.y, 200.0f);
			Assert::AreEqual(v0.z, 200.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			v0 += v1;

			Assert::AreEqual(v0.x, 201.0f);
			Assert::AreEqual(v0.y, 210.0f);
			Assert::AreEqual(v0.z, 300.0f);
		}
		TEST_METHOD(TestSubEqual)
		{
			Vector3 v0(200.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);

			Assert::AreEqual(v0.x, 200.0f);
			Assert::AreEqual(v0.y, 200.0f);
			Assert::AreEqual(v0.z, 200.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			v0 -= v1;

			Assert::AreEqual(v0.x, 199.0f);
			Assert::AreEqual(v0.y, 190.0f);
			Assert::AreEqual(v0.z, 100.0f);
		}
		TEST_METHOD(TestMultiplyEqual)
		{
			Vector3 v1(1.0f, 10.0f, 100.0f);
			float k = 9.0;

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			v1 *= k;

			Assert::AreEqual(v1.x, 9.0f);
			Assert::AreEqual(v1.y, 90.0f);
			Assert::AreEqual(v1.z, 900.0f);
		}
		TEST_METHOD(TestDivideEqual)
		{
			Vector3 v1(18.0f, 180.0f, 1800.0f);
			float k = 9.0;

			Assert::AreEqual(v1.x, 18.0f);
			Assert::AreEqual(v1.y, 180.0f);
			Assert::AreEqual(v1.z, 1800.0f);

			v1 /= k;

			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 20.0f);
			Assert::AreEqual(v1.z, 200.0f);
		}

		TEST_METHOD(TestVector3Dot)
		{
			Vector3 v0(2.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			float tans = v0.Dot(v1);

			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 2.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			Assert::AreEqual(tans, 222.0f);
		}
		TEST_METHOD(TestVector3Cross)
		{
			Vector3 v0(2.0f);
			Vector3 v1(1.0f, 10.0f, 100.0f);
			Vector3 v2 = v0.Cross(v1);

			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 2.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 10.0f);
			Assert::AreEqual(v1.z, 100.0f);

			Assert::AreEqual(v2.x, 180.0f);
			Assert::AreEqual(v2.y, -198.0f);
			Assert::AreEqual(v2.z, 18.0f);
		}
		TEST_METHOD(TestVector3MagnitudeSqr)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);
			float tans = MagnitudeSqr(v1);
			
			Assert::AreEqual(v1.x, 3.0f);
			Assert::AreEqual(v1.y, 4.0f);
			Assert::AreEqual(v1.z, 5.0f);


			Assert::AreEqual(tans, 50.0f);
		}
		TEST_METHOD(TestVector3Magnitude)
		{
			Vector3 v1(1.0f, 3.0f, 2.0f);
			float tans = Magnitude(v1);
			float sqr14 = sqrt(14);
			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 3.0f);
			Assert::AreEqual(v1.z, 2.0f);


			Assert::AreEqual(tans, sqr14);
		}
		TEST_METHOD(TestVector3Normalize)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);
			Vector3 v2 = Normalize(v1);
			float sqr50 = sqrt(50);
			Assert::AreEqual(v1.x, 3.0f);
			Assert::AreEqual(v1.y, 4.0f);
			Assert::AreEqual(v1.z, 5.0f);


			Assert::IsTrue( fabsf(v2.x - (3.0f / sqr50)) < 0.00001 );
			Assert::IsTrue(fabsf(v2.y - (4.0f / sqr50)) < 0.00001);
			Assert::IsTrue(fabsf(v2.z - (5.0f / sqr50)) < 0.00001);

			//Assert::AreEqual(v2.y, 4.0f / sqr50);
			//Assert::AreEqual(v2.z, 5.0f / sqr50);
		}



	};
}