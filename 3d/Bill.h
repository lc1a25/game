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
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	void Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetCameraZ(float cameraZ) { this->cameraZ = cameraZ; }

	BOOL billDead()
	{
		return billDeadFlag;
	}
};
