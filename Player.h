#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Matrix4.h"
#include <memory>//ユニークポインタ
#include <list>



class Player
{
private:
	//モデル
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	Model* reticleModel_ = nullptr;

	//プレイヤー　レティクル　オブジェクトの初期化
	Object3d* player = player->Create();
	Object3d* reticle = reticle->Create();

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//入力　初期化
	Input* input = nullptr;

	//弾の速さ
	const float playerVelocity = 1.2f;

	//カメラの行列
	XMMATRIX cameraObj;
	//敵の座標
	XMFLOAT3 enemyWorldPos;
	//レティクルの座標
	XMVECTOR reticleWorldPos;
	//ビューポート
	XMMATRIX viewPortMatrix;
	
	XMMATRIX cameraMatViewProjection;
	
	//カメラの座標
	XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };

	//カメラの移動量
	XMFLOAT3 cameraEyeVec = { 0.0f,0.0f,0.0f };
	XMVECTOR cameraTargetVec = { 0.0f,0.0f,0.0f };

	
	XMVECTOR posNear = { 0.0f };
	XMVECTOR posFar = { 0.0f };

	HWND hwnd;
	XMMATRIX viewPort;

	//マウス座標
	float mouseX;
	float mouseY;


	bool isDeadEnemy = false;
	//弾のクールタイム
	int32_t coolTimer = 10;
	int32_t bulletCoolTimer =10;

	bool rockOn = false;

	//プレイヤーの動ける範囲
	XMFLOAT2 playerMoveRange = { 25,15 };
	
	//プレイヤーhp
	int playerHp = 3;
	float hpBar = 0;
	bool playerDieFlag = false;
public:
	//プレイヤーの弾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">プレイヤーのモデル</param>
	/// <param name="bulletModel">弾のモデル</param>
	void Init(Model* model, Model* bulletModel);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	XMVECTOR transform(const XMVECTOR& v, const DirectX::XMMATRIX& m);
	XMVECTOR transform2(const XMVECTOR& v, const DirectX::XMMATRIX& m);

	//ゲームシーンからマウス座標をうけとる
	void SetReticleWorldPos(XMVECTOR reticlePos);

	void SetViewPort(XMMATRIX viewPort) { viewPortMatrix = viewPort; }
	void SetHwnd(HWND hwnd) { this->hwnd = hwnd; }
	void SetCameraMatViewProjection(XMMATRIX matViewProjection){ cameraMatViewProjection = matViewProjection;}

	//自機の弾のロックオンに必要
	void SetEnemyPosition(XMFLOAT3 position) { enemyWorldPos = position; }
	void SetEnemyFlag(bool isDead) { isDeadEnemy = isDead; }

	void SetCameraObj(XMMATRIX cameraObj) { this->cameraObj = cameraObj; }

	void SetCameraPos(XMFLOAT3 cameraPos) { this->cameraPos = cameraPos; }

	void SetCameraEyeVec(XMFLOAT3 cameraEyeV) { this->cameraEyeVec = cameraEyeV; }
	void SetCameraTargetVec(XMFLOAT3 cameraTargetV) { this->cameraTargetVec.m128_f32[0] = cameraTargetV.x; 
	this->cameraTargetVec.m128_f32[1] = cameraTargetV.y;
	this->cameraTargetVec.m128_f32[2] = cameraTargetV.z;
	}

	void SetPlayerHpBar(float hpBar) { this->hpBar = hpBar; }

	FLOAT GetHpBar() { return hpBar; }

	BOOL GetHp0() { return playerDieFlag; }

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetReticleWorldPosition();
	XMFLOAT3 GetReticleNear();
	XMFLOAT3 GetReticleFar();
	
	//マウス座標
	FLOAT GetMouseX(){return  mouseX;}
	FLOAT GetMouseY() { return  mouseY; }

	//当たり判定
	void OnCollision();
};
