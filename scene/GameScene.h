#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "Matrix4.h"
#include "Win.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyCircle.h"
#include "EnemyOneWay.h"
#include "CameraObj.h"
#include "Boss.h"
#include "BossChild.h"
#include "ParticleManager.h"
#include <sstream>
#include "Bill.h"


class GameScene
{

private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio, Win* win);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//当たり判定
	void CheckAllCollision(Enemy* enemy);
	void CheckBossANDChildCollision(Enemy* bossChild);
	void CheckPillarCollision();

	//hwndの取得
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//ビューポート行列の取得
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }

	//csv読み込み
	void EnemyPopLoadData();

	void UpdateEnemyPop();
	
	XMFLOAT3 CommandPositionSet(std::istream &line_stream, std::string &word);

	
	void SetGameFlag(int gameFlag) { this->gameflag = gameFlag; }
	//hpバー
	FLOAT GetbossHpBar() { return bossHpBar; }
	FLOAT GetHpBar() { return hpBar; }

	//ビル生成
	void BillCreate();

	//マウス座標
	float mouseX;
	float mouseY;

	//デバッグテキスト用
	char moji[64];
	char moji2[64];

	bool pointsLast = false;
	bool bossFlag = false;

	//コマンド用
	bool waitCommand = false;
	int waitTimer = 0;

	//コマンド用
	bool waitRailIndexCommand = false;
	int waitRail = 0;

	//ボス系
	float bossHpBar = 733;
	float bossHpBarMax = 733;

	//プレイヤーhp
	float hpBar = 288;
	float hpBarMax = 288;
	bool playerDieFlag = false;

	int dieTimer = 120;
	int  coll = 0;

	bool tutorialFlag = true;

	bool tutorialFlag2 = true;

	bool sceneChange = false;

	//csv　コマンド読み込み
	std::stringstream enemyPopCommands;

private: // メンバ変数

	HWND hwnd;
	//ビューポート
	XMMATRIX viewPort;

	DirectXCommon* dxcommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Win* win = nullptr;

	//カメラ
	Camera* camera = nullptr;

	CameraObj* cameraObj = nullptr;//カメラオブジェクト

	//スカイドーム
	Object3d* skydome = skydome->Create();

	//壁系
	Object3d* wallFloor = wallFloor->Create();
	//道
	Object3d* road = road->Create();

	//ビル
	Object3d* pillar = pillar->Create();
	Object3d* pillar2 = pillar2->Create();
	Object3d* pillar3 = pillar3->Create();
	Object3d* pillar4 = pillar4->Create();
	Object3d* pillar5 = pillar5->Create();

	std::list<std::unique_ptr<Bill>> bills;
	std::list<std::unique_ptr<EnemyOneWay>> oneWays;
	std::list<std::unique_ptr<EnemyCircle>> circles;

	Object3d* shotObj = shotObj->Create();
	Object3d* shotHibiObj = shotHibiObj->Create();

	Object3d* kanbanObj = kanbanObj->Create();
	Object3d* kanbanPlaneObj = kanbanPlaneObj->Create();
	Object3d* kanbanShotObj = kanbanShotObj->Create();
	Object3d* kanbanShot2Obj = kanbanShot2Obj->Create();
	Object3d* kanbanShot3Obj = kanbanShot3Obj->Create();
	Object3d* kanbanShot4Obj = kanbanShot4Obj->Create();

	//ボスのバリア
	Object3d* barrier = barrier->Create();
	//プレイヤー
	Player* player = nullptr;
	Object3d* startPlayer = startPlayer->Create();

	//敵
	Enemy* enemy = nullptr;
	Enemy* enemyL = nullptr;
	EnemyCircle* enemyCircle = nullptr;
	EnemyCircle* enemyCircle2 = nullptr;
	EnemyOneWay* enemyOneWay = nullptr;
	EnemyOneWay* enemyOneWay2 = nullptr;
	Boss* boss = nullptr;
	BossChild* bossChildLUF = nullptr;
	BossChild* bossChildLUB = nullptr;
	BossChild* bossChildRUF = nullptr;
	BossChild* bossChildRUB = nullptr;
	BossChild* bossChildLDF = nullptr;
	BossChild* bossChildLDB = nullptr;
	BossChild* bossChildRDF = nullptr;
	BossChild* bossChildRDB = nullptr;

	XMVECTOR reticlePos;//レティクル用変数

	//モデル
	Model* skydome_model = nullptr;

	Model* playerModel = nullptr;
	Model* bulletModel = nullptr;
	Model* enemyModel = nullptr;
	Model* enemyRotateModel = nullptr;
	Model* bossModel = nullptr;
	Model* bossMiniModel = nullptr;
	Model* wallModel = nullptr;
	Model* wallBossModel = nullptr;
	Model* wallFlatModel = nullptr;
	Model* pillarModel = nullptr;
	Model* enemyBulletModel = nullptr;
	Model* roadModel = nullptr;
	Model* shotObjModel = nullptr;
	Model* shotHibiObjModel = nullptr;
	Model* kanbanModel = nullptr;
	Model* kanbanPlaneModel = nullptr;
	Model* kanbanShotModel = nullptr;
	Model* kanbanShot2Model = nullptr;
	Model* kanbanShot3Model = nullptr;
	Model* kanbanShot4Model = nullptr;

	Model* barrierModel = nullptr;
	Model* barrier2Model = nullptr;

	//パーティクル
	ParticleManager* Particle = nullptr;

	//当たり判定用変数
	float length = 0.0f;
	float size = 25.0f;
	float wallColliLength = 0.0f;

	//床のy
	float floorY = -55.0f;

	//skydome z
	float skydomeZ = 0;

	int kanbanTime = 0;
	int kanbanTimeMax = 30;
	XMFLOAT3 kanbanShotPos = { 90,-30,115 };
	XMFLOAT3 kanbanShotPosDown = { 90,-90,115 };

	bool mutekiFlagDeb = false;
	bool mutekiFlag = false;
	int mutekiCoolTimeMax = 120;
	int mutekiCoolTime = mutekiCoolTimeMax;
	bool movieSkipFlag = false;
	bool setObjectFlag = false;

	//チュートリアル文字のｈｐ
	int mojiHp = 10;

	float bossHpX = 733.0f;
	float hp = 288;
	int gameflag = 0;

	//shotObjの当たり判定調整用
	float shotObjAddy = 10;
	XMFLOAT3 bullet1;

	bool attackParticleFlag = false;
	bool shotBreakFlag = false;
	bool gameStart = false;

	int randBill = 0;
	float billScaleY = 13;
	int randBillRot = 0;
	XMFLOAT3 billRotation;

	Sprite* reticleSprite;
	Sprite* explanSprite;
	Sprite* titleSprite;
	Sprite* endSprite;
	Sprite* bossHpWakuSprite;
	Sprite* bossHpBarSprite;
	Sprite* playerHpSprite;
	Sprite* gameOverSprite;
	Sprite* tyutoRial;
	Sprite* tyutoRialMove;
	Sprite* backBlack;
	Sprite* kSkipSprite;

	DebugText* debugtext_minute = nullptr;
	DebugText* debugtext_minute2 = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	//パーティクル
	void PlayerCreateParticle(XMFLOAT3 position);
	void EnemyCreateParticle(XMFLOAT3 position);
	void BossCreateParticle(XMFLOAT3 position);
	void MojiCreateParticle(XMFLOAT3 position);
	void MojiBreakParticle(XMFLOAT3 position);
	void CreateParticle(int particleCount ,int lifeTime, XMFLOAT3 position, float vec, float accel, float start_scale, float end_scale, XMFLOAT3 start_color, XMFLOAT3 end_color);

	FLOAT BillScaleY(int randam);
	XMFLOAT3 BillRot(int randam);
};

