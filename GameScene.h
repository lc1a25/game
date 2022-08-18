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
#include "CameraObj.h"

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

	//hwndの取得
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//ビューポート行列の取得
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }


	float mouseX;
	float mouseY;
	char moji[64];
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

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* enemyL = nullptr;
	EnemyCircle* enemyCircle = nullptr;

	XMVECTOR reticlePos;//レティクル用変数

	//モデル
	Model* playerModel = nullptr;
	Model* bulletModel = nullptr;
	Model* enemyModel = nullptr;


	//当たり判定用変数
	float length = 0.0f;
	float size = 2.0f;
	//std::unique_ptr<Enemy> enemy(new Enemy());

};
