#include "Enemy.h"
#pragma once
class BossChild
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* bossChild = nullptr;

	XMFLOAT3 bossVec;

	XMFLOAT3 playerWorldPos;

	int childNumber;


	bool bossDrawFlag = true;
public:
	void Init(Model* enemyModel,XMFLOAT3 position, int childNumber); 

	void Update();

	void Draw();

	void SetPlayerWorldPos(XMFLOAT3 playerPos) { playerWorldPos = playerPos; }

	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }


	Enemy* GetEnemy() { return bossChild; }
};

