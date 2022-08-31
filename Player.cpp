#include "Player.h"

void Player::Init(Model* model,Model* bulletModel)
{
	model_ = model;
	bulletModel_ = bulletModel;
	reticleModel_ = model;
	player->SetModel(model_);
	player->SetPosition({ 0,0,0 });
	player->scale = { 1,1,1 };

	reticle->SetModel(model_);
	reticle->SetPosition({ reticle->matWorld.r[3].m128_f32[0], reticle->matWorld.r[3].m128_f32[1], reticle->matWorld.r[3].m128_f32[2] });
	reticle->scale = { 1,1,1 };

	//player�̍��W�����[���h���W�� �J�����̑O��
	player->matWorld.r[3].m128_f32[0] = cameraPos.x;
	player->matWorld.r[3].m128_f32[1] = cameraPos.y;
	player->matWorld.r[3].m128_f32[2] = cameraPos.z + 10;
}


void Player::Update()
{
//2d���e�B�N��
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(hwnd, &mousePosition);
	mouseX = mousePosition.x;//�k���ϊ��@long -> float
	mouseY = mousePosition.y;

	//�r���[�v���W�F�N�V�����r���[�|�[�g�s��
	viewPort = {
	 1280.0f / 2,0.0f				   ,0.0f,0.0f,
	  0.0f				   ,-720/ 2,0.0f,0.0f,
	  0.0f				   ,0.0f				   ,1.0f,0.0f,
	 1280.0f / 2,720 / 2 ,0.0f,1.0f
	};
	XMMATRIX matVPV = cameraMatViewProjection * viewPort;
	//�t�s����v�Z
	XMMATRIX matInverse = XMMatrixInverse(nullptr, matVPV);
	//XMMATRIX matInverse = Matrix4::matrixInverse(matVPV);
	
	//�X�N���[�����W
	XMVECTOR posNear = XMVECTOR{ mouseX, mouseY, 0 };
	XMVECTOR posFar = XMVECTOR{ mouseX, mouseY, 1 };

	//�X�N���[�����烏�[���h���W��
	posNear = Matrix4::transformScreenToWorld(posNear, matInverse);
	posFar = Matrix4::transformScreenToWorld(posFar, matInverse);

	//�}�E�X���C�̕���
	XMVECTOR mouseDirection;
	mouseDirection = posFar - posNear;
	mouseDirection = XMVector3Normalize(mouseDirection);



	//�J��������3d���e�B�N���̋���
	const float distanceObject = 500.0f;
	//near ���� far �Ɂ@distanceObject ���i�񂾋���
	reticle->matWorld.r[3].m128_f32[0] = (mouseDirection - posNear).m128_f32[0] * distanceObject;
	reticle->matWorld.r[3].m128_f32[1] = (mouseDirection - posNear).m128_f32[1] * distanceObject;
	reticle->matWorld.r[3].m128_f32[2] = (mouseDirection - posNear).m128_f32[2] * distanceObject/500;

	//reticle->SetPosition({ reticle->matWorld.r[3].m128_f32[0], reticle->matWorld.r[3].m128_f32[1], reticle->matWorld.r[3].m128_f32[2] });
	

	reticle->position.x = reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y = reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z = reticle->matWorld.r[3].m128_f32[2];


	player->matWorld.r[3].m128_f32[2] = cameraPos.z +	30;

	player->position.x = player->matWorld.r[3].m128_f32[0];
	player->position.y = player->matWorld.r[3].m128_f32[1];
	player->position.z = player->matWorld.r[3].m128_f32[2];


//�f�X�t���O�������Ă���e������
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
//�ړ�����
	/*if (player->position.x <= -50)
	{
		player->position.x += playerVelocity;
		player->rotation.y += playerVelocity / 2;
	}
	if (player->position.x >= +40)
	{
		player->position.x -= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}

	if (player->position.y <= -30)
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity / 2;
	}
	if (player->position.y >= 20)
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity / 2;
	}*/

	if (input->isKey(DIK_W))
	{
		player->position.y+= playerVelocity;
		player->rotation.x -= playerVelocity / 4;
	}
	if (input->isKey(DIK_D))
	{
		player->position.x+= playerVelocity;
		player->rotation.y += playerVelocity / 2;
	
	}
	if (input->isKey(DIK_A))
	{
		player->position.x-= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}
	if (input->isKey(DIK_S))
	{
		player->position.y-= playerVelocity;
		player->rotation.x += playerVelocity / 4;
	}


//�U��
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{

		//bullet->SetLockOnPosition(enemyWorldPos,isDeadEnemy);	
		bullet->Update();
	}
//�X�V
	player->SetCameraMatWorld(cameraObj);
	player->Update(true);
	reticle->Update();
}

void Player::Draw()
{
	player->Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
	reticle->Draw();

}



void Player::SetReticleWorldPos(XMVECTOR reticlePos)
{
	reticle->matWorld.r[3].m128_f32[0] = reticlePos.m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = reticlePos.m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = reticlePos.m128_f32[2];


	reticle->position.x = reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y = reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z = reticle->matWorld.r[3].m128_f32[2];
}

XMFLOAT3 Player::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = player->matWorld.r[3].m128_f32[0];
	worldPos.y = player->matWorld.r[3].m128_f32[1];
	worldPos.z = player->matWorld.r[3].m128_f32[2];
	return worldPos;
}

XMFLOAT3 Player::GetReticleWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = reticle->matWorld.r[3].m128_f32[0];
	worldPos.y = reticle->matWorld.r[3].m128_f32[1];
	worldPos.z = reticle->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void Player::OnCollision()
{
}

XMFLOAT3 Player::GetSpriteReticle()
{
	return XMFLOAT3({positionReticle.m128_f32[0] ,positionReticle.m128_f32[1],0});
}

void Player::Attack()
{
	if (input->isKey(DIK_SPACE))
	{
		coolTimer--;
		if (coolTimer <= 0)
		{
			
			XMVECTOR velocity = { 0,0,1 };
			velocity = transform(velocity, player->matWorld);
			//���e�B�N���Ɍ������Ĕ��
			velocity.m128_f32[0] = reticle->matWorld.r[3].m128_f32[0] - player->matWorld.r[3].m128_f32[0];
			velocity.m128_f32[1] = reticle->matWorld.r[3].m128_f32[1] - player->matWorld.r[3].m128_f32[1];
			velocity.m128_f32[2] = reticle->matWorld.r[3].m128_f32[2] - player->matWorld.r[3].m128_f32[2];
			velocity = XMVector3Normalize(velocity) * 3.0f;

			//�e����
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Init(bulletModel_, player->position, velocity);


			//�e�o�^
			bullets_.push_back(std::move(newBullet));//move �̓��j�[�N������n���邽��

			coolTimer = bulletCoolTimer;
		}
	}
}

XMVECTOR Player::transform(const XMVECTOR& v, const DirectX::XMMATRIX& m)
{
	float w = v.m128_f32[0] * m.r[0].m128_f32[3] + v.m128_f32[1] * m.r[1].m128_f32[3] + v.m128_f32[2] * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMVECTOR result
	{
		(v.m128_f32[0] * m.r[0].m128_f32[0] + v.m128_f32[1] * m.r[1].m128_f32[0] + v.m128_f32[2] * m.r[2].m128_f32[0]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[1] + v.m128_f32[1] * m.r[1].m128_f32[1] + v.m128_f32[2] * m.r[2].m128_f32[1]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[2] + v.m128_f32[1] * m.r[1].m128_f32[2] + v.m128_f32[2] * m.r[2].m128_f32[2]) / w

	};
	return result;
}

XMVECTOR Player::transform2(const XMVECTOR& v, const DirectX::XMMATRIX& m)
{
	float w = v.m128_f32[0] * m.r[0].m128_f32[3] + v.m128_f32[1] * m.r[1].m128_f32[3] + v.m128_f32[2] * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMVECTOR result
	{
		(v.m128_f32[0] * m.r[0].m128_f32[0] + v.m128_f32[1] * m.r[1].m128_f32[0] + v.m128_f32[2] * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[1] + v.m128_f32[1] * m.r[1].m128_f32[1] + v.m128_f32[2] * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[2] + v.m128_f32[1] * m.r[1].m128_f32[2] + v.m128_f32[2] * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w

	};
	return result;
}




