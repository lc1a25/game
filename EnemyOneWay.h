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
	//�E���炭�邩�����炭�邩
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	Enemy* GetEnemy() { return enemy; }
};
