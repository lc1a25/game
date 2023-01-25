#include "Player.h"

void Player::Init(Model* model,Model* bulletModel)
{
	model_ = model;
	bulletModel_ = bulletModel;
	reticleModel_ = model;
	player->SetModel(model_);
	
	player->scale = { 1,1,1 };

	reticle->matWorld.r[3].m128_f32[0] = player->matWorld.r[3].m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = player->matWorld.r[3].m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = player->matWorld.r[3].m128_f32[2] + 30;

	reticle->SetModel(model_);
	reticle->SetPosition({ reticle->matWorld.r[3].m128_f32[0], reticle->matWorld.r[3].m128_f32[1], reticle->matWorld.r[3].m128_f32[2]});
	reticle->scale = { 1,1,1 }; 

	//playerの座標をワールド座標に カメラの前に
	player->matWorld.r[3].m128_f32[0] = cameraPos.x;
	player->matWorld.r[3].m128_f32[1] = cameraPos.y;
	player->matWorld.r[3].m128_f32[2] = cameraPos.z + 30;

	player->SetPosition({ player->matWorld.r[3].m128_f32[0],player->matWorld.r[3].m128_f32[1],player->matWorld.r[3].m128_f32[2] });

}


void Player::Update()
{
	//デバッグ用　ｈｐ下げ
	if (input->isKeyTrigger(DIK_E))
	{
		playerHp--;
		hpBar -= 96;
		if (playerHp <= 0)
		{
			hp0 = true;
		}
	}
//2dレティクル
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(hwnd, &mousePosition);
	mouseX = mousePosition.x;//縮小変換　long -> float
	mouseY = mousePosition.y;

	//ビュープロジェクションビューポート行列
	viewPort = {
	 1280.0f / 2 ,0.0f    ,0.0f, 0.0f,
	  0.0f		 ,-720/ 2 ,0.0f, 0.0f,
	  0.0f		 ,0.0f	  ,1.0f, 0.0f,
	 1280.0f / 2 ,720 / 2 ,0.0f, 1.0f
	};

	XMMATRIX matVPV = cameraMatViewProjection * viewPort;
	//XMMATRIX matVPV = viewPort;
	//逆行列を計算
	XMMATRIX matInverse = XMMatrixInverse(nullptr, matVPV);
	//XMMATRIX matInverse = Matrix4::matrixInverse(matVPV);

	
	//スクリーン座標
	posNear = XMVECTOR{ mouseX, mouseY, 0 };
	posFar = XMVECTOR{ mouseX, mouseY, 1 };

	//スクリーンからワールド座標に
	posNear = XMVector3TransformCoord(posNear, matInverse);
	posFar = XMVector3TransformCoord(posFar, matInverse);
	//posNear = Matrix4::transformScreenToWorld(posNear, matInverse);
	//posFar = Matrix4::transformScreenToWorld(posFar, matInverse);

	//マウスレイの方向
	XMVECTOR mouseDirection;
	mouseDirection = posFar - posNear;
	mouseDirection = XMVector3Normalize(mouseDirection);

	//カメラから3dレティクルの距離
	//const float distanceObject = 280.0f;
	//near から mouseDirection(near->far) に　distanceObject 分進んだ距離
	reticle->matWorld.r[3].m128_f32[0] = posNear.m128_f32[0] + mouseDirection.m128_f32[0] * 100;
	reticle->matWorld.r[3].m128_f32[1] = posNear.m128_f32[1] + mouseDirection.m128_f32[1] * 100;
	reticle->matWorld.r[3].m128_f32[2] = posNear.m128_f32[2] + mouseDirection.m128_f32[2] * 100;
	//reticle->matWorld.r[3].m128_f32[0] = (mouseDirection - posNear).m128_f32[0] * 100;
	//reticle->matWorld.r[3].m128_f32[1] = (mouseDirection - posNear).m128_f32[1] * 100;
	//reticle->matWorld.r[3].m128_f32[2] = cameraObj.r[3].m128_f32[2] + 100;//player->matWorld.r[3].m128_f32[2] + 100;//(mouseDirection - posNear).m128_f32[2] * 10;

	//ワールド座標に
	reticle->position.x = reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y = reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z = reticle->matWorld.r[3].m128_f32[2];

	//player->matWorld.r[3].m128_f32[2] = cameraPos.z +	30;
	player->matWorld.r[3].m128_f32[0] += cameraEyeVec.x;
	player->matWorld.r[3].m128_f32[1] += cameraEyeVec.y;
	player->matWorld.r[3].m128_f32[2] += cameraEyeVec.z;


	player->position.x = player->matWorld.r[3].m128_f32[0];
	player->position.y = player->matWorld.r[3].m128_f32[1];
	player->position.z = player->matWorld.r[3].m128_f32[2];

	cameraTargetVec = XMVector3Normalize(cameraTargetVec);
	//player->position.x = cameraPos.x + cameraTargetVec.m128_f32[0] * 10;
	//player->position.y = cameraPos.y + cameraTargetVec.m128_f32[1] * 10;
	//player->position.z = cameraPos.z + cameraTargetVec.m128_f32[2] * 10;
												
//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

//移動制御(画面外に行かないように)
	if (player->position.x <= cameraPos.x - playerMoveRange.x)
	{
		player->position.x += playerVelocity;
		player->rotation.y += playerVelocity / 2;
	}
	if (player->position.x >= cameraPos.x + playerMoveRange.x)
	{
		player->position.x -= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}

	if (player->position.y <= cameraPos.y - playerMoveRange.y)
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity / 4;
	}
	if (player->position.y >= cameraPos.y + playerMoveRange.y)
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity / 4;
	}

//移動処理
	if (input->isKey(DIK_W))
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity / 4;
	}
	if (input->isKey(DIK_D))
	{
		player->position.x += playerVelocity;
		player->rotation.y += playerVelocity / 2;
	
	}
	if (input->isKey(DIK_A))
	{
		player->position.x -= playerVelocity;
		player->rotation.y -= playerVelocity / 2;
	}
	if (input->isKey(DIK_S))
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity / 4;
	}

//攻撃
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		//bullet->SetLockOnPosition(enemyWorldPos,isDeadEnemy);	
		bullet->Update();
	}

//更新
	
	player->Update();
	reticle->Update();
}

//描画
void Player::Draw()
{
	player->Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
	//reticle->Draw();

}



void Player::SetReticleWorldPos(XMVECTOR reticlePos)
{
	reticle->matWorld.r[3].m128_f32[0] = reticlePos.m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = reticlePos.m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = reticlePos.m128_f32[2];


	reticle->position.x = reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y = reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z = reticle->matWorld.r[3].m128_f32[2];
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

XMFLOAT3 Player::GetReticleNear()
{
	XMFLOAT3 worldPos;
	worldPos.x = posNear.m128_f32[0];
	worldPos.y = posNear.m128_f32[1];
	worldPos.z = posNear.m128_f32[2];
	return worldPos;
}

XMFLOAT3 Player::GetReticleFar()
{
	XMFLOAT3 worldPos;
	worldPos.x = posFar.m128_f32[0];
	worldPos.y = posFar.m128_f32[1];
	worldPos.z = posFar.m128_f32[2];
	return worldPos;
}

void Player::OnCollision()
{
	playerHp--;
	hpBar -= 96;
	if (playerHp <= 0)
	{
		hp0 = true;
	}
}

XMFLOAT3 Player::GetSpriteReticle()
{
	return XMFLOAT3({positionReticle.m128_f32[0] ,positionReticle.m128_f32[1],0});
}

void Player::Attack()
{
	if (input->isMouseKey())
	{
		coolTimer--;
		if (coolTimer <= 0)
		{
			
			XMVECTOR velocity = { 0,0,1 };
			velocity = transform(velocity, player->matWorld);
			//レティクルに向かって飛ぶ
		  /*velocity.m128_f32[0] = reticle->matWorld.r[3].m128_f32[0] - player->matWorld.r[3].m128_f32[0];
			velocity.m128_f32[1] = reticle->matWorld.r[3].m128_f32[1] - player->matWorld.r[3].m128_f32[1];
			velocity.m128_f32[2] = reticle->matWorld.r[3].m128_f32[2] - player->matWorld.r[3].m128_f32[2];*/
			velocity.m128_f32[0] = reticle->position.x - player->position.x;
			velocity.m128_f32[1] = reticle->position.y - player->position.y;
			velocity.m128_f32[2] = reticle->position.z - player->position.z;

			velocity = XMVector3Normalize(velocity) * 7.0f;

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




