#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"

#include <memory>//ユニークポインタ
#include <list>

class Player
{
private:

	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;

	Object3d* player = player->Create();
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Input* input = nullptr;

	const float playerVelocity = 2.0f;

public:
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	void Init(Model* model, Model* bulletModel);
	void Attack();
	XMFLOAT3 transform(const XMFLOAT3& v, const DirectX::XMMATRIX& m);
	void Update();

	void Draw();

	XMFLOAT3 GetWorldPosition();

	void OnCollision();


	
};
