#pragma once
#include "Enemy.h"


class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;

	//‰~‰^“®
	float angle = 90.0f;//Å‰‚ÌŠp“x
	float radius = angle * 3.14f / 180.0f;//‰~‚ÌŠp“x
	float length = 0.3f;//‰~‚Ì”¼Œa
	float addCircleX = cos(radius) * length;//‰~ã‚ÌˆÊ’u x
	float addCircleY = sin(radius) * length;//‰~ã‚ÌˆÊ’u y

public:

	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue, bool attackFlag);

	void Update();

	void Draw();
	//‰E‚©‚ç‚­‚é‚©¶‚©‚ç‚­‚é‚©
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	Enemy* GetEnemy() { return enemy; }

	BOOL GetIsDead() { return enemy->IsDead(); }
};
