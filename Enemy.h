#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

class Player;//�ˑ����Ȃ��Ń|�C���^���Ă�

enum class Phase
{
	Approach,//�ڋ�
	Leave,	//���E
	Stop,  //�~�܂�
};

class Enemy
{
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	Player* player_ = nullptr;
	Phase phase = Phase::Stop;

	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//���˃^�C�}�[
	int32_t shotTimer = 0;
public:
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int shotInterval = 60;

	void Init(Model* enemyModel);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit();

	void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	XMFLOAT3 GetWorldPosition();

	void OnCollision();
};
