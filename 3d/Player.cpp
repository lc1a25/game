#include "Player.h"

void Player::Init(Model* model,Model* bulletModel)
{
	//モデルセット
	model_ = model;
	bulletModel_ = bulletModel;
	reticleModel_ = model;
	player->SetModel(model_);
	
	player->scale = {2,2,2 };

	reticle->matWorld.r[3].m128_f32[0] = player->matWorld.r[3].m128_f32[0];
	reticle->matWorld.r[3].m128_f32[1] = player->matWorld.r[3].m128_f32[1];
	reticle->matWorld.r[3].m128_f32[2] = player->matWorld.r[3].m128_f32[2] + reticleZ;

	reticle->SetModel(model_);
	reticle->SetPosition({ reticle->matWorld.r[3].m128_f32[0], reticle->matWorld.r[3].m128_f32[1], reticle->matWorld.r[3].m128_f32[2]});
	reticle->scale = { 1,1,1 }; 

	player->matWorld.r[3].m128_f32[0] = 0;
	player->matWorld.r[3].m128_f32[1] = -50;
	player->matWorld.r[3].m128_f32[2] = 0;

	player->SetPosition({ player->matWorld.r[3].m128_f32[0],player->matWorld.r[3].m128_f32[1],player->matWorld.r[3].m128_f32[2] });

	playerStartPos = { player->matWorld.r[3].m128_f32[0],player->matWorld.r[3].m128_f32[1],player->matWorld.r[3].m128_f32[2] };
	playerStartPos2 = { player->matWorld.r[3].m128_f32[0],player->matWorld.r[3].m128_f32[1],player->matWorld.r[3].m128_f32[2] + playerZ};

}


void Player::Update()
{
	//ムービーがおわったあとに画面外(手前)からプレイヤーが登場する
	if (gameEndFlag == true)
	{
	}
	else if (player->position.z >= 30 && gameEndFlag == false)
		{
			gameStartFlag = false;
			keyInput = true;
		}
	else if (gameStartFlag == true && gameEndFlag == false)
	{
		keyInput = false;
		player->matWorld.r[3].m128_f32[2]++;
		player->position.z = player->matWorld.r[3].m128_f32[2];
	
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
	
	//スクリーン座標
	posNear = XMVECTOR{ mouseX, mouseY, 0 };
	posFar = XMVECTOR{ mouseX, mouseY, 1 };

	//スクリーンからワールド座標に
	posNear = XMVector3TransformCoord(posNear, matInverse);
	posFar = XMVector3TransformCoord(posFar, matInverse);

	//マウスレイの方向
	XMVECTOR mouseDirection;
	mouseDirection = posFar - posNear;
	mouseDirection = XMVector3Normalize(mouseDirection);

	//カメラから3dレティクルの距離
	//near から mouseDirection(near->far) に　distanceObject 分進んだ距離
	reticle->matWorld.r[3].m128_f32[0] = posNear.m128_f32[0] + mouseDirection.m128_f32[0] * 100;
	reticle->matWorld.r[3].m128_f32[1] = posNear.m128_f32[1] + mouseDirection.m128_f32[1] * 100;
	reticle->matWorld.r[3].m128_f32[2] = posNear.m128_f32[2] + mouseDirection.m128_f32[2] * 100;
	
	//ワールド座標に
	reticle->position.x = reticle->matWorld.r[3].m128_f32[0];
	reticle->position.y = reticle->matWorld.r[3].m128_f32[1];
	reticle->position.z = reticle->matWorld.r[3].m128_f32[2];

	player->matWorld.r[3].m128_f32[0] += cameraEyeVec.x;
	player->matWorld.r[3].m128_f32[1] += cameraEyeVec.y;
	player->matWorld.r[3].m128_f32[2] += cameraEyeVec.z;

	player->position.x = player->matWorld.r[3].m128_f32[0];
	player->position.y = player->matWorld.r[3].m128_f32[1];
	player->position.z = player->matWorld.r[3].m128_f32[2];

	cameraTargetVec = XMVector3Normalize(cameraTargetVec);
												
//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

//移動制御(画面外に行かないように)
	if (player->position.x <= cameraPos.x - playerMoveRange.x && keyInput == true)
	{
		player->position.x += playerVelocity;
		player->rotation.z -= playerVelocity / 2;
		player->rotation.y += playerVelocity;
	}
	if (player->position.x >= cameraPos.x + playerMoveRange.x && keyInput == true)
	{
		player->position.x -= playerVelocity;
		player->rotation.z += playerVelocity / 2;
		player->rotation.y -= playerVelocity;
	}

	if (player->position.y <= cameraPos.y - playerMoveRange.y && keyInput == true && playerDieFlag == false)
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity;	
	}
	if (player->position.y >= cameraPos.y + playerMoveRange.y - 5 && keyInput == true)
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity;
	}
	//player->rotation.x -= playerVelocity;
//移動処理
	if (input->isKey(DIK_W) && keyInput == true)
	{
		player->position.y += playerVelocity;
		player->rotation.x -= playerVelocity;
	}
	else if (input->isKey(DIK_D) && keyInput == true)
	{
		player->position.x += playerVelocity;
		player->rotation.z -= playerVelocity / 2;
		player->rotation.y += playerVelocity;
	
	}
	else if (input->isKey(DIK_A) && keyInput == true)
	{
		player->position.x -= playerVelocity;
		player->rotation.z += playerVelocity / 2;
		player->rotation.y -= playerVelocity;
	}
	else if (input->isKey(DIK_S) && keyInput == true)
	{
		player->position.y -= playerVelocity;
		player->rotation.x += playerVelocity;
	}
	else
	{
	
		if (player->rotation.x <= -1)
		{
			player->rotation.x += playerVelocity;
		}
		else if (player->rotation.z <= -1)
		{
			player->rotation.z += playerVelocity / 2;
		}
		else if (player->rotation.z >= 1)
		{
			player->rotation.z -= playerVelocity / 2;
		}
		else if (player->rotation.x >= 1)
		{
			player->rotation.x -= playerVelocity;
		}
		else
		{
			player->rotation.x = 0;
			//player->rotation.y = 0;
			player->rotation.z = 0;
		}
	}

//攻撃
	Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (playerDieFlag == true)
	{
		player->position.y-=0.1;
		player->rotation.x++;
		player->rotation.z++;
	}
	if (gameEndMovieFlag == true)
	{
		player->position.z += 2;
		player->position.y += 0.5;
		player->rotation.z += 2;

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
		hpBar = 0;
		playerDieFlag = true;
	}
}


void Player::Attack()
{
	if (input->isMouseKey() && keyInput == true)
	{
		coolTimer--;
		if (coolTimer <= 0)
		{
			
			XMVECTOR velocity = { 0,0,1 };
			velocity = transform(velocity, player->matWorld);
			//レティクルに向かって飛ぶ
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

XMVECTOR Player::ease_in(const XMVECTOR& start, const XMVECTOR& end, float t)
{
	t = t * t;
	return start * (1.0f - t) + end * t;
}




