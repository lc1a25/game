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

	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	Model* reticleModel_ = nullptr;

	Enemy* enemy = nullptr;

	Object3d* player = player->Create();
	Object3d* reticle = reticle->Create();
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Input* input = nullptr;

	const float playerVelocity = 1.2f;

	XMFLOAT3 enemyWorldPos;
	XMVECTOR reticleWorldPos;
	XMMATRIX viewPortMatrix;
	XMMATRIX cameraMatViewProjection;
	XMVECTOR positionReticle = {0,0,0};

	HWND hwnd;
	XMMATRIX viewPort;

	float mouseX;
	float mouseY;

	bool isDeadEnemy = false;
	//弾のクールタイム
	int32_t coolTimer = 5;
	int32_t bulletCoolTimer = 5;

	bool rockOn = false;

public:
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	void Init(Model* model, Model* bulletModel);
	void Attack();

	void Update();

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

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetReticleWorldPosition();
	XMFLOAT3 GetSpriteReticle();
	FLOAT GetMouseX(){return  mouseX;}
	FLOAT GetMouseY() { return  mouseY; }



	void OnCollision();
};
