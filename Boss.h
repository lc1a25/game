#pragma once

#include "Enemy.h"

class Boss
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;
public:
	void Init(Model* enemyModel, XMFLOAT3 position);

	void Update();

	void Draw();
};
