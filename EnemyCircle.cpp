#include "EnemyCircle.h"

void EnemyCircle::Init(Model* enemyModel, XMFLOAT3 position, bool rightMoveTrue)
{
	enemyModel_ = enemyModel;
	enemy = new Enemy();
	this->position = position;
	enemy->Init(enemyModel_, position, rightMoveTrue);

}

void EnemyCircle::Update()
{
	position = enemy->GetPosition();

	switch (phase)
	{
	//case PhaseCircle::Approach:
	//	//�֐��ɂ���
	//	enemy->position.x -= 0.5;
	//	if (enemy->position.x <= 1280 / 4 * 3)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	//case PhaseCircle::ApproachL:
	//	//�֐��ɂ���
	//	enemy->position.x += 0.5;
	//	if (enemy->position.x <= 1280 / 4)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	case PhaseCircle::Leave:
		//�֐��ɂ���
		position.z -= 0.5;
		break;
	case PhaseCircle::Stop:
		enemy->ShotTimerLower();
		if (enemy->IsTimer())
		{
			enemy->Shot();
			enemy->ShotInit();
		}
		break;
	case PhaseCircle::CircleR://�E���痈��ver 90��
		//�~�̊p�x
		radius = angle * 3.14f / 180.0f;

		//�~��̏ꏊ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//�G�̍��W�ɑ���
		position.x += addCircleX;
		position.y += addCircleY;

		//�p�x�������ĉ~��ɓ�����
		angle += 0.5f;
		if (angle >= 460.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}

		//�e������
		enemy->ShotTimerLower();
		if (enemy->IsTimer())
		{
			enemy->Shot();
			enemy->ShotInit();
		}

		break;
	case PhaseCircle::CircleL://�����痈��ver 270��
	//�~�̊p�x
		radius = angle * 3.14f / 180.0f;

		//�~��̏ꏊ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//�G�̍��W�ɑ���
		position.x += addCircleX;
		position.y += addCircleY;

		//�p�x�������ĉ~��ɓ�����
		angle -= 0.5f;
		if (angle <= -270.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}
		//�e������
		enemy->ShotTimerLower();
		if (enemy->IsTimer())
		{
			enemy->Shot();
			enemy->ShotInit();
		}
		break;

	default:
		break;
	}
	enemy->SetPlayerPosition(playerWorldPos);
	enemy->SetPosition(position);
	enemy->Update();
}

void EnemyCircle::Draw()
{
	enemy->Draw();
}

void EnemyCircle::PhaseInit(bool rightMoveTrue)
{
	if (!rightMoveTrue)
	{
		phase = PhaseCircle::CircleR;
	}
}
