#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Enemy.h"

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

	int32_t coolTimer = 5;
	int32_t bulletCoolTimer = 5;

	bool rockOn = false;

public:
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	void Init(Model* model, Model* bulletModel);
	void Attack();
	XMVECTOR transform(const XMVECTOR& v, const DirectX::XMMATRIX& m);
	XMVECTOR transform2(const XMVECTOR& v, const DirectX::XMMATRIX& m);
	void Update();

	void Draw();

	//ゲームシーンからマウス座標をうけとる
	void SetReticleWorldPos(XMVECTOR reticlePos);

	XMMATRIX GetViewPort(XMMATRIX viewPort) { return viewPortMatrix = viewPort; }
	XMMATRIX GetCameraMatViewProjection(XMMATRIX matViewProjection)
	{return cameraMatViewProjection = matViewProjection;}


	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetReticleWorldPosition();
	XMFLOAT3 GetSpriteReticle();

	//自機の弾のロックオンに必要
	void SetEnemyPosition(XMFLOAT3 position) { enemyWorldPos = position; }

	void OnCollision();
};
