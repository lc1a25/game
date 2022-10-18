#include "EnemyCircle.h"

void EnemyCircle::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	this->position = position;
	enemy->Init(enemyModel_, position, rightMoveTrue);
	PhaseInit(rightMoveTrue);
}

void EnemyCircle::Update()
{
	position = enemy->GetPosition();

	enemy->SetPlayerPosition(playerWorldPos);
	enemy->SetPosition(position);
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
