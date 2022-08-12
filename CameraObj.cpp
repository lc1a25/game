#include "CameraObj.h"

using namespace DirectX;

//XMMATRIX CameraObj::matView{};
//XMMATRIX CameraObj::matProjection{};
//XMFLOAT3 CameraObj::eye = { 0, 0, -50.0f };
//XMFLOAT3 CameraObj::target = { 0, 0, 0 };
//XMFLOAT3 CameraObj::up = { 0, 1, 0 };
//XMMATRIX CameraObj::matViewProjection{};

void CameraObj::Init(XMVECTOR worldPos, XMFLOAT3 rotation)
{
	//cameraObj->matWorld.r[3].m128_f32[0] = worldPos.m128_f32[0];
	cameraObj->SetPosition({ cameraObj->matWorld.r[3].m128_f32[0],	cameraObj->matWorld.r[3].m128_f32[1] ,	cameraObj->matWorld.r[3].m128_f32[2] });
	this->worldPos = worldPos;
	this->rotation = rotation;


	//camera->Init();
	//// �r���[�s��̐���
	//matView = XMMatrixLookAtLH(
	//	XMLoadFloat3(&eye),
	//	XMLoadFloat3(&target),
	//	XMLoadFloat3(&up));

	//// ���s���e�ɂ��ˉe�s��̐���
	////constMap->mat = XMMatrixOrthographicOffCenterLH(
	////	0, window_width,
	////	window_height, 0,
	////	0, 1);
	//// �������e�ɂ��ˉe�s��̐���
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(60.0f),
	//	(float)Win::window_width / Win::window_height,
	//	0.1f, 1000.0f
	//);

	//matViewProjection = matView * matProjection;
}

void CameraObj::UpdateCamera()
{
	
	eye = cameraObj->GetEye();
	target = cameraObj->GetTarget();
	up = cameraObj->GetUp();

	eye = XMFLOAT3({ cameraObj->matWorld.r[3].m128_f32[0],
					 cameraObj->matWorld.r[3].m128_f32[1],
					 cameraObj->matWorld.r[3].m128_f32[2] });

	XMVECTOR forward({ 0, 0, 1 });

	forward = Matrix4::transform(forward, cameraObj->matWorld);

	target.x = eye.x + forward.m128_f32[0];
	target.y = eye.y + forward.m128_f32[1];
	target.z = eye.z + forward.m128_f32[2];

	XMVECTOR upV({ 0,1,0 });
	up.x = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[0];
	up.y = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[1];
	up.z = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[2];

	cameraObj->SetEyeCamera(eye);
	cameraObj->SetTargetCamera(target);
	cameraObj->SetUpCamera(up);

	cameraObj->Update();
	//XMMATRIX matRot, matTrans;
	////��]�A���s�ړ��s��̌v�Z
	//
	//matRot = XMMatrixIdentity();
	//rotation.z++;
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	//matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	//matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	////worldPos.m128_f32[0] = matWorld.r[3].m128_f32[0];
	////worldPos.m128_f32[1] = matWorld.r[3].m128_f32[1];
	////worldPos.m128_f32[2] = matWorld.r[3].m128_f32[2];

	//matTrans = XMMatrixTranslation(worldPos.m128_f32[0],worldPos.m128_f32[1],worldPos.m128_f32[2]);

	//worldTransform *= matRot;
	//worldTransform *= matTrans;
	//// �r���[�s��̍X�V
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(60.0f),
	//	(float)Win::window_width / Win::window_height,
	//	0.1f, 1000.0f
	//);

	//matViewProjection = matView * matProjection;
}

void CameraObj::CameraRot(float anglex, float angley)
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
	//SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	//SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
}

void CameraObj::CameraMoveVector(XMFLOAT3 move, bool addFrag)
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

	/*SetEye(eye_moved);
	SetTarget(target_moved);*/
}

//void CameraObj::CameraMoveVectorAdd(XMFLOAT3 move)
//{
//	XMFLOAT3 eye_moved = GetEye();
//	XMFLOAT3 target_moved = GetTarget();
//
//	eye_moved.x += move.x;
//	eye_moved.y += move.y;
//	eye_moved.z += move.z;
//
//	target_moved.x += move.x;
//	target_moved.y += move.y;
//	target_moved.z += move.z;
//
//	SetEye(eye_moved);
//	SetTarget(target_moved);
//}
//
//void CameraObj::CameraMoveVectorSub(XMFLOAT3 move)
//{
//	XMFLOAT3 eye_moved = GetEye();
//	XMFLOAT3 target_moved = GetTarget();
//
//	eye_moved.x -= move.x;
//	eye_moved.y -= move.y;
//	eye_moved.z -= move.z;
//
//	target_moved.x -= move.x;
//	target_moved.y -= move.y;
//	target_moved.z -= move.z;
//
//	SetEye(eye_moved);
//	SetTarget(target_moved);
//}
