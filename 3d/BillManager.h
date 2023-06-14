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

	int randBill = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(scale�p)
	float billScaleY = 13;//�r���̏c�����̃X�P�[��
	int randBillRot = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(rotation�p)


public:
	void BillSet(Model* model);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
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

