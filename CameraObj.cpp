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
	
	time = timeGetTime();

	start = this->worldPos;

	startIndex = 0;
	targetIndex = 2;

	pointsLast = false;
	pointsStart = true;

	eyeVecTemp = { 0.0f,0.0f,0.0f };
	eyeVec = { 0.0f,0.0f,0.0f };

	targetVecTemp = { 0.0f,0.0f,0.0f };
	targetVec = { 0.0f,0.0f,0.0f };

	eye = { start2 };

	////前方ベクトル
	//XMVECTOR forward({ 0, 0, 1 });
	////回転(前方ベクトル)
	//forward = Matrix4::transform(forward, cameraObj->matWorld);
	//target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
	//target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
	//target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
	//targetEnd = eye + forward;

}

void CameraObj::UpdateCamera()
{

	/*eye = XMVECTOR({ cameraObj->matWorld.r[3].m128_f32[0],
							 cameraObj->matWorld.r[3].m128_f32[1],
							 cameraObj->matWorld.r[3].m128_f32[2] });*/
	nowCount = timeGetTime();

	elapsedCount = nowCount - startCount;

	float elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

	timeRate = elapsedTime / maxTime;

	if (pointsStart == false)
	{
		if (pointsLast == false)//道中　雑魚戦
		{
			//前方ベクトル
			XMVECTOR forward({ 0, 0, 1 });
			//回転(前方ベクトル)
			forward = Matrix4::transform(forward, cameraObj->matWorld);
			targetEnd = eye + forward;


			if (timeRate >= 1.0f)
			{
				if (startIndex < points.size() - 3)
				{
					startIndex++;
					targetIndex++;
					timeRate = 0.0f;
					startCount = timeGetTime();
				}
				else
				{
					/*startIndex = 0;
					timeRate = 0.0f;*/
					startIndex++;
					pointsLast = true;
				}
				if (targetIndex > points.size() - 3)
				{
					pointsLast = true;
					targetIndex = points.size() - 1;
				}


			}
			eye = splinePosition(points, startIndex, timeRate);
			//target = splinePosition(points, targetIndex, timeRate);

			//カメラの移動量
			eyeVec.x = eye.m128_f32[0] - eyeVecTemp.x;
			eyeVec.y = eye.m128_f32[1] - eyeVecTemp.y;
			eyeVec.z = eye.m128_f32[2] - eyeVecTemp.z;

			eyeVecTemp.x = eye.m128_f32[0];
			eyeVecTemp.y = eye.m128_f32[1];
			eyeVecTemp.z = eye.m128_f32[2];

			//カメラの移動量
			targetVec.x = target.m128_f32[0] - eye.m128_f32[0];
			targetVec.y = target.m128_f32[1] - eye.m128_f32[1];
			targetVec.z = target.m128_f32[2] - eye.m128_f32[2];

			targetVecTemp.x = target.m128_f32[0];
			targetVecTemp.y = target.m128_f32[1];
			targetVecTemp.z = target.m128_f32[2];



			target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
			target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
			target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
		}
		else if (pointsLast == true)//最後まで行ったら視点を固定　ボス戦
		{
			//cameraObj->position.z+= 0.1f;
		//cameraObj->rotation.z++;
		//eye = { cameraObj->matWorld.r[3].m128_f32[0],cameraObj->matWorld.r[3].m128_f32[1],cameraObj->matWorld.r[3].m128_f32[2] };

			eye = { end };
			eyeVec = { 0,0,0 };
			//前方ベクトル
			XMVECTOR forward({ 0, 0, 1 });
			//回転(前方ベクトル)
			forward = Matrix4::transform(forward, cameraObj->matWorld);
			target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
			target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
			target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
			targetEnd = eye + forward;
		}
	}
	else
	{
		//前方ベクトル
		XMVECTOR forward({ 0, 0, 1 });
		//回転(前方ベクトル)
		forward = Matrix4::transform(forward, cameraObj->matWorld);
		target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
		target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
		target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
		targetEnd = eye + forward;
	}
	//上方ベクトル
	XMVECTOR upV({ 0,1,0 });
	//回転(上方ベクトル)
	up.x = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[0];
	up.y = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[1];
	up.z = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[2];

	cameraObj->SetPosition({
		//カメラオブジェクトの位置
		cameraObj->matWorld.r[3].m128_f32[0] = eye.m128_f32[0],
		cameraObj->matWorld.r[3].m128_f32[1] = eye.m128_f32[1],
		cameraObj->matWorld.r[3].m128_f32[2] = eye.m128_f32[2] });

	worldTransform = cameraObj->matWorld;

	//ビュー行列用に変換
	eyeView.x = eye.m128_f32[0];
	eyeView.y = eye.m128_f32[1];
	eyeView.z = eye.m128_f32[2];

	targetView.x = target.m128_f32[0];
	targetView.y = target.m128_f32[1];
	targetView.z = target.m128_f32[2];

	
	// ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eyeView), XMLoadFloat3(&targetView), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Win::window_width / Win::window_height,
		0.1f, 1800.0f
	);

	matViewProjection = matView * matProjection;
	//UpdateViewMatrix();
	cameraObj->Update();
}

 XMVECTOR CameraObj::splinePosition(const std::vector<XMVECTOR>& points, size_t startIndex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex > n)
	{
		return points[n];
	}
	if (startIndex < 1)
	{
		return points[1];
	}

	XMVECTOR p0 = points[startIndex - 1];
	XMVECTOR p1 = points[startIndex];
	XMVECTOR p2 = points[startIndex + 1];
	XMVECTOR p3 = points[startIndex + 2];

	XMVECTOR position = ((2 * p1 + (-p0 + p2) * t) +
		((2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t) +
		((-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t)) * 0.5;



	return position;
}

 void CameraObj::UpdateViewMatrix()
 {
	 // 視点座標
	 XMVECTOR eyePosition = XMLoadFloat3(&eyeBill);
	 // 注視点座標
	 XMVECTOR targetPosition = XMLoadFloat3(&targetBill);
	 // （仮の）上方向
	 XMVECTOR upVector = XMLoadFloat3(&up);

	 // カメラZ軸（視線方向）
	 XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	 // 0ベクトルだと向きが定まらないので除外
	 assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	 assert(!XMVector3IsInfinite(cameraAxisZ));
	 assert(!XMVector3Equal(upVector, XMVectorZero()));
	 assert(!XMVector3IsInfinite(upVector));
	 // ベクトルを正規化
	 cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	 // カメラのX軸（右方向）
	 XMVECTOR cameraAxisX;
	 // X軸は上方向→Z軸の外積で求まる
	 cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	 // ベクトルを正規化
	 cameraAxisX = XMVector3Normalize(cameraAxisX);

	 // カメラのY軸（上方向）
	 XMVECTOR cameraAxisY;
	 // Y軸はZ軸→X軸の外積で求まる
	 cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	 // ここまでで直交した3方向のベクトルが揃う
	 //（ワールド座標系でのカメラの右方向、上方向、前方向）	

	 // カメラ回転行列
	 XMMATRIX matCameraRot;
	 // カメラ座標系→ワールド座標系の変換行列
	 matCameraRot.r[0] = cameraAxisX;
	 matCameraRot.r[1] = cameraAxisY;
	 matCameraRot.r[2] = cameraAxisZ;
	 matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	 // 転置により逆行列（逆回転）を計算
	 matView = XMMatrixTranspose(matCameraRot);

	 // 視点座標に-1を掛けた座標
	 XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	 // カメラの位置からワールド原点へのベクトル（カメラ座標系）
	 XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	 XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	 XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	 // 一つのベクトルにまとめる
	 XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	 // ビュー行列に平行移動成分を設定
	 matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	 // ビルボード行列
	 matBillboard.r[0] = cameraAxisX;
	 matBillboard.r[1] = cameraAxisY;
	 matBillboard.r[2] = cameraAxisZ;
	 matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りビルボード行列の計算
	 // カメラX軸、Y軸、Z軸
	 XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	 // X軸は共通
	 ybillCameraAxisX = cameraAxisX;
	 // Y軸はワールド座標系のY軸
	 ybillCameraAxisY = XMVector3Normalize(upVector);
	 // Z軸はX軸→Y軸の外積で求まる
	 ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	 // Y軸回りビルボード行列
	 matBillboardY.r[0] = ybillCameraAxisX;
	 matBillboardY.r[1] = ybillCameraAxisY;
	 matBillboardY.r[2] = ybillCameraAxisZ;
	 matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
 }


