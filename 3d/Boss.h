#pragma once

#include "Enemy.h"

class Boss
{
	//モデル
	Model* enemyModel_ = nullptr;
	Model* enemyModelMini_ = nullptr;
	Model* barrierModel_ = nullptr;

	//座標
	XMFLOAT3 position = { 0,0,0 };
	Object3d* barrier = barrier->Create();

	//ボス
	Enemy* boss= nullptr;
	//ボスの周りにいるやつ
	Enemy* bossMiniLUF = nullptr;//LUF left up front 左上の手前
	
	
	//プレイヤーの座標(ホーミング用)
	XMFLOAT3 playerWorldPos;

	//ボスの移動量
	XMFLOAT3 bossVec;
	XMFLOAT3 bossVecTemp;

	//描画フラグ
	bool bossDrawFlag = true;
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">ボスのモデル</param>
	/// <param name="enemyModelMini">ボスの周りにいるやつのモデル</param>
	/// <param name="position">座標</param>
	void Init(Model* enemyModel,Model* enemyModelMini, Model* barrierModel,XMFLOAT3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの座標(ホーミング用)
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void SetPlayerWorldPos(XMFLOAT3 playerPos) { this->playerWorldPos = playerPos; }

	void SetBarrierFlag(bool barrierFlag) { boss->SetBarrierFlag(barrierFlag); }

	/// <summary>
	/// ボスの移動量
	/// </summary>
	/// <returns>ボスの移動量</returns>
	XMFLOAT3 GetBossVec() { return bossVec; }


	Enemy* GetEnemy() { return boss; }

	/// <summary>
	/// ボスの場所
	/// </summary>
	/// <returns>ボスの場所</returns>
	XMFLOAT3 GetPos() { return boss->GetWorldPosition(); }

	/// <summary>
	/// ボスの生死
	/// </summary>
	/// <returns>ボスの生死</returns>
	BOOL GetBossDead() { return boss->IsDead(); }

	BOOL GetBarrierFlag() { return boss->GetBarrierFlag(); }
	BOOL GetBarrierPhaseFlag() { return boss->GetBarrierPhaseFlag(); }
	//ボスがムービー中か
	BOOL GetBossMovieFlag() { return boss->GetBossMovieFlag(); }
};
