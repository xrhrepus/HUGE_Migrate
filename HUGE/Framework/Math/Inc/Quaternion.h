#ifndef HUGE_MATH_QUATERNION_H
#define HUGE_MATH_QUATERNION_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Constants.h"

namespace H::Math
{
	struct Quaternion
	{
		float x, y, z, w;
	public:
		constexpr Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f){}
		constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}

		static constexpr Quaternion Zero() { return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
		static constexpr Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }


		////Addition
		////constexpr Quaternion operator+(const Quaternion& rhs) const;

		////Subtraction
		////constexpr Quaternion operator-(const Quaternion& rhs) const;

		////Multiplication by another quaternion
		////constexpr Quaternion operator*(const Quaternion& rhs) const;

		////Multiplication by scalar
		////constexpr Quaternion operator*(float s) const;

		////constexpr Quaternion operator/(float s) const;
		constexpr Quaternion operator+(const Quaternion & rhs) const
		{
			return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}
		constexpr Quaternion operator-(const Quaternion & rhs) const
		{
			return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		constexpr Quaternion operator*(const Quaternion & rhs) const
		{
			Vector3 v1(x, y, z);
			Vector3 v2(rhs.x, rhs.y, rhs.z);
			Vector3 res = v2 * w + v1 * rhs.w + v1.Cross(v2);
			return Quaternion(res.x, res.y, res.z, (w * rhs.w - v1.Dot(v2)));
		}

		constexpr Quaternion operator*(float s) const
		{
			return Quaternion(s * x, s * y, s * z, s * w);
		}

		constexpr Quaternion operator/(float s) const
		{
			return Quaternion(x / s, y / s, z / s, w / s);
		}

	
	
	};

}//namespace H::Math

#endif // !HUGE_MATH_QUATERNION_H

