#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Matrix4.h"
#include <memory>//���j�[�N�|�C���^
#include <list>

class Player
{
private:
	//���f��
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	Model* reticleModel_ = nullptr;

	//�v���C���[�@���e�B�N���@�I�u�W�F�N�g�̏�����
	Object3d* player = player->Create();
	Object3d* reticle = reticle->Create();

	//�v���C���[��z���W(�J������z���W�Ƀv���X����)
	XMVECTOR playerStartPos = { 0 };
	XMVECTOR playerStartPos2 = { 0 };
	float playerZ = 30;
	//���e�B�N����z���W(�v���C���[��z���W�Ƀv���X����)
	float reticleZ = 100;
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//���́@������
	Input* input = nullptr;

	//�e�̑���
	const float playerVelocity = 1.2f;

	//�J�����̍s��
	XMMATRIX cameraObj;
	//�G�̍��W
	XMFLOAT3 enemyWorldPos;
	//���e�B�N���̍��W
	XMVECTOR reticleWorldPos;
	//�r���[�|�[�g
	XMMATRIX viewPortMatrix;
	
	XMMATRIX cameraMatViewProjection;
	
	//�J�����̍��W
	XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };

	//�J�����̈ړ���
	XMFLOAT3 cameraEyeVec = { 0.0f,0.0f,0.0f };
	XMVECTOR cameraTargetVec = { 0.0f,0.0f,0.0f };

	
	XMVECTOR posNear = { 0.0f };
	XMVECTOR posFar = { 0.0f };

	HWND hwnd;
	XMMATRIX viewPort;

	//�}�E�X���W
	float mouseX;
	float mouseY;


	bool isDeadEnemy = false;
	//�e�̃N�[���^�C��
	int32_t coolTimer = 10;
	int32_t bulletCoolTimer =10;

	bool rockOn = false;

	//�v���C���[�̓�����͈�
	XMFLOAT2 playerMoveRange = { 25,15 };
	
	//�v���C���[hp
	int playerHp = 3;
	float hpBar = 0;
	bool playerDieFlag = false;

	bool gameStartFlag = false;
	bool playerSet = true;
	bool keyInput = false;
	bool gameEndFlag = false;
public:

	//�v���C���[�̒e�@�����蔻��ł���
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">�v���C���[�̃��f��</param>
	/// <param name="bulletModel">�e�̃��f��</param>
	void Init(Model* model, Model* bulletModel);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//void gameEnd();

	XMVECTOR transform(const XMVECTOR& v, const DirectX::XMMATRIX& m);
	XMVECTOR transform2(const XMVECTOR& v, const DirectX::XMMATRIX& m);

	XMVECTOR ease_in(const XMVECTOR& start, const XMVECTOR& end, float t);

	//�Q�[���V�[������}�E�X���W�������Ƃ�
	void SetReticleWorldPos(XMVECTOR reticlePos);

	void SetViewPort(XMMATRIX viewPort) { viewPortMatrix = viewPort; }
	void SetHwnd(HWND hwnd) { this->hwnd = hwnd; }
	void SetCameraMatViewProjection(XMMATRIX matViewProjection){ cameraMatViewProjection = matViewProjection;}

	//���@�̒e�̃��b�N�I���ɕK�v
	void SetEnemyPosition(XMFLOAT3 position) { enemyWorldPos = position; }
	void SetEnemyFlag(bool isDead) { isDeadEnemy = isDead; }

	void SetCameraObj(XMMATRIX cameraObj) { this->cameraObj = cameraObj; }

	void SetCameraPos(XMFLOAT3 cameraPos) { this->cameraPos = cameraPos; }

	void SetCameraEyeVec(XMFLOAT3 cameraEyeV) { this->cameraEyeVec = cameraEyeV; }

	void SetPlayerHpBar(float hpBar) { this->hpBar = hpBar; }

	void SetStartFlag(bool gameStartFlag) { this->gameStartFlag = gameStartFlag; }

	void SetkeyInput(bool keyInput) { this->keyInput = keyInput; }

	void SetPlayerPos(XMFLOAT3 pos) {player->position.y -= pos.y; }

	void SetEndFlag(bool gameEndFlag) { this->gameEndFlag = gameEndFlag; }

	FLOAT GetHpBar() { return hpBar; }

	BOOL GetHp0() { return playerDieFlag; }

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetReticleWorldPosition();
	XMFLOAT3 GetReticleNear();
	XMFLOAT3 GetReticleFar();
	
	//�}�E�X���W
	FLOAT GetMouseX(){return  mouseX;}
	FLOAT GetMouseY() { return  mouseY; }

	//�����蔻��
	void OnCollision();
};
