#pragma once
#include "Enemy.h"

class EnemyOneWay
{
private:
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;

	bool rightMoveTrue;

public:

	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue,bool attackFlag = true, bool movieFlag = false);

	void Update();

	void Draw();
	void MoviePhase(bool movieFlag);
	//‰E‚©‚ç‚­‚é‚©¶‚©‚ç‚­‚é‚©
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }
	
	BOOL GetIsDead() { return enemy->IsDead(); }

	Enemy* GetEnemy() { return enemy; }
};
