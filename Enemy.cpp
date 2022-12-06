#include "Enemy.h"

void Enemy::PCircleR()
{
	//‰~‚ÌŠp“x
	radius = angle * 3.14f / 180.0f;

	//‰~ó‚ÌêŠ
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//“G‚ÌÀ•W‚É‘«‚·
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//Šp“x‚ğ‚½‚µ‚Ä‰~ó‚É“®‚©‚·
	angle += angleVec;

	//if (angle >= 460.0f)
	//{
	//	shotTimer;
	//	//enemy->position.z -= 0.5;
	//}
	
}

void Enemy::PCircleL()
{
	//‰~‚ÌŠp“x
	radius = angle * 3.14f / 180.0f;

	//‰~ó‚ÌêŠ
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//“G‚ÌÀ•W‚É‘«‚·
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//Šp“x‚ğ‚½‚µ‚Ä‰~ó‚É“®‚©‚·
	angle -= angleVec;

	//if (angle <= -270.0f)
	//{
	//	shotTimer = shotInterval;
	//	//enemy->position.z -= 0.5;
	//}
	

	
}

void Enemy::PShot()
{
	//’e‚ğŒ‚‚Â
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
	//’e‚ğŒ‚‚Â
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
		phase = Phase::LeaveL;//‚Æ‚Ç‚Ü‚ç‚¹‚Ä‚Ç‚Á‚©‚És‚©‚¹‚½‚¢
	}
}

void Enemy::PLeaveR()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveR;//‚Æ‚Ç‚Ü‚ç‚¹‚Ä‚Ç‚Á‚©‚És‚©‚¹‚½‚¢
	}
}

void Enemy::PLeaveF()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveF;//‚Æ‚Ç‚Ü‚ç‚¹‚Ä‚Ç‚Á‚©‚És‚©‚¹‚½‚¢
	}
}

void Enemy::PCircleZ()
{
	//‰~‚ÌŠp“x
	radius = angle * 3.14f / 180.0f;

	//‰~ó‚ÌêŠ
	addCircleX = cos(radius) * (length + 0.7);
	addCircleZ = sin(radius) * (length + 0.7);

	//“G‚ÌÀ•W‚É‘«‚·
	enemy->position.x -= addCircleX;
	enemy->position.z -= addCircleZ;

	//Šp“x‚ğ‚½‚µ‚Ä‰~ó‚É“®‚©‚·
	angle += angleVec;
}

void Enemy::PChild()
{
	enemy->position.x += bossVec.x /= 1.2;
	enemy->position.y += bossVec.y /= 1.2;
	enemy->position.z += bossVec.z /= 1.2;
}

void Enemy::PWait()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		phaseMini = BossPhase::MiniVerticalLUF;
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
	//ƒfƒXƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é’e‚ğÁ‚·
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
	case Phase::CircleR://‰E‚©‚ç—ˆ‚½ver 90‚Ç

		PCircleR();
		PShot();
		enemy->position.z += cameraZ;
		PLeaveR();
		
		
		break;
	case Phase::CircleL://¶‚©‚ç—ˆ‚½ver 270‚Ç

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


	case Phase::BossMiniVertical:
		
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
		//PChild();
		//PCircleZ();
		if (childNumber == 1)
		{
			angle = 90;
			PWait();
		}
		else if(childNumber == 2)
		{
			angle = 45;
			PWait();
		}
		else if (childNumber == 3)
		{
			angle = -90;
			PWaitR();
		}
		else if (childNumber == 4)
		{
			angle = -45;
			PWaitR();
		}
		else if (childNumber == 5)
		{
			angle = 90;
			PWait();
		}
		else if (childNumber == 6)
		{
			angle = 45;
			PWait();
		}
		else if (childNumber == 7)
		{
			angle = -90;
			PWaitR();
		}
		else if (childNumber == 8)
		{
			angle = -45;
			PWaitR();
		}

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
		}
		else
		{
			enemy->position.x = -30;
			circleZFlag = true;
		}
		//flag	wo tatete PcircleZ wo tukau

	/*	enemy->position.y = MiniPosLUF.m128_f32[1];
		enemy->position.z = MiniPosLUF.m128_f32[2];*/

		break;
	case BossPhase::MiniVerticalLUB:

		
		break;
	case BossPhase::MiniVerticalRUF:
		if (circleZFlag == true)
		{
			PCircleZ();
		}
		else
		{
			enemy->position.x = 30;
			circleZFlag = true;
		}
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

	//’e‚ÌXV
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
	const float speed = 2.0f;//1ƒtƒŒ[ƒ€i‚Ş‹——£

	//·•ªƒxƒNƒgƒ‹
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z;

	//³‹K‰»
	lockOn = XMVector3Normalize(lockOn);

	//³‹K‰»ƒxƒNƒgƒ‹‚Æ1ƒtƒŒ[ƒ€i‚Ş‹——£‚ğ‚©‚¯‚é
	lockOn *= speed;
	//’e¶¬
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


	//’e“o˜^
	bullets_.push_back(std::move(newBullet));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
}

void Enemy::FrontShot()
{
	
	//’e¶¬
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,{0,0,-2});
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


	//’e“o˜^
	bullets_.push_back(std::move(newBullet));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
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
	//”­Ëƒ^ƒCƒ}[‰Šú‰»
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
