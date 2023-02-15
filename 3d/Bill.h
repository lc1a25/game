#pragma once
#include "Object3d.h"


class Bill
{
	Model* model_ = nullptr;
	DirectX::XMVECTOR velocity_ = { 0,0,0 };
	Object3d* bill = bill->Create();

	float cameraZ;
	bool billDeadFlag = false;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	void Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetCameraZ(float cameraZ) { this->cameraZ = cameraZ; }

	BOOL billDead()
	{
		return billDeadFlag;
	}
};
