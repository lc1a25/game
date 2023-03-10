#include "Enemy.h"

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
	&Enemy::BossVertical,//boss�̃~�j���c�����ɓ���
	&Enemy::BossVerticalL,
	&Enemy::BossSide,//boss�̃~�j���������ɓ���
	&Enemy::BossSideUp,
	&Enemy::BossStop,
	&Enemy::BossDead,
	&Enemy::None
};
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
	addCircleX = cos(radius) * (length + 1.8);
	addCircleZ = sin(radius) * (length + 1.8);

	//�G�̍��W�ɑ���
	enemy->position.x -= addCircleX;
	enemy->position.z -= addCircleZ;

	//�p�x�������ĉ~��ɓ�����
	angle += angleVec;
}

void Enemy::PChild()
{
	enemy->position.x += bossVec.x ;
	enemy->position.y += bossVec.y ;
	enemy->position.z += bossVec.z ;

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
	if (bossHp <= 0)
	{
		phase = Phase::BossDead;
		phaseMini = BossPhase::BossDead;
	}
}

XMVECTOR Enemy::ease_in(const XMVECTOR& start, const XMVECTOR& end, float t)
{
	t = t * t;
	return start * (1.0f - t) + end * t;
}

void Enemy::Init(Model* enemyModel, XMFLOAT3 position, Model *bulletModel ,XMFLOAT3 scale,bool attackFlag)
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
	//�f�X�t���O�������Ă���e������
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
	phaseNumber = static_cast<int>(phase);
	(this->*spFuncTable[phaseNumber])(); 
	

	switch (phaseMini)
	{

	case BossPhase::None:
		
		break;
	case BossPhase::BossDead:
		//enemy->position.y -= 0.1;
		PChild();
		enemy->rotation.x++;
		enemy->rotation.z++;
		break;
	case BossPhase::MiniStop:
		PChild();//�{�X�ɒǏ]
		PShotMinor();//�e��O�ɔ���
		enemy->rotation.y++;
		
	default:
		break;
	}

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

void Enemy::Approach()
{
	leaveTime = leaveTimeInit;
	enemy->position.x -= ApproachSpeed;

	if (enemy->position.x >= 0)
	{
		phase = Phase::CircleR;
	}
}

void Enemy::ApproachL()
{
	leaveTime = leaveTimeInit;
	enemy->position.x += ApproachSpeed;

	if (enemy->position.x >= 0)
	{
		phase = Phase::CircleL;
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
}

void Enemy::LeaveR()
{
	enemy->position.x += outApproachSpeed;
}

void Enemy::LeaveF()
{
	enemy->position.z -= outApproachSpeed;
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

void Enemy::CircleInfinity()
{
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

	if (bossHp <= 25)
	{
		phase = Phase::BossSide;
	}
}

void Enemy::OneWayR()
{
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

	if (enemy->position.x <= -100)
	{
		OWRSpeed *= -1;
	}
	if (enemy->position.x >= 100)
	{
		OWRSpeed *= -1;
	}

	PHoming();

	if (attackFlag == true)
	{
		PLeaveF();
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

void Enemy::BossStop()
{
}

void Enemy::BossDead()
{
	enemy->position.y -= 0.1;
	enemy->rotation.x++;
	enemy->rotation.z++;
}

void Enemy::None()
{
}

void Enemy::Homing()
{
	//assert(player_);
	const float speed = 1.5f;//1�t���[���i�ދ���

	//�����x�N�g��
	lockOn.m128_f32[0] = playerWorldPos.x - GetWorldPosition().x;
	lockOn.m128_f32[1] = playerWorldPos.y - GetWorldPosition().y;
	lockOn.m128_f32[2] = playerWorldPos.z - GetWorldPosition().z;

	//���K��
	lockOn = XMVector3Normalize(lockOn);

	//���K���x�N�g����1�t���[���i�ދ�����������
	lockOn *= speed;
	//�e����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(bulletModel_, enemy->position,lockOn);
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


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
	//newBullet->SetDiffVec(GetWorldPosition(), playerWorldPos);


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
	enemyDown += 1;
	isDead = true;
	enemy->position.z = -350;
	phase = Phase::None;
}

void Enemy::OnBossCollision()
{
	bossHp--;
	hpBar -= hpBarMax / 50;
	if (bossHp <= 0)
	{
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
