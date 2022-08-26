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
	//	//ŠÖ”‚É‚·‚é
	//	enemy->position.x -= 0.5;
	//	if (enemy->position.x <= 1280 / 4 * 3)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	//case PhaseCircle::ApproachL:
	//	//ŠÖ”‚É‚·‚é
	//	enemy->position.x += 0.5;
	//	if (enemy->position.x <= 1280 / 4)
	//	{
	//		phase = PhaseCircle::CircleR;
	//	}

	//	break;
	case PhaseCircle::Leave:
		//ŠÖ”‚É‚·‚é
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
	case PhaseCircle::CircleR://‰E‚©‚ç—ˆ‚½ver 90‚Ç
		//‰~‚ÌŠp“x
		radius = angle * 3.14f / 180.0f;

		//‰~ó‚ÌêŠ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//“G‚ÌÀ•W‚É‘«‚·
		position.x += addCircleX;
		position.y += addCircleY;

		//Šp“x‚ğ‚½‚µ‚Ä‰~ó‚É“®‚©‚·
		angle += 0.5f;
		if (angle >= 460.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}

		//’e‚ğŒ‚‚Â
		enemy->ShotTimerLower();
		if (enemy->IsTimer())
		{
			enemy->Shot();
			enemy->ShotInit();
		}

		break;
	case PhaseCircle::CircleL://¶‚©‚ç—ˆ‚½ver 270‚Ç
	//‰~‚ÌŠp“x
		radius = angle * 3.14f / 180.0f;

		//‰~ó‚ÌêŠ
		addCircleX = cos(radius) * length;
		addCircleY = sin(radius) * length;

		//“G‚ÌÀ•W‚É‘«‚·
		position.x += addCircleX;
		position.y += addCircleY;

		//Šp“x‚ğ‚½‚µ‚Ä‰~ó‚É“®‚©‚·
		angle -= 0.5f;
		if (angle <= -270.0f)
		{
			enemy->ShotInit();
			position.z -= 0.5;
		}
		//’e‚ğŒ‚‚Â
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
