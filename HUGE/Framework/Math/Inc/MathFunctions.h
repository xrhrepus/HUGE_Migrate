#ifndef HUGE_MATH_MATHFUNCTIONS_H
#define HUGE_MATH_MATHFUNCTIONS_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Constants.h"
#include "Quaternion.h"
 
namespace H::Math
{
 	/*constexpr float sqrf(float f)
	{
		return f * f;
	}*/

	constexpr float Minf(const float f1, const float f2)
	{
		return f1 > f2 ? f2 : f1;
	}
	constexpr float Maxf(const float f1, const float f2)
	{
		return f1 > f2 ? f1 : f2;
	}
#pragma region Random

	int RandomInt();
	int RandomInt(int min, int max);
	float RandomFloat();
	float RandomFloat(float min, float max);
	Vector2 RandomVec2();
	Vector2 RandomVec2(const Vector2& min, const Vector2& max);
	Vector3 RandomVec3();
	Vector3 RandomVec3(const Vector3& min, const Vector3& max);
	Vector3 RandomUnitSphere();

#pragma endregion



#pragma region Comparism

	inline float Abs(float value) { return (value >= 0.0f) ? value : -value; }
	inline float Sign(float value) { return (value >= 0.0f) ? 1.0f : -1.0f; }
	inline float Sqr(float value) { return value * value; }
	inline float Sqrt(float value) { return sqrtf(value); }


	inline bool Compare(float a, float b, float epsilon = FLT_MIN) { return Abs(a - b) <= epsilon; }
	inline bool IsZero(float value) { return Compare(value, 0.0f); }
	inline bool IsZero(const Vector2& v) { return IsZero(v.x) && IsZero(v.y); }
	inline bool IsZero(const Vector3& v) { return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); }

#pragma endregion

	//VectorFunctions
#pragma region VectorFunctions

		//NOTE:0507
	//constexpr using SIMD[ SSE register
	//proc 2X data by one instruction
	constexpr float Dot(const Vector3& v1, const Vector3& v2)
	{
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	constexpr float Dot(const Vector2& v1, const Vector2& v2)
	{
		return (v1.x*v2.x + v1.y*v2.y);
	}

	constexpr Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		Vector3 ret;
		ret.x = (v1.y * v2.z) - (v1.z * v2.y);
		ret.y = (v1.z*v2.x) - (v1.x * v2.z);
		ret.z = (v1.x*v2.y) - (v1.y*v2.x);
		return ret;
	}

	constexpr float MagnitudeSqr(const Vector2& v)
	{
		return Dot(v, v);
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		return Dot(v, v);
	}

	inline float Magnitude(const Vector2& v)
	{
		return sqrt(MagnitudeSqr(v));
	}
	inline float Magnitude(const Vector3& v)
	{
		return sqrt(MagnitudeSqr(v));
	}


	inline Vector2 Normalize(const Vector2& v)
	{
		//NOTE:0507
		// v / Magnitude(v) == v.x/ f ; v.y/f ; v.z/f   (3 division)
		return v * (1.0f / Magnitude(v));// v * f		(1 multiply)
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		//NOTE:0507
		// v / Magnitude(v) == v.x/ f ; v.y/f ; v.z/f   (3 division)
		return v * (1.0f / Magnitude(v));// v * f		(1 multiply)
	}
	constexpr Vector2 Projection(const Vector2& v, const Vector2& onto)
	{
		return onto * (Dot(v, onto) / MagnitudeSqr(onto));
	}


	inline float Distance(const Vector2& v1, const Vector2& v2)
	{
		return sqrtf(((v1.x - v2.x)*(v1.x - v2.x)) + ((v1.y - v2.y)*(v1.y - v2.y)));
	}
	inline float Distance(const Vector3& v1, const Vector3& v2)
	{
		return sqrtf(((v1.x - v2.x)*(v1.x - v2.x)) + ((v1.y - v2.y)*(v1.y - v2.y)) + ((v1.z - v2.z)*(v1.z - v2.z)));
	}

#pragma endregion 
	//VectorFunctions
 

	inline float Truncate(float numerator, float denominator)
	{
		float r = numerator / denominator;
		return r > 1.0f ? 1.0f : r;
	}

	//inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
	//{
	//	return Vector2
	//	(
	//		v.x * m._11 + v.y * m._21 + m._31,
	//		v.x * m._12 + v.y * m._22 + m._32
	//	);
	//}

	inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		return Vector3
		(
			v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
			v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
			v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
		);
	}

	inline Matrix4 Translation(const Vector3& translate)
	{
		return Matrix4{
				1.0f, 0.0f, 0.0f,  0.0f,
				0.0f, 1.0f, 0.0f,  0.0f,
				0.0f, 0.0f, 1.0f,  0.0f,
				translate.x,  translate.y,  translate.z,  1.0f
		};

	}
	inline Matrix4 RotationX(const float angle)
	{
		float cosX = cosf(angle); float sinX = sinf(angle);
		return Matrix4{
				1.0f, 0.0f, 0.0f,  0.0f,
				0.0f, cosX, -sinX,  0.0f,
				0.0f, sinX, cosX,  0.0f,
				0.0f, 0.0f, 0.0f,  1.0f
		};

	}
	inline Matrix4 RotationY(const float angle)
	{
		float cosY = cosf(angle); float sinY = sinf(angle);

		return Matrix4{
				cosY, 0.0f, sinY,  0.0f,
				0.0f, 1.0f, 0.0f,  0.0f,
				-sinY, 0.0f, cosY,  0.0f,
				0.0f, 0.0f, 0.0f,  1.0f
		};

	}

	inline Matrix4 RotationZ(const float angle)
	{
		float cosZ = cosf(angle); float sinZ = sinf(angle);
		return Matrix4{
				cosZ, -sinZ, 0.0f,  0.0f,
				sinZ, cosZ, 0.0f,  0.0f,
				0.0f, 0.0f, 1.0f,  0.0f,
				0.0f, 0.0f, 0.0f,  1.0f
		};
	}
	inline Matrix4 Scaling(const Vector3& scale)
	{
		return Matrix4{
				scale.x, 0.0f, 0.0f,  0.0f,
				0.0f, scale.y, 0.0f,  0.0f,
				0.0f, 0.0f, scale.z,  0.0f,
				0.0f, 0.0f, 0.0f,  1.0f
		};
	}

	inline Matrix4 Transpose(const Matrix4& mat)
	{
		Matrix4 ret;

		ret._11 = mat._11; ret._12 = mat._21; ret._13 = mat._31; ret._14 = mat._41;

		ret._21 = mat._12; ret._22 = mat._22; ret._23 = mat._32; ret._24 = mat._42;

		ret._31 = mat._13; ret._32 = mat._23; ret._33 = mat._33; ret._34 = mat._43;

		ret._41 = mat._14; ret._42 = mat._24; ret._43 = mat._34; ret._44 = mat._44;

		return ret;
	}
	inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return Vector3
		(
			v.x * m._11 + v.y * m._21 + v.z * m._31,
			v.x * m._12 + v.y * m._22 + v.z * m._32,
			v.x * m._13 + v.y * m._23 + v.z * m._33
		);
	}
	inline Matrix4 MatrixRotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return Matrix4
		(
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}
	inline float Lerp(float a, float b, float t)
	{
		return a + ((b - a) * t);
	}

	inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
	{
		return a + ((b - a) * t);
	}

	inline float Magnitude(const Quaternion& q) { return sqrtf((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)); }
	inline Quaternion Normalize(const Quaternion& q) { return q / Magnitude(q); }
	Quaternion Lerp(Quaternion q0, Quaternion q1, float t);
	Quaternion Slerp(Quaternion q0, Quaternion q1, float t);

	//Constructing a quaternion from a rotation matrix
	Quaternion QuaternionFromRotationMatrix(const Matrix4& mat);



	//Conversion from quaternion to matrix
	inline Matrix4 MatrixRotationQuaternion(const Quaternion& q)
	{
		return Matrix4
		(
			1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
			(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
			(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
			0.0f,

			(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
			(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
			0.0f,

			(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
			(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}


	//Constructing a quaternion from an axis and an angle
	Quaternion QuaternionRotationAxis(const Vector3& axis, float rad);

	//Constructing a quaternion based on the look and up directions
	Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up);

	//Constructing a quaternion based on a from and to directions
	Quaternion QuaternionFromToRotation(const Vector3& from, const Vector3& to);

	inline float Determinant(const Matrix4& m)
	{
		float det = 0.0f;
		det = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
		det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
		det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
		det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
		return det;
	}

	inline Matrix4 Adjoint(const Matrix4& m)
	{
		return Matrix4
		(
			(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		);
	}
	inline Matrix4 Inverse(const Matrix4& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}
}//namespace H::Math

#endif // !HUGE_MATH_MATHFUNCTIONS_H

