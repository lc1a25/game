#include "Player.h"

void Player::Init(Model* model,Model* bulletModel)
{
	model_ = model;
	bulletModel_ = bulletModel;
	player->SetModel(model_);
	player->SetPosition({ 0,-20,0 });
	player->scale = { 3,3,3 };
}


void Player::Update()
{
	//ƒfƒXƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é’e‚ğÁ‚·
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
	if (input->isKey(DIK_W))
	{
		player->position.y+= playerVelocity;
	}
	if (input->isKey(DIK_D))
	{
		player->position.x+= playerVelocity;
	}
	if (input->isKey(DIK_A))
	{
		player->position.x-= playerVelocity;
	}
	if (input->isKey(DIK_S))
	{
		player->position.y-= playerVelocity;
	}

	if (input->isKey(DIK_Q))
	{
		player->rotation.y+= playerVelocity;
	}
	if (input->isKey(DIK_E))
	{
		player->rotation.y -= playerVelocity;
	}
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	player->Update();
}

void Player::Draw()
{
	player->Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

}

XMFLOAT3 Player::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = player->matWorld.r[3].m128_f32[0];
	worldPos.y = player->matWorld.r[3].m128_f32[1];
	worldPos.z = player->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void Player::OnCollision()
{
}

void Player::Attack()
{
	if (input->isKeyTrigger(DIK_SPACE))
	{
		XMFLOAT3 velocity = { 0,0,1 };
		velocity = transform(velocity, player->matWorld);
		//’e¶¬
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Init(bulletModel_, player->position,velocity);
		
		
		//’e“o˜^
		bullets_.push_back(std::move(newBullet));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
	}
}

XMFLOAT3 Player::transform(const XMFLOAT3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMFLOAT3 result
	{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2]) / w

	};
	return result;
}




