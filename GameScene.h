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
#include "BossChild.h"
#include "ParticleManager.h"
#include <sstream>

class GameScene
{

private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�����蔻��
	void CheckAllCollision(Enemy* enemy);
	void CheckBossANDChildCollision(Enemy* bossChild);
	void CheckPillarCollision();

	//hwnd�̎擾
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//�r���[�|�[�g�s��̎擾
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }

	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	void EnemyPopLoadData();

	void UpdateEnemyPop();

	FLOAT GetbossHpBar() { return bossHpBar; }
	FLOAT GetHpBar() { return hpBar; }

	float mouseX;
	float mouseY;
	char moji[64];
	char moji2[64];

	bool pointsLast = false;
	bool bossFlag = false;

	//�R�}���h�p
	bool waitCommand = false;
	int waitTimer = 0;
	//�R�}���h�p
	bool waitRaleIndexCommand = false;
	int waitRale = 0;

	//�{�X�n
	float bossHpBar = 733;
	float bossHpBarMax = 733;
	int bossDieTimer = 120;


	float hpBar = 288;
	float hpBarMax = 288;
	bool hp0 = false;
	int  coll = 0;

	bool tutorialFlag = true;

	bool tutorialFlag2 = true;

	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	std::stringstream enemyPopCommands;

private: // �����o�ϐ�

	HWND hwnd;
	XMMATRIX viewPort;
	DirectXCommon* dxcommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	//DebugText* debugText;

	//�J����
	Camera* camera = nullptr;

	CameraObj* cameraObj = nullptr;//�J�����I�u�W�F�N�g

	Object3d* wall = wall->Create();
	Object3d* wallBoss = wallBoss->Create();
	Object3d* wallBossBack = wallBossBack->Create();
	Object3d* wall2 = wall2->Create();
	Object3d* road = road->Create();

	Object3d* pillar = pillar->Create();
	Object3d* pillar2 = pillar2->Create();
	Object3d* pillar3 = pillar3->Create();
	Object3d* pillar4 = pillar4->Create();
	Object3d *pillar5 = pillar5->Create();

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* enemyL = nullptr;
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

	XMVECTOR reticlePos;//���e�B�N���p�ϐ�

	//���f��
	Model* playerModel = nullptr;
	Model* bulletModel = nullptr;
	Model* enemyModel = nullptr;
	Model* enemyRotateModel = 
		nullptr;
	Model* bossModel = nullptr;
	Model* bossMiniModel = nullptr;
	Model* wallModel = nullptr;
	Model* wallBossModel = nullptr;
	Model* wallFlatModel = nullptr;
	Model* pillarModel = nullptr;
	Model* enemyBulletModel = nullptr;
	Model* roadModel = nullptr;

	ParticleManager* Particle = nullptr;
	//�����蔻��p�ϐ�
	float length = 0.0f;
	float size = 22.0f;
	float wallColliLength = 0.0f;
	//std::unique_ptr<Enemy> enemy(new Enemy());

};
