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

	////�O���x�N�g��
	//XMVECTOR forward({ 0, 0, 1 });
	////��](�O���x�N�g��)
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
		if (pointsLast == false)//�����@�G����
		{
			//�O���x�N�g��
			XMVECTOR forward({ 0, 0, 1 });
			//��](�O���x�N�g��)
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

			//�J�����̈ړ���
			eyeVec.x = eye.m128_f32[0] - eyeVecTemp.x;
			eyeVec.y = eye.m128_f32[1] - eyeVecTemp.y;
			eyeVec.z = eye.m128_f32[2] - eyeVecTemp.z;

			eyeVecTemp.x = eye.m128_f32[0];
			eyeVecTemp.y = eye.m128_f32[1];
			eyeVecTemp.z = eye.m128_f32[2];

			//�J�����̈ړ���
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
		else if (pointsLast == true)//�Ō�܂ōs�����王�_���Œ�@�{�X��
		{
			//cameraObj->position.z+= 0.1f;
		//cameraObj->rotation.z++;
		//eye = { cameraObj->matWorld.r[3].m128_f32[0],cameraObj->matWorld.r[3].m128_f32[1],cameraObj->matWorld.r[3].m128_f32[2] };

			eye = { end };
			eyeVec = { 0,0,0 };
			//�O���x�N�g��
			XMVECTOR forward({ 0, 0, 1 });
			//��](�O���x�N�g��)
			forward = Matrix4::transform(forward, cameraObj->matWorld);
			target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
			target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
			target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
			targetEnd = eye + forward;
		}
	}
	else
	{
		//�O���x�N�g��
		XMVECTOR forward({ 0, 0, 1 });
		//��](�O���x�N�g��)
		forward = Matrix4::transform(forward, cameraObj->matWorld);
		target.m128_f32[0] = eye.m128_f32[0] + forward.m128_f32[0];
		target.m128_f32[1] = eye.m128_f32[1] + forward.m128_f32[1];
		target.m128_f32[2] = eye.m128_f32[2] + forward.m128_f32[2];
		targetEnd = eye + forward;
	}
	//����x�N�g��
	XMVECTOR upV({ 0,1,0 });
	//��](����x�N�g��)
	up.x = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[0];
	up.y = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[1];
	up.z = Matrix4::transform(upV, cameraObj->matWorld).m128_f32[2];

	cameraObj->SetPosition({
		//�J�����I�u�W�F�N�g�̈ʒu
		cameraObj->matWorld.r[3].m128_f32[0] = eye.m128_f32[0],
		cameraObj->matWorld.r[3].m128_f32[1] = eye.m128_f32[1],
		cameraObj->matWorld.r[3].m128_f32[2] = eye.m128_f32[2] });

	worldTransform = cameraObj->matWorld;

	//�r���[�s��p�ɕϊ�
	eyeView.x = eye.m128_f32[0];
	eyeView.y = eye.m128_f32[1];
	eyeView.z = eye.m128_f32[2];

	targetView.x = target.m128_f32[0];
	targetView.y = target.m128_f32[1];
	targetView.z = target.m128_f32[2];

	
	// �r���[�s��̍X�V
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
	 // ���_���W
	 XMVECTOR eyePosition = XMLoadFloat3(&eyeBill);
	 // �����_���W
	 XMVECTOR targetPosition = XMLoadFloat3(&targetBill);
	 // �i���́j�����
	 XMVECTOR upVector = XMLoadFloat3(&up);

	 // �J����Z���i���������j
	 XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	 // 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	 assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	 assert(!XMVector3IsInfinite(cameraAxisZ));
	 assert(!XMVector3Equal(upVector, XMVectorZero()));
	 assert(!XMVector3IsInfinite(upVector));
	 // �x�N�g���𐳋K��
	 cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	 // �J������X���i�E�����j
	 XMVECTOR cameraAxisX;
	 // X���͏������Z���̊O�ςŋ��܂�
	 cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	 // �x�N�g���𐳋K��
	 cameraAxisX = XMVector3Normalize(cameraAxisX);

	 // �J������Y���i������j
	 XMVECTOR cameraAxisY;
	 // Y����Z����X���̊O�ςŋ��܂�
	 cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	 // �����܂łŒ�������3�����̃x�N�g��������
	 //�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	 // �J������]�s��
	 XMMATRIX matCameraRot;
	 // �J�������W�n�����[���h���W�n�̕ϊ��s��
	 matCameraRot.r[0] = cameraAxisX;
	 matCameraRot.r[1] = cameraAxisY;
	 matCameraRot.r[2] = cameraAxisZ;
	 matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	 // �]�u�ɂ��t�s��i�t��]�j���v�Z
	 matView = XMMatrixTranspose(matCameraRot);

	 // ���_���W��-1���|�������W
	 XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	 // �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	 XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
	 XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
	 XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����
	 // ��̃x�N�g���ɂ܂Ƃ߂�
	 XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	 // �r���[�s��ɕ��s�ړ�������ݒ�
	 matView.r[3] = translation;

#pragma region �S�����r���{�[�h�s��̌v�Z
	 // �r���{�[�h�s��
	 matBillboard.r[0] = cameraAxisX;
	 matBillboard.r[1] = cameraAxisY;
	 matBillboard.r[2] = cameraAxisZ;
	 matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y�����r���{�[�h�s��̌v�Z
	 // �J����X���AY���AZ��
	 XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	 // X���͋���
	 ybillCameraAxisX = cameraAxisX;
	 // Y���̓��[���h���W�n��Y��
	 ybillCameraAxisY = XMVector3Normalize(upVector);
	 // Z����X����Y���̊O�ςŋ��܂�
	 ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	 // Y�����r���{�[�h�s��
	 matBillboardY.r[0] = ybillCameraAxisX;
	 matBillboardY.r[1] = ybillCameraAxisY;
	 matBillboardY.r[2] = ybillCameraAxisZ;
	 matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
 }


