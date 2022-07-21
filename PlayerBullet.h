#pragma once
#include "Object3d.h"

using namespace DirectX;
class PlayerBullet
{
private:
	Model* model_ = nullptr;
	Object3d* bullet = bullet->Create();
	XMFLOAT3 velocity_ = { 0,0,0 };

	//��������
	static const int32_t lifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer = lifeTime;
	//�f�X�t���O
	bool isDead = false;
public:
	
	void Init(Model* model, XMFLOAT3 playerPosition,XMFLOAT3 velocity);

	void Update();

	void Draw();

	void OnCollision();

	bool IsDead() const { return isDead; }

	XMFLOAT3 GetWorldPosition();
};
