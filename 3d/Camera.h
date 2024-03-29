#pragma once
#include "Win.h"
#include <DirectXMath.h>

class Camera
{
private:
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: 省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void Init();

	void UpdateCamera();

	void UpdateMatView();

	void CameraRot(float anglex, float angley);

	void CameraMoveVector(XMFLOAT3 move, bool addFrag);

	//getter
	const XMMATRIX& GetMatViewProjection() { return matViewProjection; }

	const XMFLOAT3& GetEye() { return eye; }
	const float& GetEyex() { return eye.x; }
	const float& GetEyey() { return eye.y; }
	const float& GetEyez() { return eye.z; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMFLOAT3& GetUp() { return up; }

	/// <summary>
/// ビュー射影行列の取得
/// </summary>
/// <returns>ビュー射影行列</returns>
	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// ビルボード行列の取得
	/// </summary>
	/// <returns>ビルボード行列</returns>
	inline const XMMATRIX& GetBillboardMatrix() {
		return matBillboard;
	}

	//setter
	void SetEye(XMFLOAT3 eye) 
	{ 
		this->eye = eye;  
	}
	void SetTarget(XMFLOAT3 target) 
	{ this->target = target; 
	  UpdateCamera();
	}
	void SetUp(XMFLOAT3 up)
	{
		this->up = up;
		UpdateCamera();
	}

	void UpdateViewMatrix();
	// 視点座標
	XMFLOAT3 eye = { 0,0,-50 };
private:

	float distance = 20.0f;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;

	// 注視点座標
	XMFLOAT3 target = { 0,0,0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0,1,0 };
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	XMMATRIX matViewProjection;

	// ビルボード行列
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();

};
