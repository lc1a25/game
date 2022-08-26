#pragma once
#include "Enemy.h"

enum class PhaseCircle
{
	Approach,//ê⁄ãﬂ
	ApproachL,
	Leave,	//ó£íE
	Stop,  //é~Ç‹ÇÈ
	CircleR,//â~â^ìÆâEÇ©ÇÁÇ≠ÇÈ
	CircleL,//ç∂Ç©ÇÁÇ≠ÇÈ
};

class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };
	//Object3d* enemy = enemy->Create();
	Enemy* enemy = nullptr;
	PhaseCircle phase = PhaseCircle::CircleL;

	XMFLOAT3 playerWorldPos;

	//â~â^ìÆ
	float angle = 90.0f;//ç≈èâÇÃäpìx
	float radius = angle * 3.14f / 180.0f;//â~ÇÃäpìx
	float length = 0.3f;//â~ÇÃîºåa
	float addCircleX = cos(radius) * length;//â~è„ÇÃà íu x
	float addCircleY = sin(radius) * length;//â~è„ÇÃà íu y
public:
	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	
};
