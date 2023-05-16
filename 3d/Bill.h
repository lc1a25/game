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

	int randBill = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(scale�p)
	float billScaleY = 13;//�r���̏c�����̃X�P�[��
	int randBillRot = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(rotation�p)
	//DirectX::XMFLOAT3 billRotation;//�r���̃��[�e�[�V����
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	void Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 billRotation = {0.0f,0.0f,0.0f });

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetCameraZ(float cameraZ) { this->cameraZ = cameraZ; }

	void SetBillDeadFlag(bool billDeadFlag) { this->billDeadFlag = billDeadFlag; }

	DirectX::XMFLOAT3 GetPos() { return bill->GetPosition(); }

	BOOL billDead()
	{
		return billDeadFlag;
	}

	FLOAT BillScaleY(int randam);
	DirectX::XMFLOAT3 BillRot(int randam);
	void BillCreate(Model* model);
};
