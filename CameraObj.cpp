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
}

void CameraObj::UpdateCamera()
{

	nowCount = timeGetTime();

	elapsedCount = nowCount - startCount;

	float elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

	timeRate = elapsedTime / maxTime;

	if (pointsLast == false)
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
				pointsLast = true;
			}
			if (targetIndex > points.size() - 4)
			{
				pointsLast = true;
				targetIndex = points.size() - 1;
			}

		}
		eye = splinePosition(points, startIndex, timeRate);
		//target = splinePosition(points, targetIndex, timeRate);

		target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
		target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
		target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
	}
	else
	{
		//cameraObj->position.z+= 0.1f;
	//cameraObj->rotation.z++;
	//eye = { cameraObj->matWorld.r[3].m128_f32[0],cameraObj->matWorld.r[3].m128_f32[1],cameraObj->matWorld.r[3].m128_f32[2] };



		
			eye = { end };
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

	worldTransform = cameraObj->matWorld;



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


