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
#include "BillManager.h"
#include "Bill.h"
#include "Light.h"
#include "LightGroup.h"

enum class Scene
{
	Title,
	Game,
	Clear,
	GameOver,
};
enum class SceneDraw
{
	TitleDraw,
	GameDraw,
	ClearDraw,
	GameOverDraw,
};

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
	void CheckTargetCollision();
	void CheckBillCollision(BillManager* bill);
	//hwndの取得
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//ビューポート行列の取得
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }

	//csv読み込み
	void EnemyPopLoadData();

	void UpdateEnemyPop();

	XMFLOAT3 CommandPositionSet(std::istream& line_stream, std::string& word);

	void SetGameFlag(int gameFlag) { this->gameScene = gameFlag; }

	void Title();
	void Game();
	void Clear();
	void GameOver();
	void TitleDraw();
	void GameDraw();
	void ClearDraw();
	void GameOverDraw();

	//マウス座標
	float mouseX;
	float mouseY;

	//デバッグテキスト用
	char moji[64];
	char moji2[64];

	bool pointsLast = false;
	bool bossFlag = false;

	bool playerDieFlag = false;
	//コマンド用
	bool waitCommand = false;
	int waitTimer = 0;

	//コマンド用
	bool waitRailIndexCommand = false;
	int waitRail = 0;

	bool tutorialFlag = true;

	bool sceneChange = false;

	//csv　コマンド読み込み
	std::stringstream enemyPopCommands;

	Light* light = nullptr;
	LightGroup* lightGroup = nullptr;

private: // メンバ変数
	int phaseNumber = static_cast<int>(scene_);
	static void (GameScene::* spFuncTable[])();
	int phaseDrawNumber = static_cast<int>(sceneDraw_);
	static void (GameScene::* spFuncTableDraw[])();
	Scene scene_ = Scene::Title;
	SceneDraw sceneDraw_ = SceneDraw::TitleDraw;
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

	Object3d* titleObj = titleObj->Create();

	BillManager* billM = nullptr;

	std::list<std::unique_ptr<EnemyOneWay>> oneWays;
	std::list<std::unique_ptr<EnemyOneWay>> oneWayMovies;
	std::list<std::unique_ptr<EnemyCircle>> circles;

	Object3d* shotObj = shotObj->Create();
	Object3d* shotHibiObj = shotHibiObj->Create();
	Object3d* targetObj = targetObj->Create();
	Object3d* targetObj2 = targetObj2->Create();
	Object3d* targetObj3 = targetObj3->Create();
	Object3d* clickObj = clickObj->Create();
	Object3d* clearObj = clearObj->Create();

	Object3d* kanbanObj = kanbanObj->Create();
	Object3d* kanbanPlaneObj = kanbanPlaneObj->Create();
	Object3d* kanbanShotObj = kanbanShotObj->Create();
	Object3d* kanbanShot2Obj = kanbanShot2Obj->Create();
	Object3d* kanbanShot3Obj = kanbanShot3Obj->Create();
	Object3d* kanbanShot4Obj = kanbanShot4Obj->Create();

	//プレイヤー
	Player* player = nullptr;
	Object3d* startPlayer = startPlayer->Create();

	//敵
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
	Model* billModel = nullptr;
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
	Model* targetModel = nullptr;
	Model* clickModel = nullptr;
	Model* clearModel = nullptr;
	Model* deadModel = nullptr;
	

	//パーティクル
	ParticleManager* Particle = nullptr;

	float particleCountTitle = 30;
	float particleLifeTitle = 64;
	XMFLOAT3 particleLeftPosTitle = { -movieParticleXL, 0,-320 };
	XMFLOAT3 particleVelocityTitle = { 0.5,0.5,0.5 };
	XMFLOAT3 particleVelocityClear = { 3.5,4.0,0.8 };
	XMFLOAT3 particleAccelTitle = { 0.02,0.02,0.02 };
	XMFLOAT3 particleAccelClear = { 0.1,0.05,0.1 };
	float particleMaxPosXTitle = 80;
	float particleMinPosXTitle = 40;

	//当たり判定用変数
	float length = 0.0f;
	float size = 25.0f;
	float wallColliLength = 0.0f;
	float startPlayerAddZ = 4.0f;
	float startPlayerAddY = 0.3f;

	XMFLOAT3 startPlayerAfterPos = { 0,-200,610 };
	float endPlayerPos = 1400.0f;

	//床のy
	float floorY = -55.0f;

	//skydome 
	XMFLOAT3 skydomePos = { 0,-220,0 };
	float skydomeZ = 0;
	float skydomeVec = 0.8;

	//看板のアニメーション
	int kanbanTime = 0;
	int kanbanTimeMax = 30;
	XMFLOAT3 kanbanShotPos = { 90,-30,115 };
	XMFLOAT3 kanbanShotPosDown = { 90,-90,115 };

    //無敵時間
	bool mutekiFlagDeb = false;
	
	bool mutekiFlag = false;
	int mutekiCoolTimeMax = 420;
	int mutekiCoolTimeMin = 0;
	int mutekiCoolTime = mutekiCoolTimeMax;

	int tenmetuCountReset = 0;//無敵時間中に点滅させるため
	int tenmetuCount = tenmetuCountReset;//無敵時間中に点滅させるため
	int tenmetuAliveCount = 10;//無敵時間中にキャラを出す時間
	int tenmetuDeadCount = 20;//無敵時間中にキャラを出さない時間

	int clickCountReset = 0;//点滅させるため
	int clickCount = clickCountReset;//点滅させるため
	int clickAliveCount = 15;//キャラを出す時間
	int clickDeadCount = 30;//キャラを出さない時間

	bool movieSkipFlag = false;//ムービースキップ用
	bool setObjectFlag = false;//ムービー後にオブジェクトをセットする用

	//チュートリアル文字のｈｐ
	int targetHpMax = 3;
	int targetHp = targetHpMax;
	int targetChangeHp = targetHp - 1;
	int count = 0;

	int gameScene = 0;//スプライトのゲームシーン
	int gameSceneTitle = 0;//スプライトのタイトルシーン
	int gameSceneInGame = 1;//スプライトのインゲームシーン
	int gameSceneClear = 2;//スプライトのクリアシーン
	int gameSceneGameOver = 3;//スプライトのゲームオーバーシーン

	//ボス系
	float bossHpBar = 733;
	float bossHpBarMax = 733;

	//プレイヤーhp
	float hpBar = 288;
	float hpBarMax = 288;


	int dieTimerMax = 120;
	int dieTimer = dieTimerMax;
	int  coll = 0;
	
	float shotObjAddy = 10;//shotObjの当たり判定調整用
	XMFLOAT3 bullet1;//プレイヤーの弾の座標をいれる用

	int movieParticleTime = 0;
	float movieParticleXL = 80;
	float movieParticleXR = 40;
	bool attackParticleFlag = false;//プレイヤーの弾のパーティクルがでるときに使うflag
	bool gameStartFlag = false;//タイトルからゲームシーンに入ったことがわかるflag

	int randBill = 0;//ビルをランダム配置するときにrandの数値をいれる(scale用)
	float billScaleY = 13;//ビルの縦方向のスケール
	int randBillRot = 0;//ビルをランダム配置するときにrandの数値をいれる(rotation用)
	XMFLOAT3 billRotation;//ビルのローテーション

	//スプライト
	Sprite* reticleSprite;//照準
	Sprite* bossHpWakuSprite;
	Sprite* bossHpBarSprite;
	Sprite* playerHpSprite;
	Sprite* kSkipSprite;

	DebugText* debugtext_minute = nullptr;
	DebugText* debugtext_minute2 = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	void ParticleClear();
	void ParticleTitle();
};

