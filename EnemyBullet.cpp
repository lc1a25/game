#include "EnemyBullet.h"

void EnemyBullet::Init(Model* model, XMFLOAT3 enemyPosition, XMVECTOR velocity)
{
	bulletModel_ = model;
	velocity_ = velocity;
	enemyBullet->SetPosition({ enemyPosition });
	enemyBullet->SetModel(bulletModel_);
	enemyBullet->scale = { 1,1,1 };
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
