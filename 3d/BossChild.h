#include "Enemy.h"
#pragma once
class BossChild //ボスの周りをまわる敵
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* bossChild = nullptr;

	XMFLOAT3 bossVec;//ボスの移動量

	XMFLOAT3 bossPos;//ボスの座標

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

	/// <summary>
	/// ボスの座標をセット
	/// </summary>
	/// <param name="bossPos">ボスの座標</param>
	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	/// <summary>
	/// ボスの移動量をセット
	/// </summary>
	/// <param name="bossVec">ボスの移動量</param>
	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	/// <summary>
	/// ボスの生死をセット
	/// </summary>
	/// <param name="bossDead">ボスの生死</param>
	void SetBossDead(bool bossDead) { this->bossDead = bossDead; }

	/// <summary>
	/// ボスがバリアを張ったかセット
	/// </summary>
	/// <param name="barrierPhaseFlag">ボスがバリアを張ったか</param>
	void SetBarrierPhaseFlag(bool barrierPhaseFlag) { bossChild->SetBarrierPhaseFlag(barrierPhaseFlag); }

	/// <summary>
	/// ボスの周りをまわる(自分)が死んだか
	/// </summary>
	/// <returns>ボスの周りをまわる(自分)が死んでるか</returns>
	bool GetMiniDead() { return bossChild->IsDead(); }

	Enemy* GetEnemy() { return bossChild; }
};

