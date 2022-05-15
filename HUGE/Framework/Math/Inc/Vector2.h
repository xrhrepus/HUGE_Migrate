#ifndef HUGE_MATH_VECTOR2_H
#define HUGE_MATH_VECTOR2_H

 namespace H::Math{

	struct Vector2
	{
		union 
		{
			struct{ float x, y; };
			std::array<float,2> v;
		};
		//constexpr  

		constexpr Vector2() noexcept: Vector2(0.0f) {}
		constexpr Vector2(float f) noexcept : x(f),y(f) {}
		constexpr Vector2(float x, float y ) noexcept : x(x), y(y)  {}
		//constexpr Vector2(const Vector3& v3) noexcept : x(v3.x), y(v3.y) {}
		
		//constexpr Vector2 Zero() noexcept { return Vector2(0.0f); }


		constexpr Vector2 operator-() { return { -x,-y  }; }
		constexpr Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y   }; }
		constexpr Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y   }; }
		constexpr Vector2 operator*(const Vector2& v) const { return { x * v.x, y * v.y   }; }
		constexpr Vector2 operator*(float f) const { return { x * f,y * f  }; }
		constexpr Vector2 operator/(const Vector2& v) const { return { x / v.x,y / v.y   }; }
		//constexpr Vector3 operator*(const Matrix4& mat)const // need test
		//{
		//	Vector3 ret;
		//	ret.x = x*mat._11 + y*mat._21 + z*mat._31 + 1.0f *mat._41;
		//	ret.y = x*mat._12 + y*mat._22 + z*mat._32 + 1.0f *mat._42;
		//	ret.z = x*mat._13 + y*mat._23 + z*mat._33 + 1.0f *mat._43;
		//	return ret;
		//}

		// -  / 
		Vector2& operator+=(const Vector2& v)
		{
			if (&v != this)
			{
				x += v.x;
				y += v.y;
 			}
			return *this;
		}
		Vector2& operator-=(const Vector2& v)
		{
			if (&v != this)
			{
				x -= v.x;
				y -= v.y;
 			}
			return *this;
		}

		Vector2& operator*=(float f)
		{
			x *= f;
			y *= f;
 			return *this;
		}
		Vector2& operator/=(float f)
		{
			float t = 1.0f / f;
			x *= t;
			y *= t;
 			return *this;
		}

		//-=,*=,/=
		constexpr float Dot(const Vector2& v)
		{
			return (x * v.x) + (y * v.y)  ;
		}

		//constexpr Vector2 Cross(const Vector2& v)
		//{
		//	Vector3 ret;
		//	ret.x = (y * v.x) - (z * v.y);
		//	ret.y = (z*v.x) - (x * v.z);
		//	return ret;
		//}



	};




}// namespace H Math
 
#endif // !HUGE_MATH_VECTOR3_H
