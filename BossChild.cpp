#include "BossChild.h"

void BossChild::Init(Model* enemyModel, XMFLOAT3 position, int childNumber)
{
	enemyModel_ = enemyModel;
	bossChild = new Enemy();
	this->position = position;
	this->childNumber = childNumber;
	bossChild->Init(enemyModel_, this->position, { 2,2,2 });


	bossChild->phase = Phase::None;
	bossChild->phaseMini = BossPhase::MiniStop;
}

void BossChild::Update()
{
	
	bossChild->SetBossVec(bossVec);
	bossChild->SetChildNumber(childNumber);
	bossChild->Update();
}

void BossChild::Draw()
{
	bossChild->Draw();
}


