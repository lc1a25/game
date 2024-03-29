#pragma once
#include "Object3d.h"
#include <DirectXMath.h>


class Bill
{
	Model* model_ = nullptr;
	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT3 scale_;
	DirectX::XMVECTOR velocity_ = { 0,0,0 };

	Object3d* bill = bill->Create();

	float cameraZ;
	bool billDeadFlag = false;
	DirectX::XMFLOAT3 billRotation;

	std::list<std::unique_ptr<Bill>> bills;

	int randBill = 0;//ビルをランダム配置するときにrandの数値をいれる(scale用)
	float billScaleY = 13;//ビルの縦方向のスケール
	int randBillRot = 0;//ビルをランダム配置するときにrandの数値をいれる(rotation用)
	//DirectX::XMFLOAT3 billRotation;//ビルのローテーション
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	void Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 billRotation = {0.0f,0.0f,0.0f });

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetCameraZ(float cameraZ) { this->cameraZ = cameraZ; }

	void SetBillDeadFlag(bool billDeadFlag) { this->billDeadFlag = billDeadFlag; }

	DirectX::XMFLOAT3 GetPos() { return bill->GetPosition(); }

	BOOL billDead()
	{
		return billDeadFlag;
	}
};
