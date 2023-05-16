#include "Boss.h"

void Boss::Init(Model* enemyModel, Model* enemyModelMini, Model* barrierModel,XMFLOAT3 position)
{
	enemyModel_ = enemyModel;
	enemyModelMini_ = enemyModelMini;
	barrierModel_ = barrierModel;
	boss = new Enemy();
	this->position = position;
	boss->Init(enemyModel_, this->position, enemyModelMini_,{4,4,4});

	barrier->SetModel(barrierModel);
	barrier->scale = { 1.8,1.8,1.8 };
	barrier->SetPosition({ 0,0,20 });
	boss->phase = Phase::BossApproach;

	bossVec = this->position;
	bossVecTemp = this->position;
	
}

void Boss::Update()
{
	//ボスのバリア
	if (boss->GetBarrierFlag() == true)
	{
		barrier->SetPosition({ GetPos().x,GetPos().y - 10 , GetPos().z - 5 });
		if (barrier->scale.x <= 8)
		{
			barrier->scale.x++;
			barrier->scale.y++;
			barrier->scale.z++;
		}
		else
		{
			barrier->scale = { 9,9,9 };
		}

	}
	else if (boss->GetBarrierFlag() == false)
	{
		barrier->scale = { 0,0,0 };
	}

	position = boss->GetPosition();
	bossVec.x = boss->GetPosition().x - bossVecTemp.x;
	bossVec.y = boss->GetPosition().y - bossVecTemp.y;
	bossVec.z = boss->GetPosition().z - bossVecTemp.z;

	bossVecTemp.x = boss->GetPosition().x;
	bossVecTemp.y = boss->GetPosition().y;
	bossVecTemp.z = boss->GetPosition().z;

	boss->SetPlayerPosition(playerWorldPos);
	boss->SetPosition(position);

	boss->Update();
	barrier->Update();
}

void Boss::Draw()
{	
	boss->Draw();
	barrier->Draw();

}


