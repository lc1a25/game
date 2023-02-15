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
#include "Bill.h"

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

	//csv�ǂݍ���
	void EnemyPopLoadData();

	void UpdateEnemyPop();
	
	XMFLOAT3 CommandPositionSet(std::istream &line_stream, std::string &word);

	//�p�[�e�B�N��
	void PlayerCreateParticle(XMFLOAT3 position);

	void EnemyCreateParticle(XMFLOAT3 position);

	void BossCreateParticle(XMFLOAT3 position);

	FLOAT GetbossHpBar() { return bossHpBar; }
	FLOAT GetHpBar() { return hpBar; }

	void BillCreate();
	//�}�E�X���W
	float mouseX;
	float mouseY;

	//�f�o�b�O�e�L�X�g�p
	char moji[64];
	char moji2[64];

	bool pointsLast = false;
	bool bossFlag = false;

	//�R�}���h�p
	bool waitCommand = false;
	int waitTimer = 0;
	//�R�}���h�p
	bool waitRailIndexCommand = false;
	int waitRail = 0;

	//�{�X�n
	float bossHpBar = 733;
	float bossHpBarMax = 733;
	int bossDieTimer = 120;

	//�v���C���[hp
	float hpBar = 288;
	float hpBarMax = 288;
	bool playerDieFlag = false;

	int  coll = 0;

	bool tutorialFlag = true;

	bool tutorialFlag2 = true;

	bool sceneChange = false;

	//csv�@�R�}���h�ǂݍ���
	std::stringstream enemyPopCommands;

private: // �����o�ϐ�

	HWND hwnd;
	//�r���[�|�[�g
	XMMATRIX viewPort;

	DirectXCommon* dxcommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	//�J����
	Camera* camera = nullptr;

	CameraObj* cameraObj = nullptr;//�J�����I�u�W�F�N�g

	//�X�J�C�h�[��
	Object3d* skydome = skydome->Create();

	//�ǌn
	Object3d* wallFloor = wallFloor->Create();
	//��
	Object3d* road = road->Create();

	//�r��
	Object3d* pillar = pillar->Create();
	Object3d* pillar2 = pillar2->Create();
	Object3d* pillar3 = pillar3->Create();
	Object3d* pillar4 = pillar4->Create();
	Object3d* pillar5 = pillar5->Create();

	std::list<std::unique_ptr<Bill>> bills;
	//�v���C���[
	Player* player = nullptr;

	//�G
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
	Model* pillarModel = nullptr;
	Model* enemyBulletModel = nullptr;
	Model* roadModel = nullptr;

	//�p�[�e�B�N��
	ParticleManager* Particle = nullptr;

	//�����蔻��p�ϐ�
	float length = 0.0f;
	float size = 22.0f;
	float wallColliLength = 0.0f;

	//����y
	float floorY = -55.0f;

};
