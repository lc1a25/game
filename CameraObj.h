#pragma once
//#include "Win.h"
//#include <DirectXMath.h>
#include "Matrix4.h"
#include "Object3d.h"

class CameraObj
{
private:
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: �ȗ�
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
	// ���_���W
	XMFLOAT3 eye = { 0,0,-50 };
private:

	float distance = 20.0f;
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;
	
	// �����_���W
	 XMFLOAT3 target = {0,0,0};
	// ������x�N�g��
	 XMFLOAT3 up = {0,1,0};
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	 XMMATRIX matViewProjection;

	XMMATRIX worldTransform ;

	//���[���h���W
	XMVECTOR worldPos;

	// X,Y,Z�����̉�]�p
	XMFLOAT3 rotation = { 0,0,0 };

};
