#include "Enemy.h"
#include <stdlib.h>
#include <time.h>

void(Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,//接近
	&Enemy::ApproachL,
	&Enemy::OutApproach,
	&Enemy::LeaveL,	//離脱
	&Enemy::LeaveR,
	&Enemy::LeaveF,
	&Enemy::Stop, //止まる
	&Enemy::CircleR,//円運動右からくる
	&Enemy::CircleL,//左からくる
	&Enemy::CircleInfinity,//∞上にまわる
	&Enemy::OneWayR,//右から左に行く
	&Enemy::OneWayL,
	&Enemy::OneWayMovieR,
	&Enemy::OneWayMovieL,
	&Enemy::BossApproach,
	&Enemy::BossVertical,//bossが縦方向に動く
	&Enemy::BossVerticalL,
	&Enemy::BossSide,//bossが横方向に動く
	&Enemy::BossSideUp,
	&Enemy::BossBarrier,
	&Enemy::BossStop,
	&Enemy::BossDead,
	&Enemy::BossMiniStop,
	&Enemy::BossMiniBarrier,
	&Enemy::BossMiniDead,
	&Enemy::BossPhaseChange,
	&Enemy::None
};

void Enemy::Init(Model* enemyModel, XMFLOAT3 position, Model* bulletModel, XMFLOAT3 scale, bool attackFlag)
{
	enemyModel_ = enemyModel;
	bulletModel_ = bulletModel;
	enemy->SetModel(enemyModel);
	enemy->scale = { scale };
	this->attackFlag = attackFlag;

	enemy->matWorld.r[3].m128_f32[0] = position.x;
	enemy->matWorld.r[3].m128_f32[1] = position.y;
	enemy->matWorld.r[3].m128_f32[2] = position.z;

	enemy->SetPosition({ enemy->matWorld.r[3].m128_f32[0] ,enemy->matWorld.r[3].m128_f32[1] ,enemy->matWorld.r[3].m128_f32[2] });

	ShotInit();

}


void Enemy::Update()
{
	//デスフラグが立っている弾を消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	phaseNumber = static_cast<int>(phase);
	(this->*spFuncTable[phaseNumber])();

	//弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
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
}

void Enemy::PShot()
{
	//敵が攻撃するかしないか
	if (attackFlag == false)
	{
		shotEndFlag = true;
		return;
	}

	//弾を撃つ
	shotTimer--;
	if (shotTimer <= 0)
	{
		shotEndFlag = true;
		FrontShot();
		shotTimer = shotInterval;
	}
}

void Enemy::PMovieShot()
{
	//弾を撃つ
	shotTimer--;
	if (shotTimer <= 0)
	{
		shotEndFlag = true;
		LowerShot();
		shotTimer = shotInterval;
	}
}

void Enemy::PHoming()
{
	//敵が攻撃するかしないか
	if (attackFlag == false)
	{
		shotEndFlag = true;
		return;
	}
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
	addCircleX = cos(radius) * (lengthBoss);
	addCircleZ = sin(radius) * (lengthBoss);

	//敵の座標に足す
	enemy->position.x -= addCircleX;
	enemy->position.z -= addCircleZ;

	//角度をたして円状に動かす
	angle += angleVec;
}

void Enemy::PCircleBoss(float addCircleSize)
{
	//円の角度
	radius = angle * 3.14f / 180.0f;

	//円状の場所
	addCircleX = cos(radius) * (lengthBoss + addCircleSize);
	addCircleZ = sin(radius) * (lengthBoss + addCircleSize);

	//敵の座標に足す
	enemy->position.x = bossPos.x - addCircleX;
	enemy->position.z = bossPos.z - addCircleZ;

	//角度をたして円状に動かす
	angle += angleVec;
}

void Enemy::PChild()
{
	srand(time(NULL));
	randAdd = rand() % 1 + 0.8;
	enemy->position.x += bossVec.x * randAdd;
	enemy->position.y += bossVec.y * randAdd;
	enemy->position.z += bossVec.z * randAdd;

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

void Enemy::PChangeBossDead()
{
	if (bossHp <= 5)
	{
		phase = Phase::BossPhaseChange;
	}
}

XMVECTOR Enemy::ease_in(const XMVECTOR& start, const XMVECTOR& end, float t)
{
	t = t * t;
	return start * (1.0f - t) + end * t;
}

XMVECTOR Enemy::lerp(const XMVECTOR& start, const XMVECTOR& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

void Enemy::Approach()
{
	leaveTime = leaveTimeInit;
	enemy->position.x += ApproachSpeed;

	if (enemy->position.x >= -10)
	{
		phase = Phase::CircleL;
	}
}

void Enemy::ApproachL()
{
	leaveTime = leaveTimeInit;
	enemy->position.x -= ApproachSpeed;

	if (enemy->position.x >= 10)
	{
		phase = Phase::CircleR;
	}
}

void Enemy::OutApproach()
{
	enemy->position.z += outApproachSpeed;

	if (enemy->position.z >= playerWorldPos.z + 100)
	{
		phase = Phase::ApproachL;
	}
}

void Enemy::LeaveL()
{
	enemy->position.x -= outApproachSpeed;
	if (enemy->position.z <= cameraZ)
	{
		isDead = true;
	}
}

void Enemy::LeaveR()
{

	enemy->position.x += outApproachSpeed;

	
	if (enemy->position.z <= cameraZ)
	{
		isDead = true;
	}
}

void Enemy::LeaveF()
{
	enemy->position.z -= outApproachSpeed;
	if (enemy->position.z <= cameraZ)
	{
		isDead = true;
	}
}

void Enemy::Stop()
{
	PHoming();

	if (enemy->position.z <= playerWorldPos.z)
	{
		phase = Phase::LeaveL;
	}
}

void Enemy::CircleR()
{
	PCircleR();
	PShot();
	enemy->position.z += cameraZ;
	enemy->rotation.z++;
	PLeaveR();
}

void Enemy::CircleL()
{
	PCircleL();
	PShot();
	enemy->position.z += cameraZ;
	enemy->rotation.z++;
	PLeaveR();
}

void Enemy::OneWayR()
{
	enemy->position.x += OWRSpeed;

	enemy->position.z += cameraZ;

	if (enemy->position.x <= -OWMaxRange)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= OWMinRange)
	{
		OWRSpeed *= -1;
	}

	PHoming();

	if (attackFlag == true)
	{
		//leaveTimeが０以下になったら離脱フェイズにいく
		PLeaveF();
	}
}

void Enemy::OneWayL()
{
	enemy->position.x -= OWRSpeed;

	enemy->position.z += cameraZ;

	if (enemy->position.x <= -OWMinRange)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= OWMaxRange)
	{
		OWRSpeed *= -1;
	}

	PHoming();

	if (attackFlag == true)
	{
		PLeaveF();
	}
}

void Enemy::OneWayMovieR()
{
	enemy->position.x += OWRSpeed;

	if (enemy->position.x <= -OWMaxRange)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= OWMaxRange)
	{
		OWRSpeed *= -1;
	}
}

void Enemy::OneWayMovieL()
{
	enemy->position.x -= OWRSpeed;

	if (enemy->position.x <= -OWMaxRange)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= OWMaxRange)
	{
		OWRSpeed *= -1;
	}

}

void Enemy::BossApproach()
{
	bossMovieFlag = true;
	enemy->position.y -=0.5;
	if (enemy->position.y <= 0)
	{
		phase = Phase::CircleInfinity;
		bossMovieFlag = false;
	}
}

void Enemy::CircleInfinity()
{
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
	if (bossHp <= 25)
	{
		phase = Phase::BossPhaseChange;
	}
}

void Enemy::BossVertical()
{
	enemy->position.y += 0.8;
	PHoming();
	if (enemy->position.y >= 30.4)
	{
		phase = Phase::BossSideUp;
	}
	PChangeBossDead();
}

void Enemy::BossVerticalL()
{
	enemy->position.y -= 0.8;
	PHoming();
	if (enemy->position.y <= -30.4)
	{
		phase = Phase::BossSide;
	}
	PChangeBossDead();
}

void Enemy::BossSide()
{
	enemy->position.x += 0.8;
	PHoming();
	if (enemy->position.x >= 60)
	{
		phase = Phase::BossVertical;
	}
	PChangeBossDead();
}

void Enemy::BossSideUp()
{
	enemy->position.x -= 0.8;
	PHoming();
	if (enemy->position.x <= -60)
	{
		phase = Phase::BossVerticalL;
	}

	PChangeBossDead();
}

void Enemy::BossBarrier()
{
	enemy->position.x = 0;
	enemy->position.y = 0;
	if (bossHp <= 0)
	{
		phase = Phase::BossDead;
	}
}

void Enemy::BossStop()
{
}

void Enemy::BossDead()
{
	enemy->position.y -= 0.1;
	enemy->rotation.x += 0.1;
	enemy->rotation.z += 0.1;
}

void Enemy::BossMiniStop()
{
	PChild();//ボスに追従
	PShotMinor();//弾を前に発射
	PCircleBoss();//ボスの周りをまわる
	enemy->rotation.y++;

	//最終形態へ
	if (barrierPhaseFlag == true)
	{
		barrierTime++;
		if (barrierTime >= 60)
		{
			phase = Phase::BossMiniBarrier;
		}
	}
}

void Enemy::BossMiniBarrier()
{
	PShotMinor();//弾を前に発射
	PCircleBoss(25.0f); //ボスの周りをまわる
	PChildHoming();
}

void Enemy::BossMiniDead()
{
	enemy->position.y--;
	enemy->rotation.x++;
	enemy->rotation.z++;
}

void Enemy::BossPhaseChange()
{
	if (bossHp <= 5)
	{
		if (barrierFlag == false)
		{
			barrierPos = enemy->GetPosition();
		}
		barrierFlag = true;
		enemy->rotation.y += 3;
		changeTime++;
		/*EasingTime();
		enemy->position.x = ease_in({ barrierPos.x,barrierPos.y,barrierPos.z },
			{ 0,0,920 }, timeRate).m128_f32[0];
		enemy->position.y = ease_in({ barrierPos.x,barrierPos.y,barrierPos.z },
			{ 0,0,920 }, timeRate).m128_f32[1];
		enemy->position.z = ease_in({ barrierPos.x,barrierPos.y,barrierPos.z },
			{ 0,0,920 }, timeRate).m128_f32[2];*/
		if (changeTime >= 120)
		{
			barrierPhaseFlag = true;
			phase = Phase::BossBarrier;
			changeTime = 0;
		}
	}
	else if (bossHp <= 25)
	{
		barrierFlag = true;
		enemy->rotation.y+=3;
		changeTime++;
		if (changeTime >= 180)
		{
			barrierFlag = false;
			phase = Phase::BossSide;
			changeTime = 0;
		}
	}
	
}

void Enemy::None()
{
}

void Enemy::Homing()
{
	//assert(player_);
	const float speed = 1.5f;//1フレーム進む距離
	const float cameraZAdd = 10;
	playerPos = { playerWorldPos.x,playerWorldPos.y ,playerWorldPos.z };

	//差分ベクトル
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z + cameraZAdd;

	//正規化
	playerPos = XMVector3Normalize(playerPos);
	lockOn = XMVector3Normalize(lockOn);
	//正規化ベクトルと1フレーム進む距離をかける
	lockOn = lerp(lockOn,playerPos,timeRate) * speed;
	
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);

	//弾登録
	bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため
}

void Enemy::FrontShot()
{
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,{0,0,-2});

	//弾登録
	bullets_.push_back(std::move(newBullet));//move はユニークから譲渡するため
}

void Enemy::LowerShot()
{
	//弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position, { 0,-4,-2 });

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

void Enemy::OnCollision()
{
	isDead = true;
	enemy->position.z = -350;
	enemy->position.y = -200;
	phase = Phase::None;
}

void Enemy::OnBossCollision()
{
	if (barrierFlag == false && bossMovieFlag == false)
	{
		bossHp--;
		hpBar -= hpBarMax / 50;
		if (bossHp <= 0)
		{
			isDead = true;
		}
	}
}

void Enemy::OnBossMiniCollision()
{
	bossMiniHp--;
	if (bossMiniHp <= 0)
	{
		phase = Phase::BossMiniDead;
		isDead = true;
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

XMFLOAT3 Enemy::GetWorldPosition()
{
	XMFLOAT3 worldPos;
	worldPos.x = enemy->matWorld.r[3].m128_f32[0];
	worldPos.y = enemy->matWorld.r[3].m128_f32[1];
	worldPos.z = enemy->matWorld.r[3].m128_f32[2];
	return worldPos;
}