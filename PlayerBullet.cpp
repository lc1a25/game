#include "PlayerBullet.h"

void PlayerBullet::Init(Model* model,XMFLOAT3 playerPosition ,XMVECTOR velocity)
{
	model_ = model;
	velocity_ = velocity;
	bullet->SetModel(model_);
	bullet->matWorld.r[3].m128_f32[0] = playerPosition.x;
	bullet->matWorld.r[3].m128_f32[1] = playerPosition.y;
	bullet->matWorld.r[3].m128_f32[2] = playerPosition.z;

	bullet->SetPosition({ bullet->matWorld.r[3].m128_f32[0],bullet->matWorld.r[3].m128_f32[1],bullet->matWorld.r[3].m128_f32[2] });
	bullet->position.x += velocity_.m128_f32[0];
	bullet->position.y += velocity_.m128_f32[1];
	bullet->position.z += velocity_.m128_f32[2];

	bullet->matWorld.r[3].m128_f32[0] = bullet->position.x ;
	bullet->matWorld.r[3].m128_f32[1] = bullet->position.y ;
	bullet->matWorld.r[3].m128_f32[2] = bullet->position.z ;

	bullet->scale = { 2,2,2 };
}

void PlayerBullet::Update()
{
	//時間経過でたまをけす
	if (--deathTimer <= 0)
	{
		isDead = true;
	}
	bullet->position.x += velocity_.m128_f32[0];
	bullet->position.y += velocity_.m128_f32[1];
	bullet->position.z += velocity_.m128_f32[2];

	bullet->matWorld.r[3].m128_f32[0] = bullet->position.x;
	bullet->matWorld.r[3].m128_f32[1] = bullet->position.y;
	bullet->matWorld.r[3].m128_f32[2] = bullet->position.z;

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

void PlayerBullet::SetLockOnPosition(XMFLOAT3 enemyPosition,bool isDead)
{
	if (isDead == true)
	{
	}
	else
	{
		//assert(player_);
		const float speed = 2.5f;//1フレーム進む距離

		//差分ベクトル
		velocity_.m128_f32[0] = enemyPosition.x - bullet->position.x;
		velocity_.m128_f32[1] = enemyPosition.y - bullet->position.y;
		velocity_.m128_f32[2] = enemyPosition.z - bullet->position.z;


		//正規化
		velocity_ = XMVector3Normalize(velocity_);

		//正規化ベクトルと1フレーム進む距離をかける
		velocity_ *= speed;
	}
}