#include "Precompiled.h"
#include "Matrix4.h"
using namespace H;
using namespace H::Math;

	//constructor
	//constexpr Matrix4::Matrix4() noexcept
	//	 : _11(0.0f), _12(0.0f), _13(0.0f), _14(0.0f),
	//	_21(0.0f), _22(0.0f), _23(0.0f), _24(0.0f),
	//	_31(0.0f), _32(0.0f), _33(0.0f), _34(0.0f),
	//	_41(0.0f), _42(0.0f), _43(0.0f), _44(0.0f)
	//{}
	///*constexpr*/ Matrix4::Matrix4(float f11, float f12, float f13, float f14, float f21, float f22, float f23, float f24, float f31, float f32, float f33, float f34, float f41, float f42, float f43, float f44) noexcept
	//	: _11(f11), _12(f12), _13(f13), _14(f14),
	//	_21(f21), _22(f22), _23(f23), _24(f24),
	//	_31(f31), _32(f32), _33(f33), _34(f34),
	//	_41(f41), _42(f42), _43(f43), _44(f44)
	//{
	//}
	//constexpr  Matrix4::Matrix4(const Matrix4& other) noexcept
	//	:_11(other._11), _12(other._12), _13(other._13), _14(other._14),
	//	_21(other._21), _22(other._22), _23(other._23), _24(other._24),
	//	_31(other._31), _32(other._32), _33(other._33), _34(other._34),
	//	_41(other._41), _42(other._42), _43(other._43), _44(other._44)
	//{}
 

	//set row column
	//constexpr void Matrix4::SetRow(const Vector3& row, const size_t row_index)
	//{
	//	for (size_t i = 0; i < 3; i++)// vector(x,y,z,1)
	//	{
	//		v[i + row_index*4] = row.v[i];
	//	}
	//}
	//constexpr void Matrix4::SetColumn(const Vector3& column, const size_t col_index)
	//{
	//	for (size_t i = 0; i < 3; i++)// vector(x,y,z,1)
	//	{
	//		v[col_index + 4 * i] = column.v[i];
	//	}

	//}
	//operator
	//bool Matrix4::operator==(const Matrix4& mat)const
	//{
	//	return 
	//		(mat.v[0] == v[0]	)&& (mat.v[1] == v[1]	)&&( mat.v[2] == v[2]  )&&  (mat.v[3] == v[3]) &&
	//		(mat.v[4] == v[4]	)&& (mat.v[5] == v[5]	)&&( mat.v[6] == v[6]  )&&  (mat.v[7] == v[7]) &&
	//		(mat.v[8] == v[8]	)&& (mat.v[9] == v[9]	)&&( mat.v[10] == v[10]) && (mat.v[11] == v[11]) &&
	//		(mat.v[12] == v[12]) && (mat.v[13] == v[13])&& ( mat.v[14] == v[14]) && (mat.v[15] == v[15]);
	//}
	//Matrix4& Matrix4::operator=(const Matrix4& mat)
	//{
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		 v[i] = mat.v[i];
	//	}
	//	return *this;
	//}
	//constexpr Matrix4 Matrix4::operator+(const Matrix4& mat)const
	//{
	//	Matrix4 ret;
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		ret.v[i] = v[i] + mat.v[i];
	//	}
	//	return ret;
	//}
	//constexpr Matrix4 Matrix4::operator-(const Matrix4& mat)const
	//{
	//	Matrix4 ret;
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		ret.v[i] = v[i] - mat.v[i];
	//	}
	//	return ret;
	//}
	//Matrix4 Matrix4::operator*(float scalar)const
	//{
	//	Matrix4 ret;
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		ret.v[i] = v[i] * scalar;
	//	}
	//	return ret;
	//
	//}
	//Vector3 Matrix4::operator*(const Vector3& vector)const
	//{
	//	// vector(x,y,z,1)
	//	Vector3 ret;
	//	ret.x = vector.x*_11 + vector.y*_21 + vector.z*_31 + 1.0f *_41;
	//	ret.y = vector.x*_12 + vector.y*_22 + vector.z*_32 + 1.0f *_42;
	//	ret.z = vector.x*_13 + vector.y*_23 + vector.z*_33 + 1.0f *_43;

	//	return ret;
	//}

	// Matrix4 Matrix4::operator*(const Matrix4& other)const
	//{
	//	Matrix4 ret;
	//	ret._11 = (_11 * other._11) + (_12 * other._21) + (_13 * other._31) + (_14 * other._41);
	//	ret._12 = (_11 * other._12) + (_12 * other._22) + (_13 * other._32) + (_14 * other._42);
	//	ret._13 = (_11 * other._13) + (_12 * other._23) + (_13 * other._33) + (_14 * other._43);
	//	ret._14 = (_11 * other._14) + (_12 * other._24) + (_13 * other._34) + (_14 * other._44);

	//	ret._21 = (_21 * other._11) + (_22 * other._21) + (_23 * other._31) + (_24 * other._41);
	//	ret._22 = (_21 * other._12) + (_22 * other._22) + (_23 * other._32) + (_24 * other._42);
	//	ret._23 = (_21 * other._13) + (_22 * other._23) + (_23 * other._33) + (_24 * other._43);
	//	ret._24 = (_21 * other._14) + (_22 * other._24) + (_23 * other._34) + (_24 * other._44);

	//	ret._31 = (_31 * other._11) + (_32 * other._21) + (_33 * other._31) + (_34 * other._41);
	//	ret._32 = (_31 * other._12) + (_32 * other._22) + (_33 * other._32) + (_34 * other._42);
	//	ret._33 = (_31 * other._13) + (_32 * other._23) + (_33 * other._33) + (_34 * other._43);
	//	ret._34 = (_31 * other._14) + (_32 * other._24) + (_33 * other._34) + (_34 * other._44);

	//	ret._41 = (_41 * other._11) + (_42 * other._21) + (_43 * other._31) + (_44 * other._41);
	//	ret._42 = (_41 * other._12) + (_42 * other._22) + (_43 * other._32) + (_44 * other._42);
	//	ret._43 = (_41 * other._13) + (_42 * other._23) + (_43 * other._33) + (_44 * other._43);
	//	ret._44 = (_41 * other._14) + (_42 * other._24) + (_43 * other._34) + (_44 * other._44);
	//	return ret;
	//}
	//Matrix4& Matrix4::operator+=(const Matrix4& mat)
	//{
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		v[i] += mat.v[i];
	//	}
	//	return *this;
	//}
	//Matrix4& Matrix4::operator-=(const Matrix4& mat)
	//{
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		v[i] -= mat.v[i];
	//	}
	//	return *this;
	//}
	//Matrix4& Matrix4::operator*=(const Matrix4& other)
	//{
	//	_11 = (_11 * other._11) + (_12 * other._21) + (_13 * other._31) + (_14 * other._41);
	//	_12 = (_11 * other._12) + (_12 * other._22) + (_13 * other._32) + (_14 * other._42);
	//	_13 = (_11 * other._13) + (_12 * other._23) + (_13 * other._33) + (_14 * other._43);
	//	_14 = (_11 * other._14) + (_12 * other._24) + (_13 * other._34) + (_14 * other._44);

	//	_21 = (_21 * other._11) + (_22 * other._21) + (_23 * other._31) + (_24 * other._41);
	//	_22 = (_21 * other._12) + (_22 * other._22) + (_23 * other._32) + (_24 * other._42);
	//	_23 = (_21 * other._13) + (_22 * other._23) + (_23 * other._33) + (_24 * other._43);
	//	_24 = (_21 * other._14) + (_22 * other._24) + (_23 * other._34) + (_24 * other._44);

	//	_31 = (_31 * other._11) + (_32 * other._21) + (_33 * other._31) + (_34 * other._41);
	//	_32 = (_31 * other._12) + (_32 * other._22) + (_33 * other._32) + (_34 * other._42);
	//	_33 = (_31 * other._13) + (_32 * other._23) + (_33 * other._33) + (_34 * other._43);
	//	_34 = (_31 * other._14) + (_32 * other._24) + (_33 * other._34) + (_34 * other._44);

	//	_41 = (_41 * other._11) + (_42 * other._21) + (_43 * other._31) + (_44 * other._41);
	//	_42 = (_41 * other._12) + (_42 * other._22) + (_43 * other._32) + (_44 * other._42);
	//	_43 = (_41 * other._13) + (_42 * other._23) + (_43 * other._33) + (_44 * other._43);
	//	_44 = (_41 * other._14) + (_42 * other._24) + (_43 * other._34) + (_44 * other._44);
	//	return *this;

	//}
	//Matrix4& Matrix4::operator*=(float scalar)
	//{
	//	for (size_t i = 0; i < 16; i++)
	//	{
	//		v[i] *= scalar;
	//	}
	//	return *this;
	//}



	//mul vec4
	//Matrix4& Matrix4::Transpose()
	//{
	//	float temp = _12;_12 = _21;_21 = temp;
	//
	//	temp = _13; _13 = _31; _31 = temp;
	//	temp = _14; _14 = _41; _41 = temp;
	//	temp = _23; _23 = _32; _32 = temp;
	//	temp = _24; _24 = _42; _42 = temp;
	//	temp = _34; _34 = _43; _43 = temp;
	//
	//	return *this;
	//}

	// void Matrix4::SetIdentity()
	//{
	//	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	//	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	//	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	//	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	//
	//}


	Matrix4& Matrix4::Translation(const Vector3& translate)
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = translate.x; _42 = translate.y; _43 = translate.z; _44 = 1.0f;
		return *this;
	}
	Matrix4& Matrix4::RotationX(const float angle)
	{
			float cosX = cosf(angle); float sinX = sinf(angle);
			_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
			_21 = 0.0f; _22 = cosX; _23 = -sinX; _24 = 0.0f;
			_31 = 0.0f; _32 = sinX; _33 = cosX; _34 = 0.0f;
			_41 = 0.0f; _42 = 0.0f;_43 = 0.0f;  _44 = 1.0f;
			return *this;
	}
	Matrix4& Matrix4::RotationY(const float angle)
	{
		float cosY = cosf(angle); float sinY = sinf(angle);
		_11 = cosY; _12 = 0.0f; _13 = sinY; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = -sinY; _32 = 0.0f; _33 = cosY; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f;  _44 = 1.0f;
		return *this;
	}

	Matrix4& Matrix4::RotationZ(const float angle)
	{
		float cosZ = cosf(angle); float sinZ = sinf(angle);
		_11 = cosZ; _12 = -sinZ; _13 = 0.0f; _14 = 0.0f;
		_21 = sinZ; _22 = cosZ; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f;  _44 = 1.0f;
		return *this;
	}
	Matrix4& Matrix4::Scaling(const Vector3& scale)
	{
		_11 = scale.x; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = scale.y; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = scale.z; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
		return *this;

	}

	Matrix4& Matrix4::Rotation(const float xr, const float yr, const float zr)
	{
		float cosX = cosf(xr); float sinX = sinf(xr);
		float cosY = cosf(yr); float sinY = sinf(yr);
		float cosZ = cosf(zr); float sinZ = sinf(zr);

		_11 = cosY * cosZ; _12 = -sinZ;			_13 = sinY;			 _14 = 0.0f;
		_21 = sinZ;		   _22 = cosX * cosZ;	_23 = -sinX;		 _24 = 0.0f;
		_31 = -sinY;	   _32 = sinX;			_33 = cosX * cosY;	 _34 = 0.0f;
		_41 = 0.0f;		   _42 = 0.0f;			_43 = 0.0f;			 _44 = 1.0f;


		return *this;
	}

 