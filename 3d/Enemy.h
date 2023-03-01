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
	BossDead,
	None,
};

enum class BossPhase
{
	None,
	BossDead,
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
	
	int phaseNumber = static_cast<int>(phase);
	int phaseBossNumber = static_cast<int>(phase);
	static void (Enemy::*spFuncTable[])();
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//GameScene* gameScene = nullptr;
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

	//���ˊ��o
	int shotMinorTimer = 60;
	const int shotMinorInterval = 40;

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

	void PChangeBossDead();

	XMVECTOR ease_in(const XMVECTOR& start, const XMVECTOR& end, float t);
public:
	float angle = 90.0f;//�ŏ��̊p�x
	Phase phase = Phase::OutApproach;
	BossPhase phaseMini = BossPhase::None;



	//bool bossDamage = false;

	//�����蔻��p�@�e
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int shotInterval = 30;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="bulletModelXMFLOAT3">�e�̃��f��</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="attackFlag">�U�����邩</param>
	void Init(Model* enemyModel,XMFLOAT3 position, Model *bulletModelXMFLOAT3 , XMFLOAT3 scale = { 2,2,2 },bool attackFlag = true);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void Approach();//�ڋ�
	void ApproachL();
	void OutApproach();
	void LeaveL();	//���E
	void LeaveR();
	void LeaveF();
	void Stop(); //�~�܂�
	void CircleR();//�~�^���E���炭��
	void CircleL();//�����炭��
	void CircleInfinity();//����ɂ܂��
	void OneWayR();//�E���獶�ɍs��
	void OneWayL();
	void BossVertical();//boss�̃~�j���c�����ɓ���
	void BossVerticalL();
	void BossSide();//boss�̃~�j���������ɓ���
	void BossSideUp();
	void BossStop();	
	void BossDead();
	void None();

	

	/// <summary>
	/// �z�[�~���O
	/// </summary>
	void Homing();

	/// <summary>
	/// �{�X�̎���̃R�A���{�X�Ɍ������Ă���
	/// </summary>
	void ChildHoming();

	/// <summary>
	/// �O�Ɍ������Ēe��ł�
	/// </summary>
	void FrontShot();

	/// <summary>
	/// Phase �́@������
	/// </summary>
	/// <param name="rightMoveTrue">�G����ʂ̉E����łĂ��邩������o�Ă��邩</param>
	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// �v���C���[�̍��W
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	/// <summary>
	/// �G�̍��W
	/// </summary>
	/// <param name="position">�G�̍��W</param>
	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	/// <summary>
	/// �J������z���W  �G���J������z�̕����̓����ɍ��킹��
	/// </summary>
	/// <param name="z">�J������z���W</param>
	void SetCameraZ(float z) { cameraZ = z; }

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

	INT GetphaseNumber() { return phaseNumber; }

	void OnCollision();

	void OnBossCollision();

	void ShotInit();

	//���˃^�C�}�[���ւ炷
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

	void SetBossHp(int bossHp) { this->bossHp = bossHp; }

	INT GetBossHp() { return bossHp; }

};
