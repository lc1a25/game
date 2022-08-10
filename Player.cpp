#include "Player.h"

void Player::Init(Model* model,Model* bulletModel)
{
	model_ = model;
	bulletModel_ = bulletModel;
	reticleModel_ = model;
	player->SetModel(model_);
	player->SetPosition({ 0,-20,0 });
	player->scale = { 3,3,3 };

	reticle->SetModel(model_);
	reticle->SetPosition({ reticle->matWorld.r[3].m128_f32[0], reticle->matWorld.r[3].m128_f32[1], reticle->matWorld.r[3].m128_f32[2] });
	reticle->scale = { 1,1,1 };
}


void Player::Update()
{
	//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
	/*if (player->position.x <= -40)
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
	}*/

	if (input->isKey(DIK_W))
	{
		player->position.y+= playerVelocity;
		player->rotation.x -= playerVelocity / 4;
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
		player->rotation.x += playerVelocity / 4;
	}

	if (input->isKey(DIK_Q))
	{
		player->rotation.y+= playerVelocity;
	}
	if (input->isKey(DIK_E))
	{
		player->rotation.y -= playerVelocity;
	}

	////自機からレティクルの距離
	//const float distancePlayerReticle = 50.0f;
	//XMVECTOR offset = { 0.0f,0.0f,1.0f };
	////自機のワールド行列の回転を反映
	//offset = transform(offset, player->matWorld);
	////ベクトルの正規化
	//offset = XMVector3Normalize(offset)* distancePlayerReticle;
	//////レティクルの座標設定
	//reticle->matWorld.r[3].m128_f32[0]	= offset.m128_f32[0];
	//reticle->matWorld.r[3].m128_f32[1]	= offset.m128_f32[1];
	//reticle->matWorld.r[3].m128_f32[2]	= offset.m128_f32[2];



	reticle->position.x =  reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y =  reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z =  reticle->matWorld.r[3].m128_f32[2];


	//XMMATRIX matViewPort = viewPortMatrix;

	//XMMATRIX matViewProjectionViewPort = cameraMatViewProjection * matViewPort;

	//reticleWorldPos = transform(reticleWorldPos, matViewProjectionViewPort);
	
/*reticle->matWorld.r[3].m128_f32[0] =   reticleWorldPos.m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = reticleWorldPos.m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = reticleWorldPos.m128_f32[2];*/

	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		
		//bullet->SetLockOnPosition(enemyWorldPos);
		
		bullet->Update();
	}
	
	player->Update();
	reticle->Update();
}

void Player::Draw()
{
	player->Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
	reticle->Draw();

}



void Player::SetReticleWorldPos(XMVECTOR reticlePos)
{
	reticle->matWorld.r[3].m128_f32[0] = reticlePos.m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = -1 *reticlePos.m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = reticlePos.m128_f32[2];


	/*reticle->position.x = reticlePos.m128_f32[0];
	reticle->position.y = reticlePos.m128_f32[1];
	reticle->position.z = reticlePos.m128_f32[2];*/
}

XMFLOAT3 Player::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = player->matWorld.r[3].m128_f32[0];
	worldPos.y = player->matWorld.r[3].m128_f32[1];
	worldPos.z = player->matWorld.r[3].m128_f32[2];
	return worldPos;
}

XMFLOAT3 Player::GetReticleWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = reticle->matWorld.r[3].m128_f32[0];
	worldPos.y = reticle->matWorld.r[3].m128_f32[1];
	worldPos.z = reticle->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void Player::OnCollision()
{
}

XMFLOAT3 Player::GetSpriteReticle()
{
	return XMFLOAT3({reticleWorldPos.m128_f32[0],reticleWorldPos.m128_f32[1] ,0});
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
			velocity.m128_f32[0] = reticle->matWorld.r[3].m128_f32[0] - player->matWorld.r[3].m128_f32[0];
			velocity.m128_f32[1] = reticle->matWorld.r[3].m128_f32[1] - player->matWorld.r[3].m128_f32[1];
			velocity.m128_f32[2] = reticle->matWorld.r[3].m128_f32[2] - player->matWorld.r[3].m128_f32[2];
			velocity = XMVector3Normalize(velocity) * 2.0f;

			//弾生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Init(bulletModel_, player->position, velocity);


			//弾登録
			bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため

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

XMVECTOR Player::transform2(const XMVECTOR& v, const DirectX::XMMATRIX& m)
{
	float w = v.m128_f32[0] * m.r[0].m128_f32[3] + v.m128_f32[1] * m.r[1].m128_f32[3] + v.m128_f32[2] * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMVECTOR result
	{
		(v.m128_f32[0] * m.r[0].m128_f32[0] + v.m128_f32[1] * m.r[1].m128_f32[0] + v.m128_f32[2] * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[1] + v.m128_f32[1] * m.r[1].m128_f32[1] + v.m128_f32[2] * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[2] + v.m128_f32[1] * m.r[1].m128_f32[2] + v.m128_f32[2] * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w

	};
	return result;
}




