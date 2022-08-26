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
	//	//関数にする
	//	enemy->position.x -= 0.5;
	//	if (enemy->position.x <= 1280 / 4 * 3)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	//case PhaseCircle::ApproachL:
	//	//関数にする
	//	enemy->position.x += 0.5;
	//	if (enemy->position.x <= 1280 / 4)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	case PhaseCircle::Leave:
		//関数にする
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
	case PhaseCircle::CircleR://右から来たver 90ど
		//円の角度
		radius = angle * 3.14f / 180.0f;

		//円状の場所
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//敵の座標に足す
		position.x += addCircleX;
		position.y += addCircleY;

		//角度をたして円状に動かす
		angle += 0.5f;
		if (angle >= 460.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}

		//弾を撃つ
		enemy->ShotTimerLower();
		if (enemy->IsTimer())
		{
			enemy->Shot();
			enemy->ShotInit();
		}

		break;
	case PhaseCircle::CircleL://左から来たver 270ど
	//円の角度
		radius = angle * 3.14f / 180.0f;

		//円状の場所
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//敵の座標に足す
		position.x += addCircleX;
		position.y += addCircleY;

		//角度をたして円状に動かす
		angle -= 0.5f;
		if (angle <= -270.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}
		//弾を撃つ
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
