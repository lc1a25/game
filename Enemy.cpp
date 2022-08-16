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
	//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	
	switch (phase)
	{
	case Phase::Approach:
		//関数にする
		enemy->position.x -= 0.5;
		if (enemy->position.x <= 1280 / 4 * 3)
		{
			phase = Phase::CircleR;
		}
		
		break;
	case Phase::ApproachL:
		//関数にする
		enemy->position.x += 0.5;
		if (enemy->position.x <= 1280 / 4 )
		{
			phase = Phase::CircleR;
		}

		break;
	case Phase::Leave:
		//関数にする
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
	case Phase::CircleR://右から来たver 90ど
		//円の角度
		radius = angle * 3.14f / 180.0f;

		//円状の場所
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//敵の座標に足す
		enemy->position.x += addCircleX;
		enemy->position.y += addCircleY;

		//角度をたして円状に動かす
		angle += 0.5f;
		if (angle >= 460.0f)
		{
			shotTimer = shotInterval;
			enemy->position.z -= 0.5;
		}

		//弾を撃つ
		shotTimer--;
		if (shotTimer <= 0)
		{
			Shot();
			shotTimer = shotInterval;
		}
		
		break;
	case Phase::CircleL://左から来たver 270ど
	//円の角度
		radius = angle * 3.14f / 180.0f;

		//円状の場所
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//敵の座標に足す
		enemy->position.x += addCircleX;
		enemy->position.y += addCircleY;

		//角度をたして円状に動かす
		angle -= 0.5f;
		if (angle <= -270.0f)
		{
			shotTimer = shotInterval;
			enemy->position.z -= 0.5;
		}

		//弾を撃つ
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

void Enemy::PhaseInit(bool rightMoveTrue)
{
	if (!rightMoveTrue)
	{
		phase = Phase::CircleL;
	}
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
	isDead = true;
	enemy->position.z -= 150;
	phase = Phase::Leave;
}
