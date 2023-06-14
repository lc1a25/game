#include "AllCollision.h"

void AllCollision::CheckCollision(Enemy* enemy,Player*player)
{
	if (mutekiFlag == true)
	{
		mutekiCoolTime--;
		if (mutekiCoolTime <= mutekiCoolTimeMin)
		{
			mutekiFlag = false;
			mutekiCoolTime = mutekiCoolTimeMax;
		}
	}
	XMFLOAT3 pos1, pos2;
	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//�G�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

	pos1 = player->GetWorldPosition();
	//���L�����ƓG�e�����蔻��
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		pos2 = bullet->GetWorldPosition();
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
			((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
			((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (length <= size)
		{
			player->OnCollision();
			//return true;
			mutekiFlag = true;
		}
		
	

	bullet->OnCollision();
		
	}
}
