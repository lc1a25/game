#include "Camera.h"
using namespace DirectX;

void Camera::Init()
{
	// �r���[�s��̐���
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// ���s���e�ɂ��ˉe�s��̐���
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Win::window_width / Win::window_height,
		0.1f, 1000.0f
	);

	matViewProjection = matView * matProjection;
}

void Camera::UpdateCamera()
{
	

	// �r���[�s��̍X�V
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Win::window_width / Win::window_height,
		0.1f, 1000.0f
	);

	matViewProjection = matView * matProjection;
}

void Camera::CameraRot(float anglex, float angley)
{
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-anglex);
	matRotNew *= XMMatrixRotationY(-angley);


	matRot = matRotNew * matRot;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	XMVECTOR vTargetEye = { 0.0f, 0.0f, distance, 1.0f };
	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	// �x�N�g������]
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	// �����_���炸�炵���ʒu�Ɏ��_���W������
	const XMFLOAT3& target = GetTarget();
	SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
}

void Camera::CameraMoveVector(XMFLOAT3 move, bool addFrag)
{
	if (addFrag == false)
	{
		move.x = move.x * -1;
		move.y = move.y * -1;
		move.z = move.z * -1;
	}

	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

		eye_moved.x += move.x;
		eye_moved.y += move.y;
		eye_moved.z += move.z;

		target_moved.x += move.x;
		target_moved.y += move.y;
		target_moved.z += move.z;
			
	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveVectorAdd(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveVectorSub(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x -= move.x;
	eye_moved.y -= move.y;
	eye_moved.z -= move.z;

	target_moved.x -= move.x;
	target_moved.y -= move.y;
	target_moved.z -= move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}
