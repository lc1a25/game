#pragma once
#include "Object3d.h"

using namespace DirectX;
class EnemyBullet
{
private:
	Model* bulletModel_ = nullptr;
	Object3d* enemyBullet = enemyBullet->Create();
	

	//生存時間
	static const int32_t lifeTime = 60 * 2;
	//デスタイマー
	int32_t deathTimer = lifeTime;
	//デスフラグ
	bool isDead = false;

	XMVECTOR lockOn = { 0,0,0 };

	XMVECTOR diff = { 0,0,0 };
public:
	XMVECTOR velocity_ = { 0,0,0 };

	void Init(Model* model, XMFLOAT3 enemyPosition,XMVECTOR velocity,XMFLOAT3 scale = { 0.5,0.5,0.5 });

	void Update();

	void Draw();

	bool IsDead() const { return isDead; }

	void OnCollision();

	XMFLOAT3 GetWorldPosition();

	void SetLockOnPosition(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition);

	void SetDiffVec(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition);

};
