
#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;
class Matrix4
{
	public:	
	//XMMATRIX matrix4;
	
	// 単位行列を求める
	XMMATRIX identity();

	// 拡大縮小行列の設定
	XMMATRIX scale(const Vector3& s);

	// 回転行列の設定
	XMMATRIX rotateX(float angle);
	XMMATRIX rotateY(float angle);
	XMMATRIX rotateZ(float angle);

	// 平行移動行列の作成
	XMMATRIX translate(const Vector3& t);

	// 座標変換（ベクトルと行列の掛け算をする）
	static XMVECTOR transform(const XMVECTOR& v, const XMMATRIX& m);

	// 代入演算子オーバーロード
	static XMMATRIX matrixMatrix(XMMATRIX m1, const XMMATRIX m2);
	
};

// 代入演算子オーバーロード
XMMATRIX& operator*=(XMMATRIX& m1, const XMMATRIX& m2);

// 2項演算子オーバーロード
XMMATRIX operator*(const XMMATRIX& m1, const XMMATRIX& m2);

Vector3 operator*(const Vector3& v, const XMMATRIX& m);
