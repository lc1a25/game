#include "EnemyOneWay.h"

void EnemyOneWay::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	this->position = position;
	enemy->Init(enemyModel_, position, enemyModel_);
	PhaseInit(rightMoveTrue);
}

void EnemyOneWay::Update()
{
	enemy->Update();
}

void EnemyOneWay::Draw()
{
	enemy->Draw();
}

void EnemyOneWay::PhaseInit(bool rightMoveTrue)
{
	if (rightMoveTrue)
	{
		enemy->phase = Phase::OneWayR;
	}
	else
	{
		enemy->phase = Phase::OneWayL;
	}
}
