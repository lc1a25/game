#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//�ˑ����Ȃ��Ń|�C���^���Ă�

enum class Phase
{
	Approach,//�ڋ�
	ApproachL,
	Leave,	//���E
	Stop,  //�~�܂�
	CircleR,//�~�^���E���炭��
	CircleL,//�����炭��
};

class Enemy
{
private:
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//Player* player_ = nullptr;
	Phase phase = Phase::CircleR;

	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//���˃^�C�}�[
	int32_t shotTimer = 0;

	//�~�^��
	float angle = 90.0f;//�ŏ��̊p�x
	float radius = angle * 3.14f / 180.0f ;//�~�̊p�x
	float length = 0.3f;//�~�̔��a
	float addCircleX = cos(radius) * length;//�~��̈ʒu x
	float addCircleY = sin(radius) * length;//�~��̈ʒu y
public:
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int shotInterval = 60;

	void Init(Model* enemyModel,XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	XMFLOAT3 GetWorldPosition();

	void OnCollision();
};
