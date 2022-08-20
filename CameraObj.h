#pragma once
//#include "Win.h"
//#include <DirectXMath.h>
#include "Matrix4.h"
#include "Object3d.h"

#pragma comment (lib, "winmm.lib")

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

	XMVECTOR splinePosition(const std::vector<XMVECTOR>& points, size_t startIndex, float t);

	//getter
	const XMMATRIX& GetMatViewProjection() { return matViewProjection; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatView() { return matView; }
	const XMMATRIX& GetMatWorld() { return cameraObj->matWorld; }


	XMFLOAT3& GetEye() { return XMFLOAT3{ eye.m128_f32[0], eye.m128_f32[1], eye.m128_f32[2] }; }
	const XMFLOAT3& GetTarget() { return XMFLOAT3{ target.m128_f32[0],target.m128_f32[1] ,target.m128_f32[2] }; }
	const XMFLOAT3& GetUp() { return up; }

	const float& GetpositionX() { return cameraObj->position.x; }


	const FLOAT& GetTimerate() { return startIndex; }

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



	XMMATRIX GetWorldTransform()
	{
		return worldTransform;
	}

private:

	float distance = 20.0f;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	// 視点座標
	XMVECTOR eye = { 0,0,0 };
	// 注視点座標
	XMVECTOR target = {0,0,0};
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

	DWORD time = 0;

	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	XMVECTOR start{ 0.0f, 0.0f, 0.0f };
	XMVECTOR p2{ -50.0f, 50.0f, +50.0f };
	XMVECTOR p3{ 0.0f, 50.0f, +70.0f };
	XMVECTOR p4{ +50.0f, -30.0f, -50.0f };
	XMVECTOR p5{ +50.0f, 20.0f, -25.0f };
	XMVECTOR p6{ +30.0f, 40.0f, 50.0f };
	XMVECTOR p7{ -40.0f, 30.0f, 20.0f };

	XMVECTOR end{ -100.0f, 0.0f, 0.0f };
	float maxTime = 5.0f;
	float timeRate = 0.0f;

	std::vector<XMVECTOR>points{ start,start,p2,p3,p4,p5,p6,p7,end,end };

	int startIndex = 1;

	//XMVECTOR position{ 0, 0, 0 };
	//XMVECTOR target{ 0, 0, 0 };
};
