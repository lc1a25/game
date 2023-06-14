#pragma once
#include "Bill.h"


class BillManager
{
	Model* model_ = nullptr;
	DirectX::XMFLOAT3 pos_;
	DirectX::XMFLOAT3 scale_;
	DirectX::XMVECTOR velocity_ = { 0,0,0 };

	Bill* bill = nullptr;

	DirectX::XMFLOAT3 billRotation;
	float cameraZ;
	bool billDeadFlag = false;
	std::list<std::unique_ptr<Bill>> bills;

	int randBill = 0;//ビルをランダム配置するときにrandの数値をいれる(scale用)
	float billScaleY = 13;//ビルの縦方向のスケール
	int randBillRot = 0;//ビルをランダム配置するときにrandの数値をいれる(rotation用)


public:
	void BillSet(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetBillDeadFlag(bool billDeadFlag) { this->billDeadFlag = billDeadFlag; }
	BOOL billDead()
	{
		return billDeadFlag;
	}
	DirectX::XMFLOAT3 GetPos() { return bill->GetPos(); }

	FLOAT BillScaleY(int randam);
	DirectX::XMFLOAT3 BillRot(int randam);
	void BillCreate(Model* model);
};

