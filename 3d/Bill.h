#pragma once
#include "Object3d.h"
#include <DirectXMath.h>


class Bill
{
	Model* model_ = nullptr;
	DirectX::XMVECTOR velocity_ = { 0,0,0 };
	Object3d* bill = bill->Create();

	float cameraZ;
	bool billDeadFlag = false;
	DirectX::XMFLOAT3 billRotation;
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
};
