#pragma once

#include "Enemy.h"

class Boss
{
	Model* enemyModel_ = nullptr;
	Model* enemyModelMini_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* boss= nullptr;

	Enemy* bossMiniLUF = nullptr;//LUF left up front ¶ã‚ÌŽè‘O
	
	XMFLOAT3 playerWorldPos;

	XMFLOAT3 bossVec;
	XMFLOAT3 bossVecTemp;

	bool bossDrawFlag = true;
public:


	void Init(Model* enemyModel,Model* enemyModelMini, XMFLOAT3 position);

	void Update();

	void Draw();

	void SetPlayerWorldPos(XMFLOAT3 playerPos) { this->playerWorldPos = playerPos; }

	XMFLOAT3 GetBossVec() { return bossVec; }

	Enemy* GetEnemy() { return boss; }
};
