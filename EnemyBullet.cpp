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
	//ŽžŠÔŒo‰ß‚Å‚½‚Ü‚ð‚¯‚·
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
