#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//�ˑ����Ȃ��Ń|�C���^���Ă�
class GameScene;

enum class Phase
{
	Approach,//�ڋ�
	ApproachL,
	OutApproach,
	LeaveL,	//���E
	LeaveR,
	LeaveF,
	Stop,  //�~�܂�
	CircleR,//�~�^���E���炭��
	CircleL,//�����炭��
	CircleInfinity,//����ɂ܂��
	OneWayR,//�E���獶�ɍs��
	OneWayL,//������E�ɍs��
	BossMiniVertical,//boss�̃~�j���c�����ɓ���
	BossMiniSide,//boss�̃~�j���������ɓ���
	BossStop,
};

enum class BossPhase
{
	MiniStop,
	MiniVerticalLUF,
	MiniVerticalLUB,
	MiniVerticalRUF,
	MiniVerticalRUB,
	MiniVerticalLDF,
	MiniVerticalLDB,
	MiniVerticalRDF,
	MiniVerticalRDB,
	MiniSideLUF,
	MiniSideLUB,
	MiniSideRUF,
	MiniSideRUB,
	MiniSideLDF,
	MiniSideLDB,
	MiniSideRDF,
	MiniSideRDB,


};

class Enemy
{
private:
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	GameScene* gameScene = nullptr;
	//Player* player_ = nullptr;
	
	//OneWay�̓G�̃X�s�[�h
	float OWRSpeed = 0.5f;
	float OWLSpeed = 0.5f;
	XMVECTOR OneWayPos;
	XMVECTOR MiniPosLUF;

	XMVECTOR LUFPosVertical = { -40, MiniPosLUF.m128_f32[1], MiniPosLUF.m128_f32[2] };
	
	int time = 0;
	int leaveTimeInit = 300;
	int leaveTime = leaveTimeInit;
	

	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	float maxTime = 10.0f;
	float timeRate = 0.0f;

	//��Phase�Ɉڍs����܂łɈړ����Ă���X�s�[�h
	float ApproachSpeed = 0.5f;

	bool shotEndFlag = false;
	float outApproachSpeed = 1.0f;

	//���@�̍��W(�z�[�~���O�p)
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
	float angleVec = 1.5f;


	bool isDead = false;

	int bossHp = 50;

	//���̌`�̓����p
	bool isL = false;

	float cameraZ = 0.0f;
	XMFLOAT3 cameraVec;


	//P���ŏ��ɂ��Ă���̂�Phase�p
	void PCircleR();

	void PCircleL();

	void PShot();

	void PHoming();

	void PLeaveL();

	void PLeaveR();

	void PLeaveF();

	XMVECTOR ease_in(const XMVECTOR& start, const XMVECTOR& end, float t);
public:

	Phase phase = Phase::OutApproach;
	BossPhase phaseMini = BossPhase::MiniStop;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int shotInterval = 30;

	void Init(Model* enemyModel,XMFLOAT3 position,XMFLOAT3 scale = {2,2,2});

	void Update();

	void Draw();

	void Homing();

	void FrontShot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	void SetCameraZ(float z) { cameraZ = z; }

	void SetGameScene(GameScene* gameScene) { this->gameScene = gameScene; }

	void EasingTime();

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	void OnCollision();

	void OnBossCollision();

	void ShotInit();

	//���˃^�C�}�[���ւ炷
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
