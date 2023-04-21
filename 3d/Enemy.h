#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//依存しないでポインタもてる
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
	BossApproach,
	BossVertical,//bossのミニが縦方向に動く
	BossVerticalL,
	BossSide,//bossのミニが横方向に動く
	BossSideUp,
	BossBarrier,
	BossStop,
	BossDead,
	BossMiniStop,
	BossMiniBarrier,
	BossMiniDead,
	BossPhaseChange,
	None,
};



class Enemy
{
private:
	
	int phaseNumber = static_cast<int>(phase);
	static void (Enemy::*spFuncTable[])();
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	
	//OneWayの敵のスピード
	float OWRSpeed = 0.5f;
	float OWLSpeed = 0.5f;
	XMVECTOR OneWayPos;
	XMVECTOR MiniPosLUF;

	XMVECTOR LUFPosVertical = { -40, MiniPosLUF.m128_f32[1], MiniPosLUF.m128_f32[2] };
	
	
	int leaveTimeInit = 300;
	int leaveTime = leaveTimeInit;
	int barrierTime = 0;

	long long startCount = timeGetTime();
	long long nowCount = 0;
	long long elapsedCount = 0;

	float maxTime = 1.0f;
	float timeRate = 0.0f;

	//別Phaseに移行するまでに移動しているスピード
	float ApproachSpeed = 0.5f;

	bool shotEndFlag = false;
	float outApproachSpeed = 1.0f;

	//自機の座標(ホーミング用)
	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	XMVECTOR playerPos;

	XMFLOAT3 bossPos;
	XMFLOAT3 childShotRange;

	//発射タイマー
	int32_t shotTimer = 0;

	int worldZ = 170;
	int worldZRange = 20;
	int worldZAdd = 40;
	
	//円運動

	float radius = angle * 3.14f / 180.0f;//円の角度
	float length = 0.3f;//円の半径
	float lengthBoss = 10.3f;//円の半径
	float addCircleX = cos(radius) * length;//円上の位置 x
	float addCircleY = sin(radius) * length;//円上の位置 y
	float addCircleZ = sin(radius) * length;//円上の位置 z
	float addCircleX2 = cos(radius) * lengthBoss;//円上の位置 x
	float addCircleY2 = sin(radius) * lengthBoss;//円上の位置 y
	float addCircleZ2 = sin(radius) * lengthBoss;//円上の位置 z
	float angleVec = 1.0f;


	bool isDead = false;

	int bossMiniHp = 5;

	int bossHp = 50;
	float hpBar = 0;
	float hpBarMax = 0;

	//∞の形の動く用
	bool isL = false;

	int childNumber;
	XMFLOAT3 bossVec;

	float waitTimer = 120;

	const float waitTimerInterval = 120;

	//発射感覚
	int shotMinorTimer = 60;
	const int shotMinorInterval = 40;

	bool circleZFlag = false;

	float cameraZ = 0.0f;

	bool attackFlag = true;
	bool barrierFlag = false;
	bool barrierPhaseFlag = false;
	bool bossMovieFlag = false;

	int changeTime = 0;

	float randAdd;

	XMFLOAT3 barrierPos;

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

	void PCircleBoss(float addCircleSize = 0);

	void PChild();

	void PWait();

	void PWaitLB();

	void PWaitR();

	void PWaitRB();

	void PChildHoming();

	void PChangeBossDead();


	XMVECTOR ease_in(const XMVECTOR& start, const XMVECTOR& end, float t);
	XMVECTOR lerp(const XMVECTOR& start, const XMVECTOR& end, const float t);
public:
	float angle = 90.0f;//最初の角度
	Phase phase = Phase::OutApproach;
	//bool bossDamage = false;

	//当たり判定用　弾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
	static const int shotInterval = 30;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="bulletModelXMFLOAT3">弾のモデル</param>
	/// <param name="scale">サイズ</param>
	/// <param name="attackFlag">攻撃するか</param>
	void Init(Model* enemyModel,XMFLOAT3 position, Model *bulletModelXMFLOAT3 , XMFLOAT3 scale = { 2,2,2 },bool attackFlag = true);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Approach();//接近
	void ApproachL();
	void OutApproach();
	void LeaveL();	//離脱
	void LeaveR();
	void LeaveF();
	void Stop(); //止まる
	void CircleR();//円運動右からくる
	void CircleL();//左からくる
	void CircleInfinity();//∞上にまわる
	void OneWayR();//右から左に行く
	void OneWayL();//左から右に行く
	void BossApproach();
	void BossVertical();//bossのミニが縦方向に動く
	void BossVerticalL();
	void BossSide();//bossのミニが横方向に動く
	void BossSideUp();
	void BossBarrier();
	void BossStop();	
	void BossDead();
	void BossMiniStop();
	void BossMiniBarrier();
	void BossMiniDead();
	void BossPhaseChange();
	void None();

	

	/// <summary>
	/// ホーミング
	/// </summary>
	void Homing();

	/// <summary>
	/// ボスの周りのコアがボスに向かっていく
	/// </summary>
	void ChildHoming();

	/// <summary>
	/// 前に向かって弾を打つ
	/// </summary>
	void FrontShot();

	/// <summary>
	/// Phase の　初期化
	/// </summary>
	/// <param name="rightMoveTrue">敵が画面の右からでてくるか左から出てくるか</param>
	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// プレイヤーの座標
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	/// <summary>
	/// 敵の座標
	/// </summary>
	/// <param name="position">敵の座標</param>
	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	/// <summary>
	/// カメラのz座標  敵がカメラのzの方向の動きに合わせる
	/// </summary>
	/// <param name="z">カメラのz座標</param>
	void SetCameraZ(float z) { cameraZ = z; }

	/// <summary>
	/// ボスの周りをまわる敵の番号(全部で8体いる)セット
	/// </summary>
	/// <param name="childNumber"> ボスの周りをまわる敵の番号</param>
	void SetChildNumber(int childNumber) { this->childNumber = childNumber; }

	/// <summary>
	/// ボスの移動量
	/// </summary>
	/// <param name="bossVec">ボスの移動量</param>
	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	/// <summary>
	/// ボスの座標
	/// </summary>
	/// <param name="bossPos">ボスの座標</param>
	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	/// <summary>
	/// 弾の発射間隔
	/// </summary>
	/// <param name="shotRange"></param>
	void SetChildShotRange(XMFLOAT3 childShotRange) { this->childShotRange = childShotRange; }

	//スプライトhpバー用
	void SetBossHpBar(float bossHpBar,float bossHpBarMax) {
		hpBar = bossHpBar;
		hpBarMax = bossHpBarMax;
	}

	void SetBossChildAngle(float angle) { this->angle = angle; }

	void SetBarrierFlag(bool barrierFlag) { this->barrierFlag = barrierFlag; }
	void SetBarrierPhaseFlag(bool barrierPhaseFlag) { this->barrierPhaseFlag = barrierPhaseFlag; }

	void SetBossHp(int bossHp) { this->bossHp = bossHp; }
	FLOAT GetHpBarX() { return hpBar; }

	void EasingTime();

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	INT GetphaseNumber() { return phaseNumber; }

	void OnCollision();

	void OnBossCollision();

	void OnBossMiniCollision();

	void ShotInit();

	//発射タイマーをへらす
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

	bool GetBarrierFlag() { return barrierFlag; }
	bool GetBarrierPhaseFlag() { return barrierPhaseFlag; }
	bool GetBossMovieFlag() { return bossMovieFlag; }

	INT GetBossHp() { return bossHp; }

};
