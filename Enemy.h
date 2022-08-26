#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"

#include <memory>
#include <list>

//class Player;//ˆË‘¶‚µ‚È‚¢‚Åƒ|ƒCƒ“ƒ^‚à‚Ä‚é

enum class Phase
{
	Approach,//Ú‹ß
	ApproachL,
	Leave,	//—£’E
	Stop,  //~‚Ü‚é
	CircleR,//‰~‰^“®‰E‚©‚ç‚­‚é
	CircleL,//¶‚©‚ç‚­‚é
};

class Enemy
{
private:
	Model* enemyModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	
	Object3d* enemy = enemy->Create();
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//Player* player_ = nullptr;


	XMFLOAT3 playerWorldPos;
	XMVECTOR lockOn;
	//”­Ëƒ^ƒCƒ}[
	int32_t shotTimer = 0;

	//‰~‰^“®
	float angle = 90.0f;//Å‰‚ÌŠp“x
	float radius = angle * 3.14f / 180.0f ;//‰~‚ÌŠp“x
	float length = 0.3f;//‰~‚Ì”¼Œa
	float addCircleX = cos(radius) * length;//‰~ã‚ÌˆÊ’u x
	float addCircleY = sin(radius) * length;//‰~ã‚ÌˆÊ’u y

	bool isDead = false;
public:


	Phase phase = Phase::CircleL;
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//”­ËŠÔŠu
	static const int shotInterval = 60;

	void Init(Model* enemyModel,XMFLOAT3 position, bool rightMoveTrue);

	void Update();

	void Draw();

	void Shot();

	void PhaseInit(bool rightMoveTrue);

	//void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerPosition(XMFLOAT3 position) { playerWorldPos = position; }

	void SetPosition(XMFLOAT3 position) { enemy->position = position; }

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 GetPosition() { return enemy->position; }

	void OnCollision();

	void ShotInit();

	//”­Ëƒ^ƒCƒ}[‚ğ‚Ö‚ç‚·
	void ShotTimerLower(int32_t lowerTime = 1);

	bool IsTimer();

	bool IsDead() const { return isDead; }

};
