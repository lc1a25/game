#include "Vector3.h"
#include <cmath>




Vector3::Vector3(float x, float y, float z)
{
}

float Vector3::length() const
{
	return (float)sqrt(x * x + y * y + z * z);
}

Vector3 &Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3 &v) const
{
	return Vector3(y * v.z - z * v.y, z *v.x - x * v.z, x *v.y - y * v.x);
}



Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(x * -1, y * -1, z * -1);
}

Vector3 &Vector3::operator+=(const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 &Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 &Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3 transform(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result
	{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w

	};
	return result;
}

Vector3 transform2(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result
	{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2]) / w

	};
	return result;
}


const Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3 &v, float s)
{
	Vector3 temp(v);

	return temp *= s;
}

const Vector3 operator*(float s, const Vector3 &v)
{
	return v * s;
}


const Vector3 operator/(const Vector3 &v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

