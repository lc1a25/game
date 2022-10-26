#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//àÀë∂ÇµÇ»Ç¢Ç≈É|ÉCÉìÉ^Ç‡ÇƒÇÈ

enum class Phase
{
	Approach,//ê⁄ãﬂ
	ApproachL,
	Leave,	//ó£íE
	Stop,  //é~Ç‹ÇÈ
	CircleR,//â~â^ìÆâEÇ©ÇÁÇ≠ÇÈ
	CircleL,//ç∂Ç©ÇÁÇ≠ÇÈ
	CircleInfinity,//Åáè„Ç…Ç‹ÇÌÇÈ
	OneWayR,//âEÇ©ÇÁç∂Ç…çsÇ≠
	OneWayL,//ç∂Ç©ÇÁâEÇ…çsÇ≠
};

class Enemy
{
private:
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//Player* player_ = nullptr;
	float OWRbulletSpeed = 0.5f;
	float OWLbulletSpeed = 0.5f;

	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//î≠éÀÉ^ÉCÉ}Å[
	int32_t shotTimer = 0;

	//â~â^ìÆ
	float angle = 90.0f;//ç≈èâÇÃäpìx
	float radius = angle * 3.14f / 180.0f ;//â~ÇÃäpìx
	float length = 0.3f;//â~ÇÃîºåa
	float addCircleX = cos(radius) * length;//â~è„ÇÃà íu x
	float addCircleY = sin(radius) * length;//â~è„ÇÃà íu y
	float angleVec = 0.5f;

	bool isDead = false;

	bool isL = false;

	float cameraZ = 0.0f;

	void CircleR();

	void CircleL();
public:


	Phase phase = Phase::Stop;
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//î≠éÀä‘äu
	static const int shotInterval = 60;

	void Init(Model* enemyModel,XMFLOAT3 position);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	void SetCameraZ(float z) { cameraZ = z; }

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	void OnCollision();

	void ShotInit();

	//î≠éÀÉ^ÉCÉ}Å[ÇÇ÷ÇÁÇ∑
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
