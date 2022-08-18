#include "CameraObj.h"

using namespace DirectX;

//XMMATRIX CameraObj::matView{};
//XMMATRIX CameraObj::matProjection{};
//XMFLOAT3 CameraObj::eye = { 0, 0, -150.0f };
//XMFLOAT3 CameraObj::target = { 0, 0, 0 };
//XMFLOAT3 CameraObj::up = { 0, 1, 0 };
//XMMATRIX CameraObj::matViewProjection{};

void CameraObj::Init(XMVECTOR worldPos, XMFLOAT3 rotation)
{
	this->worldPos = worldPos;
	cameraObj->matWorld.r[3].m128_f32[0] = this->worldPos.m128_f32[0];
	cameraObj->matWorld.r[3].m128_f32[1] = this->worldPos.m128_f32[1];
	cameraObj->matWorld.r[3].m128_f32[2] = this->worldPos.m128_f32[2];

	cameraObj->SetPosition({ cameraObj->matWorld.r[3].m128_f32[0],	cameraObj->matWorld.r[3].m128_f32[1] ,	cameraObj->matWorld.r[3].m128_f32[2] });

	this->rotation = rotation;


	//camera->Init();
	// ビュー行列の生成
	/*matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));*/
	// 透視投影による射影行列の生成
	/*matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Win::window_width / Win::window_height,
		0.1f, 1000.0f
	);

	matViewProjection = matView * matProjection;*/

	
}

void CameraObj::UpdateCamera()
{
	
	//cameraObj->matWorld.r[3].m128_f32[2];
	//cameraObj->rotation.z++;
	
	eye = XMFLOAT3({ cameraObj->matWorld.r[3].m128_f32[0],
					 cameraObj->matWorld.r[3].m128_f32[1],
					 cameraObj->matWorld.r[3].m128_f32[2] });
	//前方ベクトル
	XMVECTOR forward({ 0, 0, 1 });
	//回転(前方ベクトル)
	forward = Matrix4::transform(forward, cameraObj->matWorld);

	target.x = eye.x + forward.m128_f32[0];
	target.y = eye.y + forward.m128_f32[1];
	target.z = eye.z + forward.m128_f32[2];
	//上方ベクトル
	XMVECTOR upV({ 0,1,0 });
	//回転(上方ベクトル)
	up.x = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[0];
	up.y = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[1];
	up.z = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[2];

	


	//matViewProjection = matView * matProjection;

	//cameraObj->matView = matViewProjection;

	cameraObj->Update();
}

void CameraObj::CameraRot(float anglex, float angley)
{
	
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
