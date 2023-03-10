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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">bossの周りのモデル</param>
	/// <param name="position">座標</param>
	/// <param name="childNumber">周りの番号</param>
	void Init(Model* enemyModel,XMFLOAT3 position, int childNumber); 

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾の発射間隔
	/// </summary>
	/// <param name="shotRange"></param>
	void SetChildShotRange(XMFLOAT3 shotRange) { this->shotRange = shotRange; }

	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	void SetBossDead(bool bossDead) { this->bossDead = bossDead; }


	Enemy* GetEnemy() { return bossChild; }
};

