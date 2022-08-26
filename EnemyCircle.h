#pragma once
#include "Enemy.h"

enum class PhaseCircle
{
	Approach,//�ڋ�
	ApproachL,
	Leave,	//���E
	Stop,  //�~�܂�
	CircleR,//�~�^���E���炭��
	CircleL,//�����炭��
};

class EnemyCircle
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };
	//Object3d* enemy = enemy->Create();
	Enemy* enemy = nullptr;
	PhaseCircle phase = PhaseCircle::CircleL;

	XMFLOAT3 playerWorldPos;

	//�~�^��
	float angle = 90.0f;//�ŏ��̊p�x
	float radius = angle * 3.14f / 180.0f;//�~�̊p�x
	float length = 0.3f;//�~�̔��a
	float addCircleX = cos(radius) * length;//�~��̈ʒu x
	float addCircleY = sin(radius) * length;//�~��̈ʒu y
public:
	void Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void PhaseInit(bool rightMoveTrue);

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	
};
