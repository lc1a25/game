#include "Boss.h"

void Boss::Init(Model* enemyModel, Model* enemyModelMini, XMFLOAT3 position)
{
	enemyModel_ = enemyModel;
	enemyModelMini_ = enemyModelMini;
	boss = new Enemy();
	this->position = position;
	boss->Init(enemyModel_, this->position, enemyModelMini_,{4,4,4});

	boss->phase = Phase::CircleInfinity;

	bossVec = this->position;
	bossVecTemp = this->position;
	
}

void Boss::Update()
{

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
	
}

void Boss::Draw()
{
	boss->Draw();
}


