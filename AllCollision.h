#pragma once
#include "Enemy.h"
#include "Player.h"


class AllCollision
{
	Player* player = nullptr;
	bool mutekiFlag = false;
	int mutekiCoolTimeMax = 420;
	int mutekiCoolTimeMin = 0;
	int mutekiCoolTime = mutekiCoolTimeMax;
	//“–‚½‚è”»’è—p•Ï”
	float length = 0.0f;
	float size = 25.0f;
public:
	void CheckCollision(Enemy *enemy, Player* player);
};

