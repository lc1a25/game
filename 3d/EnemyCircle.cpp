#include "EnemyCircle.h"

void EnemyCircle::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue,bool attackFlag)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	this->position = position;
	enemy->Init(enemyModel_, position, enemyModel_,{2.5,2.5,2.5},attackFlag);
	PhaseInit(rightMoveTrue);
}

void EnemyCircle::Update()
{
	if (playerWorldPos.z <= enemy->GetWorldPosition().z)
	{
		enemy->SetPlayerPosition(playerWorldPos);
	}
	enemy->Update();
}

void EnemyCircle::Draw()
{
	enemy->Draw();
}

void EnemyCircle::PhaseInit(bool rightMoveTrue)
{
	if (rightMoveTrue)
	{
		enemy->phase = Phase::Approach;
	}
	else
	{
		enemy->phase = Phase::ApproachL;
	}
}
