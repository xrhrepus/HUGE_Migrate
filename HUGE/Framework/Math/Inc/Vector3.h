#ifndef HUGE_MATH_VECTOR3_H
#define HUGE_MATH_VECTOR3_H


namespace H::Math{

	struct Vector3
	{
		union 
		{
			struct{ float x, y, z; };
			std::array<float,3> v;
		};
		//constexpr  

		constexpr Vector3() noexcept: Vector3(0.0f) {}

		constexpr Vector3(float f) noexcept : x(f),y(f),z(f){}
		constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
		constexpr Vector3(const Vector2& vec2) noexcept :x(vec2.x), y(vec2.y), z(0.0f) {}

		static constexpr Vector3 xAxis() { return Vector3{ 1.0f,0.0f,0.0f }; }
		static constexpr Vector3 yAxis() { return Vector3{ 0.0f,1.0f,0.0f }; }
		static constexpr Vector3 zAxis() { return Vector3{ 0.0f,0.0f,1.0f }; }
		static constexpr Vector3 Zero() { return Vector3{ 0.0f,0.0f,0.0f }; }
 
		constexpr Vector3 operator-() { return { -x,-y,-z }; }
		constexpr Vector3 operator+(const Vector3& v) const { return { x + v.x,y + v.y,z + v.z }; }
		constexpr Vector3 operator-(const Vector3& v) const { return { x - v.x,y - v.y,z - v.z }; }
		constexpr Vector3 operator*(const Vector3& v) const { return { x * v.x,y * v.y,z * v.z }; }
		constexpr Vector3 operator*(float f) const { return { x * f,y * f,z * f }; }
		constexpr Vector3 operator/(const Vector3& v) const { return { x / v.x,y / v.y,z / v.z }; }
		//constexpr Vector3 operator*(const Matrix4& mat)const // need test
		//{
		//	Vector3 ret;
		//	ret.x = x*mat._11 + y*mat._21 + z*mat._31 + 1.0f *mat._41;
		//	ret.y = x*mat._12 + y*mat._22 + z*mat._32 + 1.0f *mat._42;
		//	ret.z = x*mat._13 + y*mat._23 + z*mat._33 + 1.0f *mat._43;
		//	return ret;
		//}

		// -  / 
		Vector3& operator+=(const Vector3& v)
		{
			if (&v != this)
			{
				x += v.x;
				y += v.y;
				z += v.z;
			}
			return *this;
		}
		Vector3& operator-=(const Vector3& v)
		{
			if (&v != this)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
			}
			return *this;
		}

		Vector3& operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		Vector3& operator/=(float f)
		{
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}

		//-=,*=,/=
		constexpr float Dot(const Vector3& v)
		{
			return (x * v.x) + (y * v.y) + (z * v.z);
		}

		constexpr Vector3 Cross(const Vector3& v)
		{
			Vector3 ret;
			ret.x = (y * v.z) - (z * v.y);
			ret.y = (z*v.x) - (x * v.z);
			ret.z = (x*v.y) - (y*v.x);
			return ret;
		}
 

	};

 
}// namespace H::Math
 
#endif // !HUGE_MATH_VECTOR3_H
