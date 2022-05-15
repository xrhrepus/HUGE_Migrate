#include "Precompiled.h"
#include "Quaternion.h"

using namespace H::Math;

//constexpr Quaternion H::Math::Quaternion::operator+(const Quaternion & rhs) const
//{
//	return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
//}
//constexpr Quaternion H::Math::Quaternion::operator-(const Quaternion & rhs) const
//{
//	return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
//}
//
//constexpr Quaternion H::Math::Quaternion::operator*(const Quaternion & rhs) const
//{
//	Vector3 v1( x,  y, z);
//	Vector3 v2 (rhs.x, rhs.y, rhs.z);
//	Vector3 res = v2 * w + v1 * rhs.w + v1.Cross(v2);
//	return Quaternion(res.x, res.y, res.z, (w * rhs.w - v1.Dot(v2)));
//}
//
//constexpr Quaternion H::Math::Quaternion::operator*(float s) const
//{
//	return Quaternion(s * x, s * y, s * z, s * w);
//}
//
//constexpr Quaternion H::Math::Quaternion::operator/(float s) const
//{
//	return Quaternion(x / s, y / s, z / s, w / s);
//}

//Quaternion H::Math::QuaternionLookRotation(const Vector3& forward, const Vector3& up)
//{
//	Vector3 z = Normalize(forward);
//	Vector3 x = Normalize(Cross(up, z));
//	Vector3 y = Normalize(Cross(z, x));
//	Matrix4 mat
//	(
//		x.x, x.y, x.z, 0.0f,
//		y.x, y.y, y.z, 0.0f,
//		z.x, z.y, z.z, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f
//	);
//
//	//// TODO: Need matrix to quaternion
//	//// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
//
//	float qw = (sqrtf(0.5f*(1 + mat._11 + mat._22 + mat._33)));
//	float fourXqw = 4 * qw;
//	return Quaternion((mat._32 - mat._23) / (fourXqw), (mat._13 - mat._31) / (fourXqw), (mat._12 - mat._21) / (fourXqw), qw);
//
//	
//
//}
