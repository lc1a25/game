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
// 座標変換（ベクトルと行列の掛け算をする）
Vector3 transform(const Vector3& v, const DirectX::XMMATRIX& m);
// 座標変換（ベクトルと行列の掛け算をする）平行移動をむし
Vector3 transform2(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
const Vector3 operator*(const Vector3 &v, float s);
const Vector3 operator*(float s, const Vector3 &v);
const Vector3 operator/(const Vector3 &v, float s);