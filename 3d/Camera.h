#pragma once
#include "Win.h"
#include <DirectXMath.h>

class Camera
{
private:
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: �ȗ�
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
/// �r���[�ˉe�s��̎擾
/// </summary>
/// <returns>�r���[�ˉe�s��</returns>
	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// �r���{�[�h�s��̎擾
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
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
	// ���_���W
	XMFLOAT3 eye = { 0,0,-50 };
private:

	float distance = 20.0f;
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;

	// �����_���W
	XMFLOAT3 target = { 0,0,0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0,1,0 };
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	XMMATRIX matViewProjection;

	// �r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();

};
