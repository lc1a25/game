#pragma once
#include "Enemy.h"


class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;

	//â~â^ìÆ
	float angle = 90.0f;//ç≈èâÇÃäpìx
	float radius = angle * 3.14f / 180.0f;//â~ÇÃäpìx
	float length = 0.3f;//â~ÇÃîºåa
	float addCircleX = cos(radius) * length;//â~è„ÇÃà íu x
	float addCircleY = sin(radius) * length;//â~è„ÇÃà íu y

public:

	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue, bool attackFlag);

	void Update();

	void Draw();
	//âEÇ©ÇÁÇ≠ÇÈÇ©ç∂Ç©ÇÁÇ≠ÇÈÇ©
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	Enemy* GetEnemy() { return enemy; }
};
