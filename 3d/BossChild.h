#include "Enemy.h"
#pragma once
class BossChild
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* bossChild = nullptr;

	XMFLOAT3 bossVec;

	XMFLOAT3 bossPos;

	XMFLOAT3 shotRange;

	int childNumber;


	bool bossDrawFlag = true;

	bool bossDead = false;
public:
	void Init(Model* enemyModel,XMFLOAT3 position, int childNumber); 

	void Update();

	void Draw();

	void SetChildShotRange(XMFLOAT3 shotRange) { this->shotRange = shotRange; }

	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	void SetBossDead(bool bossDead) { this->bossDead = bossDead; }


	Enemy* GetEnemy() { return bossChild; }
};

