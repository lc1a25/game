#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "Matrix4.h"

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
	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio);

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

	//パーティクル
	void PlayerCreateParticle(XMFLOAT3 position);

	void EnemyCreateParticle(XMFLOAT3 position);

	void BossCreateParticle(XMFLOAT3 position);

	FLOAT GetbossHpBar() { return bossHpBar; }
	FLOAT GetHpBar() { return hpBar; }

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
	int bossDieTimer = 120;

	//プレイヤーhp
	float hpBar = 288;
	float hpBarMax = 288;
	bool playerDieFlag = false;

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
	//プレイヤー
	Player* player = nullptr;

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

	//パーティクル
	ParticleManager* Particle = nullptr;

	//当たり判定用変数
	float length = 0.0f;
	float size = 22.0f;
	float wallColliLength = 0.0f;

	//床のy
	float floorY = -55.0f;

};
