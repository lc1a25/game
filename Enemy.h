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
	BossVertical,//boss�̃~�j���c�����ɓ���
	BossVerticalL,
	BossSide,//boss�̃~�j���������ɓ���
	BossSideUp,
	BossStop,
	None,
};

enum class BossPhase
{
	None,
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

	XMFLOAT3 bossPos;
	XMFLOAT3 childShotRange;

	//���˃^�C�}�[
	int32_t shotTimer = 0;
	
	//�~�^��

	float radius = angle * 3.14f / 180.0f;//�~�̊p�x
	float length = 0.3f;//�~�̔��a
	float addCircleX = cos(radius) * length;//�~��̈ʒu x
	float addCircleY = sin(radius) * length;//�~��̈ʒu y
	float addCircleZ = sin(radius) * length;//�~��̈ʒu z
	float angleVec = 1.0f;


	bool isDead = false;

	int bossHp = 50;
	float hpBar = 0;
	float hpBarMax = 0;

	//���̌`�̓����p
	bool isL = false;

	int childNumber;
	XMFLOAT3 bossVec;
	float waitTimer = 120;

	const float waitTimerInterval = 120;

	int shotMinorTimer = 60;
	const int shotMinorInterval = 60;

	bool circleZFlag = false;

	float cameraZ = 0.0f;
	XMFLOAT3 cameraVec;

	bool attackFlag = true;

	//�G�����񂾐� (�`���[�g���A���Ɏg��)
	int enemyDown = 0;


	//P���ŏ��ɂ��Ă���̂�Phase�p
	void PCircleR();

	void PCircleL();

	void PShot();

	void PShotMinor();

	void PHoming();

	void PLeaveL();

	void PLeaveR();

	void PLeaveF();

	void PCircleZ();

	void PCircleZInverce();

	void PChild();

	void PWait();

	void PWaitLB();

	void PWaitR();

	void PWaitRB();

	void PChildHoming();

	XMVECTOR ease_in(const XMVECTOR& start, const XMVECTOR& end, float t);
public:
	float angle = 90.0f;//�ŏ��̊p�x
	Phase phase = Phase::OutApproach;
	BossPhase phaseMini = BossPhase::None;

	bool bossDamage = false;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int shotInterval = 30;

	void Init(Model* enemyModel,XMFLOAT3 position, Model *bulletModelXMFLOAT3 , XMFLOAT3 scale = { 2,2,2 },bool attackFlag = true);

	void Update();

	void Draw();

	void Homing();

	void ChildHoming();

	void FrontShot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	void SetCameraZ(float z) { cameraZ = z; }

	void SetGameScene(GameScene* gameScene) { this->gameScene = gameScene; }

	void SetChildNumber(int childNumber) { this->childNumber = childNumber; }

	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	void SetChildShotRange(XMFLOAT3 childShotRange) { this->childShotRange = childShotRange; }

	//�X�v���C�ghp�o�[�p
	void SetBossHpBar(float bossHpBar,float bossHpBarMax) {
		hpBar = bossHpBar;
		hpBarMax = bossHpBarMax;
	}
	FLOAT GetHpBarX() { return hpBar; }

	void EasingTime();

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	INT GetEnemyDownCount() { return enemyDown; }

	void OnCollision();

	void OnBossCollision();

	void ShotInit();

	//���˃^�C�}�[���ւ炷
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
