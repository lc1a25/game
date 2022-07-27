#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Enemy.h"

#include <memory>//ユニークポインタ
#include <list>


class Player
{
private:

	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;

	Enemy* enemy = nullptr;

	Object3d* player = player->Create();
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Input* input = nullptr;

	const float playerVelocity = 1.2f;

	XMFLOAT3 enemyWorldPos;

	int32_t coolTimer = 5;
	int32_t bulletCoolTimer = 5;

	bool rockOn = false;

public:
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	void Init(Model* model, Model* bulletModel);
	void Attack();
	XMVECTOR transform(const XMVECTOR& v, const DirectX::XMMATRIX& m);
	void Update();

	void Draw();

	XMFLOAT3 GetWorldPosition();

	void SetEnemyPosition(XMFLOAT3 position) { enemyWorldPos = position; }

	void OnCollision();


	
};
