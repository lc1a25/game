#pragma once
#include "Object3d.h"

using namespace DirectX;
class EnemyBullet
{
private:
	Model* bulletModel_ = nullptr;
	Object3d* enemyBullet = enemyBullet->Create();
	

	//��������
	static const int32_t lifeTime = 55;
	//�f�X�^�C�}�[
	int32_t deathTimer = lifeTime;
	//�f�X�t���O
	bool isDead = false;

	XMVECTOR lockOn = { 0,0,0 };

	XMVECTOR diff = { 0,0,0 };
public:
	XMVECTOR velocity_ = { 0,0,0 };

	void Init(Model* model, XMFLOAT3 enemyPosition,XMVECTOR velocity);

	void Update();

	void Draw();

	bool IsDead() const { return isDead; }

	void OnCollision();

	XMFLOAT3 GetWorldPosition();

	void SetLockOnPosition(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition);

	void SetDiffVec(XMFLOAT3 enemyPosition, XMFLOAT3 playerPosition);

};
