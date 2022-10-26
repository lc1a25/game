#include "Boss.h"

void Boss::Init(Model* enemyModel, XMFLOAT3 position)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	this->position = position;
	enemy->Init(enemyModel_, position);
	enemy->phase = Phase::CircleInfinity;
}

void Boss::Update()
{
	position = enemy->GetPosition();

	enemy->SetPlayerPosition(playerWorldPos);
	enemy->SetPosition(position);
	enemy->Update();
}

void Boss::Draw()
{
	enemy->Draw();
}


