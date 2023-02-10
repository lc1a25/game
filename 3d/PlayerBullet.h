#pragma once
#include "Object3d.h"

using namespace DirectX;
class PlayerBullet
{
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
	
	void Init(Model* model, XMFLOAT3 playerPosition, XMVECTOR velocity);

	void Update();

	void Draw();

	void OnCollision();

	bool IsDead() const { return isDead; }

	XMFLOAT3 GetWorldPosition();
	
	void SetLockOnPosition(XMFLOAT3 enemyPosition, bool isDead);

	void SetEnemyDead(bool isDead) { isDead = enemyIsDead; }
};
