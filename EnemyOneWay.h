#pragma once
#include "Enemy.h"

class EnemyOneWay
{
private:
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;

public:

	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();
	//âEÇ©ÇÁÇ≠ÇÈÇ©ç∂Ç©ÇÁÇ≠ÇÈÇ©
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	Enemy* GetEnemy() { return enemy; }
};
