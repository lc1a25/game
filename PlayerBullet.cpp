#include "PlayerBullet.h"

void PlayerBullet::Init(Model* model,XMFLOAT3 playerPosition , XMFLOAT3 velocity)
{
	model_ = model;
	velocity_ = velocity;
	bullet->SetModel(model_);
	bullet->SetPosition({ playerPosition });
	bullet->position.x += velocity_.x;
	bullet->position.y += velocity_.y;
	bullet->position.z += velocity_.z;
	bullet->scale = { 2,2,2 };
}

void PlayerBullet::Update()
{
	//時間経過でたまをけす
	if (--deathTimer <= 0)
	{
		isDead = true;
	}
	bullet->position.x += velocity_.x;
	bullet->position.y += velocity_.y;
	bullet->position.z += velocity_.z;

	bullet->Update();
}

void PlayerBullet::Draw()
{
	bullet->Draw();
}

void PlayerBullet::OnCollision()
{
	isDead = true;
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = bullet->matWorld.r[3].m128_f32[0];
	worldPos.y = bullet->matWorld.r[3].m128_f32[1];
	worldPos.z = bullet->matWorld.r[3].m128_f32[2];
	return worldPos;
}
