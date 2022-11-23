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

#include <sstream>

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
	void CheckPillarCollision();

	//hwndの取得
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//ビューポート行列の取得
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }

	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	void EnemyPopLoadData();

	void UpdateEnemyPop();

	void SetBossHp(FLOAT bossHpX) { bossHpBar = bossHpX; }

	FLOAT GetbossHpBar() { return bossHpBar; }


	float mouseX;
	float mouseY;
	char moji[64];
	char moji2[64];

	bool pointsLast = false;

	//コマンド用
	bool waitCommand = false;
	int waitTimer = 0;
	//コマンド用
	bool waitRaleIndexCommand = false;
	int waitRale = 0;

	//ボス系
	float bossHpBar = 733;
	float bossHpBarMax = 733;
	int bossDieTimer = 120;

	int  coll = 0;

	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	std::stringstream enemyPopCommands;
private: // メンバ変数

	HWND hwnd;
	XMMATRIX viewPort;
	DirectXCommon* dxcommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	//DebugText* debugText;

	//カメラ
	Camera* camera = nullptr;

	CameraObj* cameraObj = nullptr;//カメラオブジェクト

	Object3d* wall = wall->Create();
	Object3d* wallBoss = wallBoss->Create();
	Object3d* wallBossBack = wallBossBack->Create();
	Object3d* pillar = pillar->Create();
	Object3d* pillar2 = pillar2->Create();

	Object3d* pillar3 = pillar3->Create();
	Object3d* pillar4 = pillar4->Create();

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* enemyL = nullptr;
	EnemyCircle* enemyCircle = nullptr;
	EnemyCircle* enemyCircle2 = nullptr;
	EnemyOneWay* enemyOneWay = nullptr;
	EnemyOneWay* enemyOneWay2 = nullptr;
	Boss* boss = nullptr;
	

	XMVECTOR reticlePos;//レティクル用変数

	//モデル
	Model* playerModel = nullptr;
	Model* bulletModel = nullptr;
	Model* enemyModel = nullptr;
	Model* bossModel = nullptr;
	Model* bossMiniModel = nullptr;
	Model* wallModel = nullptr;
	Model* wallBossModel = nullptr;
	Model* wallFlatModel = nullptr;
	Model* pillarModel = nullptr;


	//当たり判定用変数
	float length = 0.0f;
	float size = 2.0f;
	float wallColliLength = 0.0f;
	//std::unique_ptr<Enemy> enemy(new Enemy());

};
