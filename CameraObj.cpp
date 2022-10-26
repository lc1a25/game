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

	startIndex = 1;
	targetIndex = 2;
	pointsLast = false;

	eyeVecTemp = { 0.0f,0.0f,0.0f };
	eyeVec = { 0.0f,0.0f,0.0f };

	targetVecTemp = { 0.0f,0.0f,0.0f };
	targetVec = { 0.0f,0.0f,0.0f };

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

	if (pointsLast == false)//道中　雑魚戦
	{
		//前方ベクトル
		XMVECTOR forward({ 0, 0, 1 });
		//回転(前方ベクトル)
		forward = Matrix4::transform(forward, cameraObj->matWorld);
		targetEnd = eye + forward;
		if (timeRate >= 1.0f)
		{
			if (startIndex < points.size() - 4)
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
	else//最後まで行ったら視点を固定　ボス戦
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

	////前方ベクトル
	//XMVECTOR forward({ 0, 0, 1 });
	////回転(前方ベクトル)
	//forward = Matrix4::transform(forward, cameraObj->matWorld);
	//target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
	//target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
	//target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
	////targetEnd = eye + forward;

	//上方ベクトル
	XMVECTOR upV({ 0,1,0 });
	//回転(上方ベクトル)
	up.x = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[0];
	up.y = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[1];
	up.z = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[2];

	//カメラオブジェクトの位置
	cameraObj->matWorld.r[3].m128_f32[0] = eye.m128_f32[0];
	cameraObj->matWorld.r[3].m128_f32[1] = eye.m128_f32[1];
	cameraObj->matWorld.r[3].m128_f32[2] = eye.m128_f32[2];

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
		0.1f, 1100.0f
	);

	matViewProjection = matView * matProjection;
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


