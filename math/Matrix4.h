
#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

//using namespace DirectX;
class Matrix4
{
	public:	
	//XMMATRIX matrix4;
	
	// 単位行列を求める
	DirectX::XMMATRIX identity();

	// 拡大縮小行列の設定
	DirectX::XMMATRIX scale(const Vector3& s);

	// 回転行列の設定
	DirectX::XMMATRIX rotateX(float angle);
	DirectX::XMMATRIX rotateY(float angle);
	DirectX::XMMATRIX rotateZ(float angle);

	// 平行移動行列の作成
	DirectX::XMMATRIX translate(const Vector3& t);

	// 座標変換（ベクトルと行列の掛け算をする）
	static DirectX::XMVECTOR transform(const DirectX::XMVECTOR& v, const DirectX::XMMATRIX& m);

	//スクリーン座標からワールド座標に
	static DirectX::XMVECTOR transformScreenToWorld(const DirectX::XMVECTOR& v, const DirectX::XMMATRIX& m);

	// 代入演算子オーバーロード
	static DirectX::XMMATRIX matrixMatrix(DirectX::XMMATRIX& m1, const DirectX::XMMATRIX& m2);

	static DirectX::XMMATRIX matrixInverse(DirectX::XMMATRIX& m);
	
};

//// 代入演算子オーバーロード
//XMMATRIX& operator*=(XMMATRIX& m1, const XMMATRIX& m2);
//
//// 2項演算子オーバーロード
//XMMATRIX operator*(const XMMATRIX& m1, const XMMATRIX& m2);
//
//Vector3 operator*(const Vector3& v, const XMMATRIX& m);
