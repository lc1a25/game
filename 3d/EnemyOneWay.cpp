#include "EnemyOneWay.h"

void EnemyOneWay::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue, bool attackFlag, bool movieFlag)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	XMFLOAT3 scale{ 2.0f,2.0f,2.0f };
	this->position = position;
	this->rightMoveTrue = rightMoveTrue;
	enemy->Init(enemyModel_, position, enemyModel_,scale,attackFlag);
	if (movieFlag)
	{
		MoviePhase(this->rightMoveTrue);
	}
	else
	{
		PhaseInit(this->rightMoveTrue);
	}

	
}

void EnemyOneWay::Update()
{
	if (playerWorldPos.z <= enemy->GetWorldPosition().z)
	{
		enemy->SetPlayerPosition(playerWorldPos);
	}
	
	enemy->Update();
}

void EnemyOneWay::Draw()
{
	enemy->Draw();
}

void EnemyOneWay::MoviePhase(bool movieFlag)
{
	if (rightMoveTrue)
	{
		enemy->phase = Phase::OneWayMovieR;
	}
	else
	{
		enemy->phase = Phase::OneWayMovieL;
	}
	
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
