#pragma once
#include "Enemy.h"


class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* enemy = nullptr;

	XMFLOAT3 playerWorldPos;

	//�~�^��
	float angle = 90.0f;//�ŏ��̊p�x
	float radius = angle * 3.14f / 180.0f;//�~�̊p�x
	float length = 0.3f;//�~�̔��a
	float addCircleX = cos(radius) * length;//�~��̈ʒu x
	float addCircleY = sin(radius) * length;//�~��̈ʒu y

public:

	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue, bool attackFlag);

	void Update();

	void Draw();
	//�E���炭�邩�����炭�邩
	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	Enemy* GetEnemy() { return enemy; }
};
