#include "Enemy.h"

void Enemy::PCircleR()
{
	//円の角度
	radius = angle * 3.14f / 180.0f;

	//円状の場所
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//敵の座標に足す
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//角度をたして円状に動かす
	angle += angleVec;

	//if (angle >= 460.0f)
	//{
	//	shotTimer;
	//	//enemy->position.z -= 0.5;
	//}
	
}

void Enemy::PCircleL()
{
	//円の角度
	radius = angle * 3.14f / 180.0f;

	//円状の場所
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//敵の座標に足す
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//角度をたして円状に動かす
	angle -= angleVec;

	//if (angle <= -270.0f)
	//{
	//	shotTimer = shotInterval;
	//	//enemy->position.z -= 0.5;
	//}
	

	
}

void Enemy::PShot()
{
	//弾を撃つ
	shotTimer--;
	if (shotTimer <= 0)
	{
		shotEndFlag = true;
		FrontShot();
		shotTimer = shotInterval;
	}
}

void Enemy::PHoming()
{
	//弾を撃つ
	shotTimer--;
	if (shotTimer <= 0)
	{
		shotEndFlag = true;
		Homing();
		shotTimer = shotInterval;
	}
}

void Enemy::PLeaveL()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveL;//とどまらせてどっかに行かせたい
	}
}

void Enemy::PLeaveR()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveR;//とどまらせてどっかに行かせたい
	}
}

void Enemy::PLeaveF()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveF;//とどまらせてどっかに行かせたい
	}
}


XMVECTOR Enemy::ease_in(const XMVECTOR& start, const XMVECTOR& end, float t)
{
	t = t * t;
	return start * (1.0f - t) + end * t;
}

void Enemy::Init(Model* enemyModel, XMFLOAT3 position, XMFLOAT3 scale)
{
	enemyModel_ = enemyModel;
	bulletModel_ = enemyModel;
	enemy->SetModel(enemyModel);
	enemy->scale = { scale };

	enemy->matWorld.r[3].m128_f32[0] = position.x;
	enemy->matWorld.r[3].m128_f32[1] = position.y;
	enemy->matWorld.r[3].m128_f32[2] = position.z;

	enemy->SetPosition({ enemy->matWorld.r[3].m128_f32[0] ,enemy->matWorld.r[3].m128_f32[1] ,enemy->matWorld.r[3].m128_f32[2] });
 
	ShotInit();
	//PhaseInit(rightMoveTrue);
	OneWayPos.m128_f32[0] = enemy->position.x;
	OneWayPos.m128_f32[1] = enemy->position.y;
	OneWayPos.m128_f32[2] = enemy->position.z;

	MiniPosLUF.m128_f32[0] = enemy->position.x;
	MiniPosLUF.m128_f32[1] = enemy->position.y;
	MiniPosLUF.m128_f32[2] = enemy->position.z;
}


void Enemy::Update()
{
	//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	switch (phase)
	{
	case Phase::Approach:
		leaveTime = leaveTimeInit;
		enemy->position.x -= ApproachSpeed;

		if (enemy->position.x >= 0)
		{
			phase = Phase::CircleR;
		}
		
		break;
	case Phase::ApproachL:
		leaveTime = leaveTimeInit;
		enemy->position.x += ApproachSpeed;

		if (enemy->position.x >= 0 )
		{
			phase = Phase::CircleL;
		}

		break;
	case Phase::OutApproach:
		enemy->position.z += outApproachSpeed;

		if (enemy->position.z >= playerWorldPos.z + 100)
		{
			phase = Phase::ApproachL;
		}
		break;
	case Phase::LeaveL:
		enemy->position.x -= outApproachSpeed;
		break;

	case Phase::LeaveR:
		enemy->position.x += outApproachSpeed;

		break;
	case Phase::LeaveF:
		enemy->position.z -= outApproachSpeed;

		break;
	case Phase::Stop:

		PHoming();

		if (enemy->position.z <= playerWorldPos.z)
		{
			phase = Phase::LeaveL;
		}

		break;
	case Phase::CircleR://右から来たver 90ど

		PCircleR();
		PShot();
		enemy->position.z += cameraZ;
		PLeaveR();
		
		
		break;
	case Phase::CircleL://左から来たver 270ど

		PCircleL();
		PShot();
		enemy->position.z += cameraZ;
		PLeaveR();
		
		break;

	case Phase::CircleInfinity:
		time++;
		PCircleR();
		PShot();
		
		if (angle >= 450)
		{
			angleVec = -2;
			isL = true;
		}
		if (angle <= 90 && isL == true)
		{
			angleVec = 2;
			isL = false;
		}

		if (time >= 600)
		{
			/*EasingTime();
			enemy->position.x = ease_in({ enemy->position.x,enemy->position.y,enemy->position.z },
				{ 0,0,enemy->position.z }, timeRate).m128_f32[0];
			enemy->position.x = ease_in({ enemy->position.x,enemy->position.y,enemy->position.z },
				{ 0,0,enemy->position.z }, timeRate).m128_f32[1];
			enemy->position.x = ease_in({ enemy->position.x,enemy->position.y,enemy->position.z },
				{ 0,0,enemy->position.z }, timeRate).m128_f32[2];*/
			//phase = Phase::BossMiniVertical;
		}
		break;

	case Phase::OneWayL:
		EasingTime();

		//OneWayPos = ease_in(OneWayPos, { -200,OneWayPos.m128_f32[1] ,OneWayPos.m128_f32[2]  },timeRate);
		//
		//enemy->position.x = OneWayPos.m128_f32[0];
		//enemy->position.y = OneWayPos.m128_f32[1];
		//enemy->position.z = OneWayPos.m128_f32[2];
		
		enemy->position.x -= OWRSpeed;

		enemy->position.z += cameraZ;

		if (enemy->position.x <= -100)
		{
			OWRSpeed *= -1;
		}
		if (enemy->position.x >= 100)
		{
			OWRSpeed *= -1;
		}
		//shotEndFlag = true;
		PHoming();
		PLeaveF();

		break;

	case Phase::OneWayR:

		enemy->position.x += OWRSpeed;

		enemy->position.z += cameraZ;

		if (enemy->position.x <= -100)
		{
			OWRSpeed *= -1;
		}
		if (enemy->position.x >= 100)
		{
			OWRSpeed *= -1;
		}
		//shotEndFlag = true;
		PHoming();
		PLeaveF();

		break;

	case Phase::BossMiniVertical:
		
		
	default:
		break;
	}

	switch (phaseMini)
	{


	case BossPhase::MiniStop:
		break;
	case BossPhase::MiniVerticalLUF:
		EasingTime();
		MiniPosLUF = ease_in(MiniPosLUF, { -20, MiniPosLUF.m128_f32[1], playerWorldPos.z }, timeRate);

		enemy->position.x = MiniPosLUF.m128_f32[0];
		enemy->position.y = MiniPosLUF.m128_f32[1];
		enemy->position.z = MiniPosLUF.m128_f32[2];

		break;
	case BossPhase::MiniVerticalLUB:
		break;
	case BossPhase::MiniVerticalRUF:
		break;
	case BossPhase::MiniVerticalRUB:
		break;
	case BossPhase::MiniVerticalLDF:
		break;
	case BossPhase::MiniVerticalLDB:
		break;
	case BossPhase::MiniVerticalRDF:
		break;
	case BossPhase::MiniVerticalRDB:
		break;
	case BossPhase::MiniSideLUF:
		break;
	case BossPhase::MiniSideLUB:
		break;
	case BossPhase::MiniSideRUF:
		break;
	case BossPhase::MiniSideRUB:
		break;
	case BossPhase::MiniSideLDF:
		break;
	case BossPhase::MiniSideLDB:
		break;
	case BossPhase::MiniSideRDF:
		break;
	case BossPhase::MiniSideRDB:
		break;
	default:
		break;
	}

	//弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		//bullet->SetLockOnPosition(GetWorldPosition(), playerWorldPos);
		bullet->Update();
	}

	enemy->Update();
}

void Enemy::Draw()
{
	enemy->Draw();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}
}

void Enemy::Homing()
{
	//assert(player_);
	const float speed = 3.0f;//1フレーム進む距離

	//差分ベクトル
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z;

	//正規化
	lockOn = XMVector3Normalize(lockOn);

	//正規化ベクトルと1フレーム進む距離をかける
	lockOn *= speed;
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


	//弾登録
	bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため
}

void Enemy::FrontShot()
{
	
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,{0,0,-2});
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


	//弾登録
	bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため
}

void Enemy::PhaseInit(bool rightMoveTrue)
{
	if (!rightMoveTrue)
	{
		phase = Phase::CircleR;
	}
}

void Enemy::EasingTime()
{
	float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

	nowCount = timeGetTime();

	elapsedCount = nowCount - startCount;

	timeRate = elapsedTime / maxTime;
}

XMFLOAT3 Enemy::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = enemy->matWorld.r[3].m128_f32[0];
	worldPos.y = enemy->matWorld.r[3].m128_f32[1];
	worldPos.z = enemy->matWorld.r[3].m128_f32[2];
	return worldPos;
}

void Enemy::OnCollision()
{
	isDead = true;
	enemy->position.z -= 350;
	//phase = Phase::Leave;
}

void Enemy::OnBossCollision()
{
	bossHp--;
	bossDamage = true;
	hpBar -= hpBarMax / 50;
	if (bossHp <= 0)
	{
		isDead = true;
		enemy->position.z -= 350;
	}
}

void Enemy::ShotInit()
{
	//発射タイマー初期化
	shotTimer = shotInterval;
}

void Enemy::ShotTimerLower(int32_t lowerTime)
{
	shotTimer -= lowerTime;
}

bool Enemy::IsTimer()
{
	if (shotTimer <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
