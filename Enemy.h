#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

class Player;//依存しないでポインタもてる

enum class Phase
{
	Approach,//接近
	Leave,	//離脱
	Stop,  //止まる
};

class Enemy
{
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	Player* player_ = nullptr;
	Phase phase = Phase::Stop;

	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//発射タイマー
	int32_t shotTimer = 0;
public:
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
	static const int shotInterval = 60;

	void Init(Model* enemyModel);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit();

	void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	XMFLOAT3 GetWorldPosition();

	void OnCollision();
};
