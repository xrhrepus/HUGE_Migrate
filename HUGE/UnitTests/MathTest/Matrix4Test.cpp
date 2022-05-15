#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H::Math;
namespace MathTest
{		
	TEST_CLASS(Matrix4Test)
	{
	public:
		
		TEST_METHOD(MatrixTestConstructor)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			mat1.v[9] = 999.0f;

			//copy cons
			Matrix4 mat2(mat1);
			Assert::AreEqual(mat1.v[9], mat2.v[9]);
			Assert::AreEqual(999.0f, mat2.v[9]);
			

		}
		TEST_METHOD(MatrixTestSetRow)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetRow(r, 2);

			Assert::AreEqual(mat1.v[8], 9.0f);
			Assert::AreEqual(mat1.v[9], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);
		}
		TEST_METHOD(MatrixTestSetColumn)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
		}
		TEST_METHOD(MatrixTestAdd)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat2;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat2.v[i], 0.0f);
			}

			mat2.SetRow(r, 2);

			Assert::AreEqual(mat2.v[8], 9.0f);
			Assert::AreEqual(mat2.v[9], 8.0f);
			Assert::AreEqual(mat2.v[10], 7.0f);
			Assert::AreEqual(mat2.v[11], 0.0f);

			Matrix4 mat3 = mat1 + mat2;
			Assert::AreEqual(mat3.v[0], 0.0f);
			Assert::AreEqual(mat3.v[1], 0.0f);
			Assert::AreEqual(mat3.v[2], 9.0f);
			Assert::AreEqual(mat3.v[3], 0.0f);

			Assert::AreEqual(mat3.v[4], 0.0f);
			Assert::AreEqual(mat3.v[5], 0.0f);
			Assert::AreEqual(mat3.v[6], 8.0f);
			Assert::AreEqual(mat3.v[7], 0.0f);

			Assert::AreEqual(mat3.v[8], 9.0f);
			Assert::AreEqual(mat3.v[9], 8.0f);
			Assert::AreEqual(mat3.v[10], 14.0f);
			Assert::AreEqual(mat3.v[11], 0.0f);

			Assert::AreEqual(mat3.v[12], 0.0f);
			Assert::AreEqual(mat3.v[13], 0.0f);
			Assert::AreEqual(mat3.v[14], 0.0f);
			Assert::AreEqual(mat3.v[15], 0.0f);
		}
		TEST_METHOD(MatrixTestAddEqual)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat2;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat2.v[i], 0.0f);
			}

			mat2.SetRow(r, 2);

			Assert::AreEqual(mat2.v[8], 9.0f);
			Assert::AreEqual(mat2.v[9], 8.0f);
			Assert::AreEqual(mat2.v[10], 7.0f);
			Assert::AreEqual(mat2.v[11], 0.0f);

			mat1 += mat2;
			Assert::AreEqual(mat1.v[0], 0.0f);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);
								
			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], 0.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);
								
			Assert::AreEqual(mat1.v[8], 9.0f);
			Assert::AreEqual(mat1.v[9], 8.0f);
			Assert::AreEqual(mat1.v[10], 14.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);
								
			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 0.0f);
		}
		TEST_METHOD(MatrixTestSub)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat2;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat2.v[i], 0.0f);
			}

			mat2.SetRow(r, 2);

			Assert::AreEqual(mat2.v[8], 9.0f);
			Assert::AreEqual(mat2.v[9], 8.0f);
			Assert::AreEqual(mat2.v[10], 7.0f);
			Assert::AreEqual(mat2.v[11], 0.0f);

			Matrix4 mat3 = mat1 - mat2;
			Assert::AreEqual(mat3.v[0], 0.0f);
			Assert::AreEqual(mat3.v[1], 0.0f);
			Assert::AreEqual(mat3.v[2], 9.0f);
			Assert::AreEqual(mat3.v[3], 0.0f);

			Assert::AreEqual(mat3.v[4], 0.0f);
			Assert::AreEqual(mat3.v[5], 0.0f);
			Assert::AreEqual(mat3.v[6], 8.0f);
			Assert::AreEqual(mat3.v[7], 0.0f);

			Assert::AreEqual(mat3.v[8], -9.0f);
			Assert::AreEqual(mat3.v[9], -8.0f);
			Assert::AreEqual(mat3.v[10], 0.0f);
			Assert::AreEqual(mat3.v[11], 0.0f);

			Assert::AreEqual(mat3.v[12], 0.0f);
			Assert::AreEqual(mat3.v[13], 0.0f);
			Assert::AreEqual(mat3.v[14], 0.0f);
			Assert::AreEqual(mat3.v[15], 0.0f);
		}
		TEST_METHOD(MatrixTestIsIdentity)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			bool res = mat1.IsIdentity();
			Assert::AreEqual(res,false);
			mat1.SetIdentity();
			res = mat1.IsIdentity();
			Assert::AreEqual(res, true);
		}
		TEST_METHOD(MatrixTestIsZero)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			bool res = mat1.IsZero();
			Assert::AreEqual(res, true);
		}

		TEST_METHOD(MatrixTestSubEqual)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat2;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat2.v[i], 0.0f);
			}

			mat2.SetRow(r, 2);

			Assert::AreEqual(mat2.v[8], 9.0f);
			Assert::AreEqual(mat2.v[9], 8.0f);
			Assert::AreEqual(mat2.v[10], 7.0f);
			Assert::AreEqual(mat2.v[11], 0.0f);

			mat1 -= mat2;
			Assert::AreEqual(mat1.v[0], 0.0f);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);
								
			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], 0.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);
								
			Assert::AreEqual(mat1.v[8], -9.0f);
			Assert::AreEqual(mat1.v[9], -8.0f);
			Assert::AreEqual(mat1.v[10], 0.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);
								
			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 0.0f);
		}
		TEST_METHOD(MatrixTestMultiplyMatrix)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat2;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat2.v[i], 0.0f);
			}

			mat2.SetRow(r, 2);

			Assert::AreEqual(mat2.v[8], 9.0f);
			Assert::AreEqual(mat2.v[9], 8.0f);
			Assert::AreEqual(mat2.v[10], 7.0f);
			Assert::AreEqual(mat2.v[11], 0.0f);

			Matrix4 mat3 = mat1 * mat2;
			Assert::AreEqual(mat3.v[0], 81.0f);
			Assert::AreEqual(mat3.v[1], 72.0f);
			Assert::AreEqual(mat3.v[2], 63.0f);
			Assert::AreEqual(mat3.v[3], 0.0f);

			Assert::AreEqual(mat3.v[4], 72.0f);
			Assert::AreEqual(mat3.v[5], 64.0f);
			Assert::AreEqual(mat3.v[6], 56.0f);
			Assert::AreEqual(mat3.v[7], 0.0f);

			Assert::AreEqual(mat3.v[8], 63.0f);
			Assert::AreEqual(mat3.v[9], 56.0f);
			Assert::AreEqual(mat3.v[10], 49.0f);
			Assert::AreEqual(mat3.v[11], 0.0f);

			Assert::AreEqual(mat3.v[12], 0.0f);
			Assert::AreEqual(mat3.v[13], 0.0f);
			Assert::AreEqual(mat3.v[14], 0.0f);
			Assert::AreEqual(mat3.v[15], 0.0f);
		}
		TEST_METHOD(MatrixTestMultiplScalar)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.SetColumn(r, 2);

			Assert::AreEqual(mat1.v[2], 9.0f);
			Assert::AreEqual(mat1.v[6], 8.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);

			Matrix4 mat3 = mat1 * 10.0f;

			Assert::AreEqual(mat3.v[0], 0.0f);
			Assert::AreEqual(mat3.v[1], 0.0f);
			Assert::AreEqual(mat3.v[2], 90.0f);
			Assert::AreEqual(mat3.v[3], 0.0f);

			Assert::AreEqual(mat3.v[4], 0.0f);
			Assert::AreEqual(mat3.v[5], 0.0f);
			Assert::AreEqual(mat3.v[6], 80.0f);
			Assert::AreEqual(mat3.v[7], 0.0f);

			Assert::AreEqual(mat3.v[8], 0.0f);
			Assert::AreEqual(mat3.v[9], 0.0f);
			Assert::AreEqual(mat3.v[10], 70.0f);
			Assert::AreEqual(mat3.v[11], 0.0f);

			Assert::AreEqual(mat3.v[12], 0.0f);
			Assert::AreEqual(mat3.v[13], 0.0f);
			Assert::AreEqual(mat3.v[14], 0.0f);
			Assert::AreEqual(mat3.v[15], 0.0f);

		}


		//TEST_METHOD(MatrixTestMultiplVector3)
		//{
		//	Matrix4 mat1;
		//	for (size_t i = 0; i < 16; i++)
		//	{
		//		Assert::AreEqual(mat1.v[i], 0.0f);
		//	}
		//	Vector3 r{ 9.0f,8.0f,7.0f };
		//	mat1.SetColumn(r, 2);

		//	Assert::AreEqual(mat1.v[2], 9.0f);
		//	Assert::AreEqual(mat1.v[6], 8.0f);
		//	Assert::AreEqual(mat1.v[10], 7.0f);
		//	Assert::AreEqual(mat1.v[14], 0.0f);

		//	Vector3 result = mat1 * (r);

		//	Assert::AreEqual(result.x, 0.0f);
		//	Assert::AreEqual(result.y, 0.0f);
		//	Assert::AreEqual(result.z, 81.0f + 64.0f + 49.0f);

		//}

		TEST_METHOD(MatrixTestTranslation)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.Translation(r);

			Assert::AreEqual(mat1.v[0], 1.0f);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], 0.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);
								
			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], 1.0f);
			Assert::AreEqual(mat1.v[6], 0.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);
								
			Assert::AreEqual(mat1.v[8], 0.0f);
			Assert::AreEqual(mat1.v[9], 0.0f);
			Assert::AreEqual(mat1.v[10], 1.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);
								
			Assert::AreEqual(mat1.v[12], 9.0f);
			Assert::AreEqual(mat1.v[13], 8.0f);
			Assert::AreEqual(mat1.v[14], 7.0f);
			Assert::AreEqual(mat1.v[15], 1.0f);

		}
		TEST_METHOD(MatrixTestRotationX)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };

			float angle = 60.0f;
			mat1.RotationX(angle);
			float cosX = cosf(angle); float sinX = sinf(angle);
			//Assert::AreEqual(cosX, 0.5f);

			Assert::AreEqual(mat1.v[0], 1.0f);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], 0.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);

			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], cosX);
			Assert::AreEqual(mat1.v[6], -sinX);
			Assert::AreEqual(mat1.v[7], 0.0f);

			Assert::AreEqual(mat1.v[8], 0.0f);
			Assert::AreEqual(mat1.v[9], sinX);
			Assert::AreEqual(mat1.v[10], cosX);
			Assert::AreEqual(mat1.v[11], 0.0f);

			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 1.0f);

		}
		TEST_METHOD(MatrixTestRotationY)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };

			float angle = 60.0f;
			mat1.RotationY(angle);
			float cosX = cosf(angle); float sinX = sinf(angle);
			//Assert::AreEqual(cosX, 0.5f);

			Assert::AreEqual(mat1.v[0], cosX);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], sinX);
			Assert::AreEqual(mat1.v[3], 0.0f);

			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], 1.0f);
			Assert::AreEqual(mat1.v[6], 0.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);

			Assert::AreEqual(mat1.v[8], -sinX);
			Assert::AreEqual(mat1.v[9], 0.0f);
			Assert::AreEqual(mat1.v[10], cosX);
			Assert::AreEqual(mat1.v[11], 0.0f);

			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 1.0f);

		}
		TEST_METHOD(MatrixTestRotationZ)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };

			float angle = 60.0f;
			mat1.RotationZ(angle);
			float cosX = cosf(angle); float sinX = sinf(angle);
			//Assert::AreEqual(cosX, 0.5f);

			Assert::AreEqual(mat1.v[0], cosX);
			Assert::AreEqual(mat1.v[1], -sinX);
			Assert::AreEqual(mat1.v[2], 0.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);

			Assert::AreEqual(mat1.v[4], sinX);
			Assert::AreEqual(mat1.v[5], cosX);
			Assert::AreEqual(mat1.v[6], 0.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);

			Assert::AreEqual(mat1.v[8], 0.0f);
			Assert::AreEqual(mat1.v[9], 0.0f);
			Assert::AreEqual(mat1.v[10], 1.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);

			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 1.0f);

		}
		TEST_METHOD(MatrixTestScaling)
		{
			Matrix4 mat1;
			for (size_t i = 0; i < 16; i++)
			{
				Assert::AreEqual(mat1.v[i], 0.0f);
			}
			Vector3 r{ 9.0f,8.0f,7.0f };
			mat1.Scaling(r);

			Assert::AreEqual(mat1.v[0], 9.0f);
			Assert::AreEqual(mat1.v[1], 0.0f);
			Assert::AreEqual(mat1.v[2], 0.0f);
			Assert::AreEqual(mat1.v[3], 0.0f);

			Assert::AreEqual(mat1.v[4], 0.0f);
			Assert::AreEqual(mat1.v[5], 8.0f);
			Assert::AreEqual(mat1.v[6], 0.0f);
			Assert::AreEqual(mat1.v[7], 0.0f);

			Assert::AreEqual(mat1.v[8], 0.0f);
			Assert::AreEqual(mat1.v[9], 0.0f);
			Assert::AreEqual(mat1.v[10], 7.0f);
			Assert::AreEqual(mat1.v[11], 0.0f);

			Assert::AreEqual(mat1.v[12], 0.0f);
			Assert::AreEqual(mat1.v[13], 0.0f);
			Assert::AreEqual(mat1.v[14], 0.0f);
			Assert::AreEqual(mat1.v[15], 1.0f);

		}

	};
}