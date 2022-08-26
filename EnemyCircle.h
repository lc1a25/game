#pragma once
#include "Enemy.h"

enum class PhaseCircle
{
	Approach,//接近
	ApproachL,
	Leave,	//離脱
	Stop,  //止まる
	CircleR,//円運動右からくる
	CircleL,//左からくる
};

class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };
	//Object3d* enemy = enemy->Create();
	Enemy* enemy = nullptr;
	PhaseCircle phase = PhaseCircle::CircleL;

	XMFLOAT3 playerWorldPos;

	//円運動
	float angle = 90.0f;//最初の角度
	float radius = angle * 3.14f / 180.0f;//円の角度
	float length = 0.3f;//円の半径
	float addCircleX = cos(radius) * length;//円上の位置 x
	float addCircleY = sin(radius) * length;//円上の位置 y
public:
	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	
};
