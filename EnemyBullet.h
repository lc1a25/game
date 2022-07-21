#pragma once
#include "Object3d.h"

using namespace DirectX;
class EnemyBullet
{
private:
	Model* bulletModel_ = nullptr;
	Object3d* enemyBullet = enemyBullet->Create();
	XMVECTOR velocity_ = { 0,0,0 };

	//生存時間
	static const int32_t lifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer = lifeTime;
	//デスフラグ
	bool isDead = false;
public:
	void Init(Model* model, XMFLOAT3 enemyPosition,XMVECTOR velocity);

	void Update();

	void Draw();

	bool IsDead() const { return isDead; }

	void OnCollision();

	XMFLOAT3 GetWorldPosition();

};
