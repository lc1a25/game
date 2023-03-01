#pragma once
#include "Object3d.h"

class PlayerBullet
{
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	Model* model_ = nullptr;
	Object3d* bullet = bullet->Create();
	XMVECTOR velocity_ = { 0,0,0 };

	//生存時間
	static const int32_t lifeTime = 60 * 1.5;
	//デスタイマー
	int32_t deathTimer = lifeTime;
	//デスフラグ
	bool isDead = false;

	bool enemyIsDead = false;
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="playerPosition">座標</param>
	/// <param name="velocity">発射速度</param>
	void Init(Model* model, XMFLOAT3 playerPosition, XMVECTOR velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たった判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// プレイヤーの生死
	/// </summary>
	/// <returns>プレイヤーの生死</returns>
	bool IsDead() const { return isDead; }

	/// <summary>
	/// プレイヤーの座標
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	XMFLOAT3 GetWorldPosition();
	
	/// <summary>
	/// ロックオン 多分使わない
	/// </summary>
	/// <param name="enemyPosition">敵の座標</param>
	/// <param name="isDead">敵が死んでいるか</param>
	void SetLockOnPosition(XMFLOAT3 enemyPosition, bool isDead);

	/// <summary>
	/// 敵の生死
	/// </summary>
	/// <param name="isDead">敵の生死</param>
	void SetEnemyDead(bool isDead) { isDead = enemyIsDead; }
};
