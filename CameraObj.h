#pragma once
//#include "Win.h"
//#include <DirectXMath.h>
#include "Matrix4.h"
#include "Object3d.h"

class CameraObj
{
private:
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: 省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	Object3d* cameraObj = cameraObj->Create();
public:
	void Init(XMVECTOR worldPos,XMFLOAT3 rotation);

	void UpdateCamera();

	void CameraRot(float anglex, float angley);

	void CameraMoveVector(XMFLOAT3 move, bool addFrag);

	//void CameraMoveVectorAdd(XMFLOAT3 move);

	//void CameraMoveVectorSub(XMFLOAT3 move);

	//getter
	const XMMATRIX& GetMatViewProjection() { return matViewProjection; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatView() { return matView; }

	XMFLOAT3& GetEye() { return eye; }
	const float& GetEyex() { return eye.x; }
	const float& GetEyey() { return eye.y; }
	const float& GetEyez() { return eye.z; }
	const XMFLOAT3& GetTarget() { return target; }

	const float& GetpositionX() { return cameraObj->position.x; }

	//setter
	/*void SetEye(XMFLOAT3 eye)
	{
		this->eye = eye;
		UpdateCamera();
	}
	void SetTarget(XMFLOAT3 target)
	{
		this->target = target;
		UpdateCamera();
	}
	void SetUp(XMFLOAT3 up)
	{
		this->up = up;
		UpdateCamera();
	}*/

	void SetWorldTransform(XMMATRIX& cameraWorldTransform)
	{
		worldTransform = cameraWorldTransform;
	}

	XMMATRIX GetWorldTransform()
	{
		return worldTransform;
	}
	// 視点座標
	XMFLOAT3 eye = { 0,0,-50 };
private:

	float distance = 20.0f;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	
	// 注視点座標
	 XMFLOAT3 target = {0,0,0};
	// 上方向ベクトル
	 XMFLOAT3 up = {0,1,0};
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	 XMMATRIX matViewProjection;

	XMMATRIX worldTransform ;

	//ワールド座標
	XMVECTOR worldPos;

	// X,Y,Z軸回りの回転角
	XMFLOAT3 rotation = { 0,0,0 };

};
