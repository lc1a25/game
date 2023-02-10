#pragma once
#include <DirectXMath.h>

class Vector3 
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3(float x, float y,float z);

	float length() const;
	Vector3 &normalize();
	float dot(const Vector3 &v) const;
	Vector3 cross(const Vector3 &v) const;

	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3 &operator+= (const Vector3 &v);
	Vector3 &operator-= (const Vector3 &v);
	Vector3 &operator*= (float s);
	Vector3 &operator/= (float s);

	
};
// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
Vector3 transform(const Vector3& v, const DirectX::XMMATRIX& m);
// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j���s�ړ����ނ�
Vector3 transform2(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator*(const Vector3 &v, float s);
const Vector3 operator*(float s, const Vector3 &v);
const Vector3 operator/(const Vector3 &v, float s);