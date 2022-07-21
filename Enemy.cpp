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
	//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	switch (phase)
	{
	case Phase::Approach:
		//関数にする
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
		//関数にする
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
	const float speed = 2.0f;//1フレーム進む距離

	//差分ベクトル
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z;

	//正規化
	lockOn = XMVector3Normalize(lockOn);

	//正規化ベクトルと1フレーム進む距離をかける
	lockOn *= speed;
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);


	//弾登録
	bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため
}

void Enemy::PhaseInit()
{
	//発射タイマー初期化
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
