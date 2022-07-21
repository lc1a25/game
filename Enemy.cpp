#include "Enemy.h"

void Enemy::Init(Model* enemyModel)
{
	enemyModel_ = enemyModel;
	bulletModel_ = enemyModel;
	enemy->SetModel(enemyModel_);
	enemy->SetPosition({ 30,0,+100 });
	enemy->scale = { 2,2,2 };

	PhaseInit();
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
		enemy->position.z-= 0.5;
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		if (enemy->position.z < -25)
		{
			shotTimer = shotInterval;
			phase = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//�֐��ɂ���
		enemy->position.z += 0.5;
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		if (enemy->position.z > +100)
		{
			shotTimer = shotInterval;
			phase = Phase::Approach;
		}
		break;
	default:
		break;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
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

void Enemy::PhaseInit()
{
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
}
