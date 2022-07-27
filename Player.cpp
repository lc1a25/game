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
	//ÉfÉXÉtÉâÉOÇ™óßÇ¡ÇƒÇ¢ÇÈíeÇè¡Ç∑
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
	if (player->position.x <= -40)
	{
		player->position.x += playerVelocity;
		player->rotation.y += playerVelocity / 2;
	}
	if (player->position.x >= +40)
	{
		player->position.x -= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}

	if (player->position.y <= -30)
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity / 2;
	}
	if (player->position.y >= 20)
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity / 2;
	}

	if (input->isKey(DIK_W))
	{
		player->position.y+= playerVelocity;
		player->rotation.x -= playerVelocity / 2;
	}
	if (input->isKey(DIK_D))
	{
		player->position.x+= playerVelocity;
		player->rotation.y += playerVelocity / 2;
	}
	if (input->isKey(DIK_A))
	{
		player->position.x-= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}
	if (input->isKey(DIK_S))
	{
		player->position.y-= playerVelocity;
		player->rotation.x += playerVelocity / 2;
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
		
		//bullet->SetLockOnPosition(enemyWorldPos);
		
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
	if (input->isKey(DIK_SPACE))
	{
		coolTimer--;
		if (coolTimer <= 0)
		{
			XMVECTOR velocity = { 0,0,1 };
			velocity = transform(velocity, player->matWorld);
			//íeê∂ê¨
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Init(bulletModel_, player->position, velocity);


			//íeìoò^
			bullets_.push_back(std::move(newBullet));//move ÇÕÉÜÉjÅ[ÉNÇ©ÇÁè˜ìnÇ∑ÇÈÇΩÇﬂ

			coolTimer = bulletCoolTimer;
		}
	}
}

XMVECTOR Player::transform(const XMVECTOR& v, const DirectX::XMMATRIX& m)
{
	float w = v.m128_f32[0] * m.r[0].m128_f32[3] + v.m128_f32[1] * m.r[1].m128_f32[3] + v.m128_f32[2] * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMVECTOR result
	{
		(v.m128_f32[0] * m.r[0].m128_f32[0] + v.m128_f32[1] * m.r[1].m128_f32[0] + v.m128_f32[2] * m.r[2].m128_f32[0]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[1] + v.m128_f32[1] * m.r[1].m128_f32[1] + v.m128_f32[2] * m.r[2].m128_f32[1]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[2] + v.m128_f32[1] * m.r[1].m128_f32[2] + v.m128_f32[2] * m.r[2].m128_f32[2]) / w

	};
	return result;
}




