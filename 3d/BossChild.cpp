#include "BossChild.h"

void BossChild::Init(Model* enemyModel, XMFLOAT3 position, int childNumber)
{
	enemyModel_ = enemyModel;
	bossChild = new Enemy();
	this->position = position;
	this->childNumber = childNumber;
	bossChild->Init(enemyModel_, this->position,  enemyModel_ ,{ 2,2,2 });

	bossChild->phase = Phase::BossMiniStop;

	//LF
	if (childNumber == 1 || childNumber == 5)
	{
		bossChild->SetBossChildAngle(90);
	}
	//RF
	if (childNumber == 3 || childNumber == 7)
	{
		bossChild->SetBossChildAngle(180);
	}
	//LB
	if (childNumber == 4 || childNumber == 8)
	{
		bossChild->SetBossChildAngle(270);
	}
	//RB
	if (childNumber == 2 || childNumber == 6)
	{
		bossChild->SetBossChildAngle(0);
	}
}

void BossChild::Update()
{
	if (bossDead == true)
	{
		bossChild->phase = Phase::BossMiniDead;
	}
	
	bossChild->SetChildShotRange(shotRange);
	bossChild->SetBossPos(bossPos);
	bossChild->SetBossVec(bossVec);
	bossChild->SetChildNumber(childNumber);
	bossChild->Update();
}

void BossChild::Draw()
{
	bossChild->Draw();
}


