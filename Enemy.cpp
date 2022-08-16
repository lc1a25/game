#include "Enemy.h"

void Enemy::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue)
{
	enemyModel_ = enemyModel;
	bulletModel_ = enemyModel;
	enemy->SetModel(enemyModel_);
	enemy->SetPosition({ position });
	enemy->scale = { 2,2,2 };

	PhaseInit(rightMoveTrue);
}

void Enemy::Update()
{
	//�f�X�t���O�������Ă���e������
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	
	switch (phase)
	{
	case Phase::Approach:
		//�֐��ɂ���
		enemy->position.x -= 0.5;
		if (enemy->position.x <= 1280 / 4 * 3)
		{
			phase = Phase::CircleR;
		}
		
		break;
	case Phase::ApproachL:
		//�֐��ɂ���
		enemy->position.x += 0.5;
		if (enemy->position.x <= 1280 / 4 )
		{
			phase = Phase::CircleR;
		}

		break;
	case Phase::Leave:
		//�֐��ɂ���
		enemy->position.z -= 0.5;
		break;
	case Phase::Stop:
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		break;
	case Phase::CircleR://�E���痈��ver 90��
		//�~�̊p�x
		radius = angle * 3.14f / 180.0f;

		//�~��̏ꏊ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//�G�̍��W�ɑ���
		enemy->position.x += addCircleX;
		enemy->position.y += addCircleY;

		//�p�x�������ĉ~��ɓ�����
		angle += 0.5f;
		if (angle >= 460.0f)
		{
			shotTimer = shotInterval;
			enemy->position.z -= 0.5;
		}

		//�e������
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		
		break;
	case Phase::CircleL://�����痈��ver 270��
	//�~�̊p�x
		radius = angle * 3.14f / 180.0f;

		//�~��̏ꏊ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//�G�̍��W�ɑ���
		enemy->position.x += addCircleX;
		enemy->position.y += addCircleY;

		//�p�x�������ĉ~��ɓ�����
		angle -= 0.5f;
		if (angle <= -270.0f)
		{
			shotTimer = shotInterval;
			enemy->position.z -= 0.5;
		}

		//�e������
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		break;

	default:
		break;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		//bullet->SetLockOnPosition(GetWorldPosition(), playerWorldPos);
		bullet->Update();
	}
	enemy->Update();
}

void Enemy::Draw()
{
	enemy->Draw();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
}

void Enemy::Shot()
{
	//assert(player_);
	const float speed = 2.0f;//1�t���[���i�ދ���

	//�����x�N�g��
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z;

	//���K��
	lockOn = XMVector3Normalize(lockOn);

	//���K���x�N�g����1�t���[���i�ދ�����������
	lockOn *= speed;
	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);


	//�e�o�^
	bullets_.push_back(std::move(newBullet));//move �̓��j�[�N������n���邽��
}

void Enemy::PhaseInit(bool rightMoveTrue)
{
	if (!rightMoveTrue)
	{
		phase = Phase::CircleL;
	}
	//���˃^�C�}�[������
	shotTimer = shotInterval;
}

XMFLOAT3 Enemy::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = enemy->matWorld.r[3].m128_f32[0];
	worldPos.y = enemy->matWorld.r[3].m128_f32[1];
	worldPos.z = enemy->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void Enemy::OnCollision()
{
	isDead = true;
	enemy->position.z -= 150;
	phase = Phase::Leave;
}
