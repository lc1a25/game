#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//依存しないでポインタもてる
class GameScene;

enum class Phase
{
	Approach,//接近
	ApproachL,
	OutApproach,
	LeaveL,	//離脱
	LeaveR,
	LeaveF,
	Stop,  //止まる
	CircleR,//円運動右からくる
	CircleL,//左からくる
	CircleInfinity,//∞上にまわる
	OneWayR,//右から左に行く
	OneWayL,//左から右に行く
	BossVertical,//bossのミニが縦方向に動く
	BossVerticalL,
	BossSide,//bossのミニが横方向に動く
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
	
	//OneWayの敵のスピード
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

	//別Phaseに移行するまでに移動しているスピード
	float ApproachSpeed = 0.5f;

	bool shotEndFlag = false;
	float outApproachSpeed = 1.0f;

	//自機の座標(ホーミング用)
	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;

	XMFLOAT3 bossPos;
	XMFLOAT3 childShotRange;

	//発射タイマー
	int32_t shotTimer = 0;
	
	//円運動

	float radius = angle * 3.14f / 180.0f;//円の角度
	float length = 0.3f;//円の半径
	float addCircleX = cos(radius) * length;//円上の位置 x
	float addCircleY = sin(radius) * length;//円上の位置 y
	float addCircleZ = sin(radius) * length;//円上の位置 z
	float angleVec = 1.0f;


	bool isDead = false;

	int bossHp = 50;
	float hpBar = 0;
	float hpBarMax = 0;

	//∞の形の動く用
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

	//敵が死んだ数 (チュートリアルに使う)
	int enemyDown = 0;


	//Pが最初についているのはPhase用
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
	float angle = 90.0f;//最初の角度
	Phase phase = Phase::OutApproach;
	BossPhase phaseMini = BossPhase::None;

	bool bossDamage = false;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
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

	//スプライトhpバー用
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

	//発射タイマーをへらす
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
