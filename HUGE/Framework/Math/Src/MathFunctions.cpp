#include "Precompiled.h"

#include "MathFunctions.h"

using namespace H::Math;
namespace
{
	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };
}

//float H::Math::RandomFloat()
//{
//	return std::uniform_real_distribution<float>{ 0, 1.0f }(myRandomEngine);
//}
//
//float H::Math::RandomFloat(float min, float max)
//{
//	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
//}

H::Math::Quaternion H::Math::Lerp(Quaternion q0, Quaternion q1, float t)
{
	return q0 * (1.0f - t) + (q1 * t);
}
H::Math::Quaternion H::Math::Slerp(Quaternion q0, Quaternion q1, float t)
{
	// Find the dot product
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	// Determine the direction of the rotation.
	if (dot < 0.0f)
	{
		dot = -dot;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
	}
	else if (dot > 0.999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	float theta = acosf(dot);
	float sintheta = sinf(theta);
	float scale0 = sinf(theta * (1.0f - t)) / sintheta;
	float scale1 = sinf(theta * t) / sintheta;

	// Perform the slerp
	return Quaternion
	(
		(q0.x * scale0) + (q1.x * scale1),
		(q0.y * scale0) + (q1.y * scale1),
		(q0.z * scale0) + (q1.z * scale1),
		(q0.w * scale0) + (q1.w * scale1)
	);
}

Quaternion H::Math::QuaternionFromRotationMatrix(const Matrix4 & mat)
{
	//qw = ¡Ì(1 + m00 + m11 + m22) / 2
	//	qx = (m21 - m12) / (4 * qw)
	//	qy = (m02 - m20) / (4 * qw)
	//	qz = (m10 - m01) / (4 * qw)
	//https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quaternion q;
	float trace = mat._11 + mat._22 + mat._33;
	if (trace > 0.0f)
	{
		float s = 0.5f / sqrtf(1.0f + trace);
		q.w = 0.25f / s;
		q.x = (mat._32 - mat._23) * s;
		q.y = (mat._13 - mat._31) * s;
		q.z = (mat._21 - mat._12) * s;
	}
	else if (mat._11 > mat._22 && mat._11 > mat._33)
	{
		float s = 2.0f * sqrtf(1.0f + mat._11 - mat._22 - mat._33);
		q.w = (mat._32 - mat._23) / s;
		q.x = 0.25f * s;
		q.y = (mat._12 + mat._21) * s;
		q.z = (mat._13 + mat._31) * s;
	}
	else if (mat._22 > mat._33)
	{
		float s = 2.0f * sqrtf(1.0f + mat._22 - mat._11 - mat._33);
		q.w = (mat._13 - mat._31) / s;
		q.x = (mat._12 + mat._21) / s;
		q.y = 0.25f * s;
		q.z = (mat._23 + mat._32) * s;
	}
	else
	{
		float s = 2.0f * sqrtf(1.0f + mat._33 - mat._11 - mat._22);
		q.w = (mat._21 - mat._12) / s;
		q.x = (mat._13 + mat._31) / s;
		q.y = (mat._23 + mat._32) * s;
		q.z = 0.25f * s;
	}

	return q;
}

//H::Math::Matrix4 MatrixRotationQuaternion(const Quaternion& q)
//{
//	return Matrix4
//	(
//		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
//		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
//		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
//		0.0f,
//
//		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
//		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
//		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
//		0.0f,
//
//		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
//		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
//		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
//		0.0f,
//
//		0.0f,
//		0.0f,
//		0.0f,
//		1.0f
//	);
//}


//Constructing a quaternion from an axis and an angle
H::Math::Quaternion H::Math::QuaternionRotationAxis(const Vector3 & axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(a.x * s, a.y * s, a.z * s, c);
}


//TODO: Constructing a quaternion based on a from and to directions
Quaternion H::Math::QuaternionFromToRotation(const Vector3 & from, const Vector3 & to)
{
	Quaternion q;
	//vector a = crossproduct(v1, v2);
	Vector3 a = Cross(from, to);
	//q.xyz = a;
	q.x = a.x; q.y = a.y; q.z = a.z;
	//q.w = sqrt((v1.Length ^ 2) * (v2.Length ^ 2)) + dotproduct(v1, v2);}
	q.w = sqrtf(MagnitudeSqr(from) * MagnitudeSqr(to) + Dot(from, to));
	q = Normalize(q);
	return q;
}

// Constructing a quaternion based on the look and up directions
H::Math::Quaternion H::Math::QuaternionLookRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 z = Normalize(forward);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//// TODO: Need matrix to quaternion
	//// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

	//float qw = (sqrtf(0.5f*(1 + mat._11 + mat._22 + mat._33)));
	//float fourXqw = 4 * qw;
	//return Quaternion((mat._32 - mat._23) / (fourXqw), (mat._13 - mat._31) / (fourXqw), (mat._12 - mat._21) / (fourXqw), qw);

	return QuaternionFromRotationMatrix(mat);

}

int H::Math::RandomInt()
{
	return std::uniform_int_distribution<>{ 0, (std::numeric_limits<int>::max)() }(myRandomEngine);
}

int H::Math::RandomInt(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(myRandomEngine);
}

float H::Math::RandomFloat()
{
	return std::uniform_real_distribution<float>{ 0, 1.0f }(myRandomEngine);
}

float H::Math::RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
}

Vector2 H::Math::RandomVec2()
{
	return Math::Vector2
	(
		RandomFloat(),
		RandomFloat()
	);
}

Vector2 H::Math::RandomVec2(const Vector2 & min, const Vector2 & max)
{
	return Math::Vector2
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y)
	);
}

Vector3 H::Math::RandomVec3()
{
	return Math::Vector3
	(
		RandomFloat(),
		RandomFloat(),
		RandomFloat()
	);
}

Vector3 H::Math::RandomVec3(const Vector3 & min, const Vector3 & max)
{
	return Math::Vector3
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z)
	);
}

Vector3 H::Math::RandomUnitSphere()
{
	return Math::Normalize(Math::Vector3(
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f),
		RandomFloat(-1.0f, 1.0f))
	);
}
