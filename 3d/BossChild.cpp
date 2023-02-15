#include "BossChild.h"

void BossChild::Init(Model* enemyModel, XMFLOAT3 position, int childNumber)
{
	enemyModel_ = enemyModel;
	bossChild = new Enemy();
	this->position = position;
	this->childNumber = childNumber;
	bossChild->Init(enemyModel_, this->position,  enemyModel_ ,{ 2,2,2 });


	bossChild->phase = Phase::None;
	bossChild->phaseMini = BossPhase::MiniStop;
}

void BossChild::Update()
{
	if (bossDead == true)
	{
		bossChild->phaseMini = BossPhase::BossDead;
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


