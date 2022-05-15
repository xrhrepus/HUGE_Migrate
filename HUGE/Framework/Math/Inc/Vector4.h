#ifndef HUGE_MATH_VECTOR4_H
#define HUGE_MATH_VECTOR4_H

namespace H {
	namespace Math {

		struct Vector4
		{
			union{float x; float r;};
			union { float y; float g; };
			union { float z; float b; };
			union { float w; float a; };
			//constexpr  

			constexpr Vector4() noexcept : Vector4(0.0f) {}
			constexpr Vector4(float f) noexcept : x(f), y(f), z(f),w(f) {}
			constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z),w(w) {}


			//constexpr Vector4 operator-() { return { -x,-y,-z,-w }; }
			//constexpr Vector4 operator+(const Vector4& v) const { return { x + v.x,y + v.y,z + v.z ,w + v.w }; }
			//constexpr Vector4 operator-(const Vector4& v) const { return { x - v.x,y - v.y,z - v.z, w- v.w }; }
			//constexpr Vector4 operator*(const Vector4& v) const { return { x * v.x,y * v.y,z * v.z , w*v.w}; }
			//constexpr Vector4 operator*(float f) const { return { x * f,y * f,z * f,w*f }; }
			//constexpr Vector4 operator/(const Vector4& v) const { return { x / v.x,y / v.y,z / v.z,w/v.w }; }
			//constexpr Vector3 operator*(const Matrix4& mat)const // need test
			//{
			//	Vector3 ret;
			//	ret.x = x*mat._11 + y*mat._21 + z*mat._31 + 1.0f *mat._41;
			//	ret.y = x*mat._12 + y*mat._22 + z*mat._32 + 1.0f *mat._42;
			//	ret.z = x*mat._13 + y*mat._23 + z*mat._33 + 1.0f *mat._43;
			//	return ret;
			//}

			// -  / 
			//Vector4& operator+=(const Vector4& v)
			//{
			//	if (&v != this)
			//	{
			//		x += v.x;
			//		y += v.y;
			//		z += v.z;
			//	}
			//	return *this;
			//}
			//Vector4& operator-=(const Vector4& v)
			//{
			//	if (&v != this)
			//	{
			//		x -= v.x;
			//		y -= v.y;
			//		z -= v.z;
			//	}
			//	return *this;
			//}

			//Vector4& operator*=(float f)
			//{
			//	x *= f;
			//	y *= f;
			//	z *= f;
			//	return *this;
			//}
			//Vector4& operator/=(float f)
			//{
			//	x /= f;
			//	y /= f;
			//	z /= f;
			//	return *this;
			//}

			////-=,*=,/=
			//constexpr float Dot(const Vector4& v)
			//{
			//	return (x * v.x) + (y * v.y) + (z * v.z);
			//}

			//constexpr Vector4 Cross(const Vector4& v)
			//{
			//	Vector4 ret;
			//	ret.x = (y * v.z) - (z * v.y);
			//	ret.y = (z*v.x) - (x * v.z);
			//	ret.z = (x*v.y) - (y*v.x);
			//	return ret;
			//}



		};




	}// namespace Math
}// namespace H

#endif // !HUGE_MATH_VECTOR3_H
