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

void Enemy::PShotMinor()
{
	//弾を撃つ
	shotMinorTimer--;
	if (shotMinorTimer <= 0)
	{

		FrontShot();
		shotMinorTimer = shotMinorInterval;
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

void Enemy::PCircleZ()
{
	//円の角度
	radius = angle * 3.14f / 180.0f;

	//円状の場所
	addCircleX = cos(radius) * (length + 1.8);
	addCircleZ = sin(radius) * (length + 1.8);

	//敵の座標に足す
	enemy->position.x -= addCircleX;
	enemy->position.z -= addCircleZ;

	//角度をたして円状に動かす
	angle += angleVec;
}

void Enemy::PChild()
{
	enemy->position.x += bossVec.x /= 1.1;
	enemy->position.y += bossVec.y /= 1.1;
	enemy->position.z += bossVec.z /= 1.1;
	PShotMinor();
}

void Enemy::PWait()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		phaseMini = BossPhase::MiniVerticalLUF;
	}
}

void Enemy::PWaitLB()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		phaseMini = BossPhase::MiniVerticalLUB;
	}
}

void Enemy::PWaitR()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		phaseMini = BossPhase::MiniVerticalRUF;
	}
}

void Enemy::PWaitRB()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		phaseMini = BossPhase::MiniVerticalRUB;
	}
}

void Enemy::PChildHoming()
{
	//弾を撃つ
	shotTimer--;
	if (shotTimer <= 0)
	{
		shotEndFlag = true;
		ChildHoming();
		shotTimer = shotInterval;
	}
}




XMVECTOR Enemy::ease_in(const XMVECTOR& start, const XMVECTOR& end, float t)
{
	t = t * t;
	return start * (1.0f - t) + end * t;
}

void Enemy::Init(Model* enemyModel, XMFLOAT3 position, Model *bulletModel ,XMFLOAT3 scale)
{
	enemyModel_ = enemyModel;
	bulletModel_ = bulletModel;
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
		if (bossHp <= 25)
		{
			phase = Phase::BossSide;
		}
		
		break;

	case Phase::OneWayL:
		//EasingTime();

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


	case Phase::BossVertical:
		enemy->position.y+= 0.8;
		PHoming();
		if (enemy->position.y >= 30.4)
		{
			phase = Phase::BossSideUp;
		}
		
		break;
		case Phase::BossVerticalL:
		enemy->position.y-= 0.8;
		PHoming();
		if (enemy->position.y <= -30.4)
		{
			phase = Phase::BossSide;
		}

		break;
	case Phase::BossSide:
		enemy->position.x+= 0.8;
		PHoming();
		if (enemy->position.x >= 60)
		{
			phase = Phase::BossVertical;
		}
		break;
	case Phase::BossSideUp:
		enemy->position.x-= 0.8;
		PHoming();
		if (enemy->position.x <= -60)
		{
			phase = Phase::BossVerticalL;
		}
		break;
	case Phase::None:

		break;
	default:
		break;
	}

	switch (phaseMini)
	{

	case BossPhase::None:
		break;
	case BossPhase::MiniStop:
		PChild();
		//PCircleZ();
		/*if (childNumber == 1)
		{
			enemy->position.x = 0;
			angle = 90;

			PWait();
		}
		else if (childNumber == 2)
		{
			enemy->position.x = 0;
			angle = 180;
			PWaitLB();
		}
		else if (childNumber == 3)
		{
			enemy->position.x = 0;
			angle = 270;
			PWaitR();
		}
		else if (childNumber == 4)
		{
			enemy->position.x = 0;
			angle = 0;
			PWaitRB();
		}
		else if (childNumber == 5)
		{
			angle = 60;
			PWait();
		}
		else if (childNumber == 6)
		{
			angle = 60;
			PWaitLB();
		}
		else if (childNumber == 7)
		{
			angle = 60;
			PWaitR();
		}
		else if (childNumber == 8)
		{
			angle = 60;
			PWaitRB();
		}*/

		break;
	case BossPhase::MiniVerticalLUF:

		//EasingTime();
		//MiniPosLUF = ease_in({ MiniPosLUF }, { -15,MiniPosLUF.m128_f32[1], MiniPosLUF.m128_f32[2] }, timeRate);

		//enemy->position.x = MiniPosLUF.m128_f32[0];
		//enemy->position.y = MiniPosLUF.m128_f32[1];
		//enemy->position.z = MiniPosLUF.m128_f32[2];


		if (circleZFlag == true)
		{
			PCircleZ();
			if (childShotRange.x - 25 <= enemy->position.x && childShotRange.x + 25 >= enemy->position.x
				&& childShotRange.z >= enemy->position.z)
			{
			}
			else
			{
				PChildHoming();
			}
		}
		else
		{
			enemy->position.x = -90;
			circleZFlag = true;
		}
		//flag	wo tatete PcircleZ wo tukau

	/*	enemy->position.y = MiniPosLUF.m128_f32[1];
		enemy->position.z = MiniPosLUF.m128_f32[2];*/

		break;
	case BossPhase::MiniVerticalLUB:
		if (circleZFlag == true)
		{
			PCircleZ();
			if (childShotRange.x - 25 <= enemy->position.x && childShotRange.x + 25 >= enemy->position.x
				&& childShotRange.z >= enemy->position.z)
			{
			}
			else
			{

				PChildHoming();
			}
		}
		else
		{
			enemy->position.z = 930;
			circleZFlag = true;
		}

		break;
	case BossPhase::MiniVerticalRUF:
		if (circleZFlag == true)
		{
			PCircleZ();
			if (childShotRange.x - 25 <= enemy->position.x && childShotRange.x + 25 >= enemy->position.x
				&& childShotRange.z >= enemy->position.z)
			{
			}
			else
			{
				//PChildHoming();
			}
		}
		else
		{
			enemy->position.x = 90;
			circleZFlag = true;
		}
		break;
	case BossPhase::MiniVerticalRUB:
		if (circleZFlag == true)
		{
			PCircleZ();
			if (childShotRange.x - 25 <= enemy->position.x && childShotRange.x + 25 >= enemy->position.x
				&& childShotRange.z >= enemy->position.z)
			{
			}
			else
			{
				PChildHoming();
			}
		}
		else
		{
			enemy->position.z = 990;
			circleZFlag = true;
		}
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
	const float speed = 1.5f;//1フレーム進む距離

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

void Enemy::ChildHoming()
{
	//assert(player_);
	const float speed = 2.0f;//1フレーム進む距離

	//差分ベクトル
	lockOn.m128_f32[0] = bossPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = bossPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = bossPos.z - GetWorldPosition().z;

	//正規化
	lockOn = XMVector3Normalize(lockOn);

	//正規化ベクトルと1フレーム進む距離をかける
	lockOn *= speed;
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position, lockOn);
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
	

	nowCount = timeGetTime();

	elapsedCount = nowCount - startCount;

	float elapsedTime = static_cast<float> (elapsedCount) / 1000000.0f;

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
	enemy->position.z = -350;
	phase = Phase::None;
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
