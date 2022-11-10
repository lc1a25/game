#include "Boss.h"

void Boss::Init(Model* enemyModel, Model* enemyModelMini, XMFLOAT3 position)
{
	enemyModel_ = enemyModel;
	enemyModelMini_ = enemyModelMini;
	boss = new Enemy();
	this->position = position;
	boss->Init(enemyModel_, this->position,{4,4,4});

	bossMiniLUF = new Enemy();
	this->position = position;
	bossMiniLUF->Init(enemyModelMini_,this->position);


	boss->phase = Phase::CircleInfinity;
	bossMiniLUF->phaseMini = BossPhase::MiniStop;
	bossMiniLUF->phase = Phase::Leave;
}

void Boss::Update()
{

	position = boss->GetPosition();

	boss->SetPlayerPosition(playerWorldPos);
	boss->SetPosition(position);
	boss->Update();

	bossMiniLUF->SetPlayerPosition(playerWorldPos);

	if (boss->phase == Phase::BossMiniVertical)
	{
		bossDrawFlag = false;
		//bossMiniLUF->phaseMini = BossPhase::MiniVerticalLUF;
	}
	if (bossDrawFlag == false)
	{
		bossMiniLUF->Update();
	}
	
}

void Boss::Draw()
{
	//if (bossDrawFlag == true)
	//{
	//	boss->Draw();
	//}
	//else if(bossDrawFlag == false)
	//{
	//	bossMiniLUF->Draw();
	//}
	boss->Draw();
}


