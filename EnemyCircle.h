#pragma once
#include "Enemy.h"

class EnemyCircle : public Enemy 
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 setPosition = { 60,0,100 };
	Object3d* enemy = enemy->Create();
public:
	void Init();

	void Update();

	void Draw();
};
