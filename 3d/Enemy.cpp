#include "Enemy.h"
#include <stdlib.h>
#include <time.h>

void(Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,//�ڋ�
	&Enemy::ApproachL,
	&Enemy::OutApproach,
	&Enemy::LeaveL,	//���E
	&Enemy::LeaveR,
	&Enemy::LeaveF,
	&Enemy::Stop, //�~�܂�
	&Enemy::CircleR,//�~�^���E���炭��
	&Enemy::CircleL,//�����炭��
	&Enemy::CircleInfinity,//����ɂ܂��
	&Enemy::OneWayR,//�E���獶�ɍs��
	&Enemy::OneWayL,
	&Enemy::BossApproach,
	&Enemy::BossVertical,//boss���c�����ɓ���
	&Enemy::BossVerticalL,
	&Enemy::BossSide,//boss���������ɓ���
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
	//�f�X�t���O�������Ă���e������
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	phaseNumber = static_cast<int>(phase);
	(this->*spFuncTable[phaseNumber])();


	

	//�e�̍X�V
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

void Enemy::PCircleR()
{
	//�~�̊p�x
	radius = angle * 3.14f / 180.0f;

	//�~��̏ꏊ
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//�G�̍��W�ɑ���
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//�p�x�������ĉ~��ɓ�����
	angle += angleVec;
}

void Enemy::PCircleL()
{
	//�~�̊p�x
	radius = angle * 3.14f / 180.0f;

	//�~��̏ꏊ
	addCircleX = cos(radius) * length;
	addCircleY = sin(radius) * length;

	//�G�̍��W�ɑ���
	enemy->position.x += addCircleX;
	enemy->position.y += addCircleY;

	//�p�x�������ĉ~��ɓ�����
	angle -= angleVec;
}

void Enemy::PShot()
{
	//�G���U�����邩���Ȃ���
	if (attackFlag == false)
	{
		shotEndFlag = true;
		return;
	}

	//�e������
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
	//�G���U�����邩���Ȃ���
	if (attackFlag == false)
	{
		shotEndFlag = true;
		return;
	}
		//�e������
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
		phase = Phase::LeaveL;//�Ƃǂ܂点�Ăǂ����ɍs��������
	}
}

void Enemy::PShotMinor()
{
	//�e������
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
		phase = Phase::LeaveR;//�Ƃǂ܂点�Ăǂ����ɍs��������
	}
}

void Enemy::PLeaveF()
{
	leaveTime--;
	if (leaveTime <= 0 && shotEndFlag == true)
	{
		phase = Phase::LeaveF;//�Ƃǂ܂点�Ăǂ����ɍs��������
	}
}

void Enemy::PCircleZ()
{
	//�~�̊p�x
	radius = angle * 3.14f / 180.0f;

	//�~��̏ꏊ
	addCircleX = cos(radius) * (lengthBoss);
	addCircleZ = sin(radius) * (lengthBoss);

	//�G�̍��W�ɑ���
	enemy->position.x -= addCircleX;
	enemy->position.z -= addCircleZ;

	//�p�x�������ĉ~��ɓ�����
	angle += angleVec;
}

void Enemy::PCircleBoss(float addCircleSize)
{
	//�~�̊p�x
	radius = angle * 3.14f / 180.0f;

	//�~��̏ꏊ
	addCircleX = cos(radius) * (lengthBoss + addCircleSize);
	addCircleZ = sin(radius) * (lengthBoss + addCircleSize);

	//�G�̍��W�ɑ���
	enemy->position.x = bossPos.x - addCircleX;
	enemy->position.z = bossPos.z - addCircleZ;

	//�p�x�������ĉ~��ɓ�����
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

void Enemy::PWait()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		//phaseMini = BossPhase::MiniVerticalLUF;
	}
}

void Enemy::PWaitLB()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		//phaseMini = BossPhase::MiniVerticalLUB;
	}
}

void Enemy::PWaitR()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		//phaseMini = BossPhase::MiniVerticalRUF;
	}
}

void Enemy::PWaitRB()
{
	waitTimer--;
	if (waitTimer <= 0)
	{
		//phaseMini = BossPhase::MiniVerticalRUB;
	}
}

void Enemy::PChildHoming()
{
	//�e������
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

	if (enemy->position.x <= -80)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= 20)
	{
		OWRSpeed *= -1;
	}

	PHoming();

	if (attackFlag == true)
	{
		PLeaveF();
	}
}

void Enemy::OneWayL()
{
	enemy->position.x -= OWRSpeed;

	enemy->position.z += cameraZ;

	if (enemy->position.x <= -20)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= 80)
	{
		OWRSpeed *= -1;
	}

	PHoming();

	if (attackFlag == true)
	{
		PLeaveF();
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
	PChild();//�{�X�ɒǏ]
	PShotMinor();//�e��O�ɔ���
	PCircleBoss();//�{�X�̎�����܂��
	enemy->rotation.y++;

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
	PShotMinor();//�e��O�ɔ���
	PCircleBoss(25.0f); //�{�X�̎�����܂��
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

	if (bossHp <= 25)
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
	const float speed = 1.5f;//1�t���[���i�ދ���
	const float cameraZAdd = 10;
	playerPos = { playerWorldPos.x,playerWorldPos.y ,playerWorldPos.z };

	//�����x�N�g��
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z + cameraZAdd;

	//���K��
	playerPos = XMVector3Normalize(playerPos);
	lockOn = XMVector3Normalize(lockOn);
	//���K���x�N�g����1�t���[���i�ދ�����������
	lockOn = lerp(lockOn,playerPos,timeRate) * speed;
	
	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);

	//�e�o�^
	bullets_.push_back(std::move(newBullet));//move �̓��j�[�N������n���邽��
}

void Enemy::FrontShot()
{
	
	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,{0,0,-2});

	//�e�o�^
	bullets_.push_back(std::move(newBullet));//move �̓��j�[�N������n���邽��
}

void Enemy::ChildHoming()
{
	//assert(player_);
	const float speed = 2.0f;//1�t���[���i�ދ���

	//�����x�N�g��
	lockOn.m128_f32[0] = bossPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = bossPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = bossPos.z - GetWorldPosition().z;

	//���K��
	lockOn = XMVector3Normalize(lockOn);

	//���K���x�N�g����1�t���[���i�ދ�����������
	lockOn *= speed;
	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position, lockOn);

	//�e�o�^
	bullets_.push_back(std::move(newBullet));//move �̓��j�[�N������n���邽��
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
	if (barrierFlag == false || bossMovieFlag == false)
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
	//���˃^�C�}�[������
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