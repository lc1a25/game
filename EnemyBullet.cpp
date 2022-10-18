#include "EnemyBullet.h"

void EnemyBullet::Init(Model* model, XMFLOAT3 enemyPosition, XMVECTOR velocity)
{
	bulletModel_ = model;
	velocity_ = velocity;
	enemyBullet->SetPosition({ enemyPosition });
	enemyBullet->SetModel(bulletModel_);
	enemyBullet->scale = { 0.5,0.5,1.0 };
	enemyBullet->position.x += velocity_.m128_f32[0];
	enemyBullet->position.y += velocity_.m128_f32[1];
	enemyBullet->position.z += velocity_.m128_f32[2];
}

void EnemyBullet::Update()
{
	//時間経過でたまをけす
	if (--deathTimer <= 0)
	{
		isDead = true;
	}
	
	enemyBullet->rotation.y = atan2(diff.m128_f32[2], diff.m128_f32[0]);

	//enemyBullet->rotation.y *= -enemyBullet->rotation.y;
	enemyBullet->rotation.x = atan2( diff.m128_f32[1], diff.m128_f32[0] * diff.m128_f32[0] + diff.m128_f32[2] * diff.m128_f32[2]);

	enemyBullet->position.x += velocity_.m128_f32[0];
	enemyBullet->position.y += velocity_.m128_f32[1];
	enemyBullet->position.z += velocity_.m128_f32[2];
	
	enemyBullet->Update();
}

void EnemyBullet::Draw()
{
	enemyBullet->Draw();
}

void EnemyBullet::OnCollision()
{
	isDead = true;
}

XMFLOAT3 EnemyBullet::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = enemyBullet->matWorld.r[3].m128_f32[0];
	worldPos.y = enemyBullet->matWorld.r[3].m128_f32[1];
	worldPos.z = enemyBullet->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void EnemyBullet::SetLockOnPosition(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition)
{
	//assert(player_);
	const float speed = 1.0f;//1フレーム進む距離

	//差分ベクトル
	velocity_.m128_f32[0] = playerPosition.x - enemyPosition.x;
	velocity_.m128_f32[1] = playerPosition.y - enemyPosition.y;
	velocity_.m128_f32[2] = playerPosition.z - enemyPosition.z;
	
	//正規化
	velocity_ = XMVector3Normalize(velocity_);

	//正規化ベクトルと1フレーム進む距離をかける
	velocity_ *= speed;
}

void EnemyBullet::SetDiffVec(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition)
{
	//差分ベクトル
	diff.m128_f32[0] = playerPosition.x - enemyPosition.x;
	diff.m128_f32[1] = playerPosition.y - enemyPosition.y;
	diff.m128_f32[2] = playerPosition.z - enemyPosition.z;
}
