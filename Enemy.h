#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//依存しないでポインタもてる

enum class Phase
{
	Approach,//接近
	ApproachL,
	Leave,	//離脱
	Stop,  //止まる
	CircleR,//円運動右からくる
	CircleL,//左からくる
	OneWayR,//右から左に行く
	OneWayL,//左から右に行く
};

class Enemy
{
private:
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//Player* player_ = nullptr;


	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//発射タイマー
	int32_t shotTimer = 0;

	//円運動
	float angle = 90.0f;//最初の角度
	float radius = angle * 3.14f / 180.0f ;//円の角度
	float length = 0.3f;//円の半径
	float addCircleX = cos(radius) * length;//円上の位置 x
	float addCircleY = sin(radius) * length;//円上の位置 y

	bool isDead = false;
public:


	Phase phase;
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
	static const int shotInterval = 60;

	void Init(Model* enemyModel,XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	void OnCollision();

	void ShotInit();

	//発射タイマーをへらす
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
