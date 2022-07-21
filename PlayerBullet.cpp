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
	//ŽžŠÔŒo‰ß‚Å‚½‚Ü‚ð‚¯‚·
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
