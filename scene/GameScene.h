#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "Matrix4.h"
#include "Win.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyCircle.h"
#include "EnemyOneWay.h"
#include "CameraObj.h"
#include "Boss.h"
#include "BossChild.h"
#include "ParticleManager.h"
#include <sstream>
#include "BillManager.h"
#include "Bill.h"
#include "Light.h"
#include "LightGroup.h"

enum class Scene
{
	Title,
	Game,
	Clear,
	GameOver,
};
enum class SceneDraw
{
	TitleDraw,
	GameDraw,
	ClearDraw,
	GameOverDraw,
};

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
	void Init(DirectXCommon* dxCommon, Input* input, Audio* audio, Win* win);

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
	void CheckTargetCollision();
	void CheckBillCollision(BillManager* bill);
	//hwnd�̎擾
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//�r���[�|�[�g�s��̎擾
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }

	//csv�ǂݍ���
	void EnemyPopLoadData();

	void UpdateEnemyPop();

	XMFLOAT3 CommandPositionSet(std::istream& line_stream, std::string& word);

	void SetGameFlag(int gameFlag) { this->gameScene = gameFlag; }

	void Title();
	void Game();
	void Clear();
	void GameOver();
	void TitleDraw();
	void GameDraw();
	void ClearDraw();
	void GameOverDraw();

	//�}�E�X���W
	float mouseX;
	float mouseY;

	//�f�o�b�O�e�L�X�g�p
	char moji[64];
	char moji2[64];

	bool pointsLast = false;
	bool bossFlag = false;

	bool playerDieFlag = false;
	//�R�}���h�p
	bool waitCommand = false;
	int waitTimer = 0;

	//�R�}���h�p
	bool waitRailIndexCommand = false;
	int waitRail = 0;

	bool tutorialFlag = true;

	bool sceneChange = false;

	//csv�@�R�}���h�ǂݍ���
	std::stringstream enemyPopCommands;

	Light* light = nullptr;
	LightGroup* lightGroup = nullptr;

private: // �����o�ϐ�
	int phaseNumber = static_cast<int>(scene_);
	static void (GameScene::* spFuncTable[])();
	int phaseDrawNumber = static_cast<int>(sceneDraw_);
	static void (GameScene::* spFuncTableDraw[])();
	Scene scene_ = Scene::Title;
	SceneDraw sceneDraw_ = SceneDraw::TitleDraw;
	HWND hwnd;
	//�r���[�|�[�g
	XMMATRIX viewPort;

	DirectXCommon* dxcommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Win* win = nullptr;

	//�J����
	Camera* camera = nullptr;

	CameraObj* cameraObj = nullptr;//�J�����I�u�W�F�N�g

	//�X�J�C�h�[��
	Object3d* skydome = skydome->Create();

	//�ǌn
	Object3d* wallFloor = wallFloor->Create();
	//��
	Object3d* road = road->Create();

	Object3d* titleObj = titleObj->Create();

	BillManager* billM = nullptr;

	std::list<std::unique_ptr<EnemyOneWay>> oneWays;
	std::list<std::unique_ptr<EnemyOneWay>> oneWayMovies;
	std::list<std::unique_ptr<EnemyCircle>> circles;

	Object3d* shotObj = shotObj->Create();
	Object3d* shotHibiObj = shotHibiObj->Create();
	Object3d* targetObj = targetObj->Create();
	Object3d* targetObj2 = targetObj2->Create();
	Object3d* targetObj3 = targetObj3->Create();
	Object3d* clickObj = clickObj->Create();
	Object3d* clearObj = clearObj->Create();

	Object3d* kanbanObj = kanbanObj->Create();
	Object3d* kanbanPlaneObj = kanbanPlaneObj->Create();
	Object3d* kanbanShotObj = kanbanShotObj->Create();
	Object3d* kanbanShot2Obj = kanbanShot2Obj->Create();
	Object3d* kanbanShot3Obj = kanbanShot3Obj->Create();
	Object3d* kanbanShot4Obj = kanbanShot4Obj->Create();

	//�v���C���[
	Player* player = nullptr;
	Object3d* startPlayer = startPlayer->Create();

	//�G
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
	Model* billModel = nullptr;
	Model* enemyBulletModel = nullptr;
	Model* roadModel = nullptr;
	Model* shotObjModel = nullptr;
	Model* shotHibiObjModel = nullptr;
	Model* kanbanModel = nullptr;
	Model* kanbanPlaneModel = nullptr;
	Model* kanbanShotModel = nullptr;
	Model* kanbanShot2Model = nullptr;
	Model* kanbanShot3Model = nullptr;
	Model* kanbanShot4Model = nullptr;
	Model* barrierModel = nullptr;
	Model* targetModel = nullptr;
	Model* clickModel = nullptr;
	Model* clearModel = nullptr;
	Model* deadModel = nullptr;
	

	//�p�[�e�B�N��
	ParticleManager* Particle = nullptr;

	float particleCountTitle = 30;
	float particleLifeTitle = 64;
	XMFLOAT3 particleLeftPosTitle = { -movieParticleXL, 0,-320 };
	XMFLOAT3 particleVelocityTitle = { 0.5,0.5,0.5 };
	XMFLOAT3 particleVelocityClear = { 3.5,4.0,0.8 };
	XMFLOAT3 particleAccelTitle = { 0.02,0.02,0.02 };
	XMFLOAT3 particleAccelClear = { 0.1,0.05,0.1 };
	float particleMaxPosXTitle = 80;
	float particleMinPosXTitle = 40;

	//�����蔻��p�ϐ�
	float length = 0.0f;
	float size = 25.0f;
	float wallColliLength = 0.0f;
	float startPlayerAddZ = 4.0f;
	float startPlayerAddY = 0.3f;

	XMFLOAT3 startPlayerAfterPos = { 0,-200,610 };
	float endPlayerPos = 1400.0f;

	//����y
	float floorY = -55.0f;

	//skydome 
	XMFLOAT3 skydomePos = { 0,-220,0 };
	float skydomeZ = 0;
	float skydomeVec = 0.8;

	//�Ŕ̃A�j���[�V����
	int kanbanTime = 0;
	int kanbanTimeMax = 30;
	XMFLOAT3 kanbanShotPos = { 90,-30,115 };
	XMFLOAT3 kanbanShotPosDown = { 90,-90,115 };

    //���G����
	bool mutekiFlagDeb = false;
	
	bool mutekiFlag = false;
	int mutekiCoolTimeMax = 420;
	int mutekiCoolTimeMin = 0;
	int mutekiCoolTime = mutekiCoolTimeMax;

	int tenmetuCountReset = 0;//���G���Ԓ��ɓ_�ł����邽��
	int tenmetuCount = tenmetuCountReset;//���G���Ԓ��ɓ_�ł����邽��
	int tenmetuAliveCount = 10;//���G���Ԓ��ɃL�������o������
	int tenmetuDeadCount = 20;//���G���Ԓ��ɃL�������o���Ȃ�����

	int clickCountReset = 0;//�_�ł����邽��
	int clickCount = clickCountReset;//�_�ł����邽��
	int clickAliveCount = 15;//�L�������o������
	int clickDeadCount = 30;//�L�������o���Ȃ�����

	bool movieSkipFlag = false;//���[�r�[�X�L�b�v�p
	bool setObjectFlag = false;//���[�r�[��ɃI�u�W�F�N�g���Z�b�g����p

	//�`���[�g���A�������̂���
	int targetHpMax = 3;
	int targetHp = targetHpMax;
	int targetChangeHp = targetHp - 1;
	int count = 0;

	int gameScene = 0;//�X�v���C�g�̃Q�[���V�[��
	int gameSceneTitle = 0;//�X�v���C�g�̃^�C�g���V�[��
	int gameSceneInGame = 1;//�X�v���C�g�̃C���Q�[���V�[��
	int gameSceneClear = 2;//�X�v���C�g�̃N���A�V�[��
	int gameSceneGameOver = 3;//�X�v���C�g�̃Q�[���I�[�o�[�V�[��

	//�{�X�n
	float bossHpBar = 733;
	float bossHpBarMax = 733;

	//�v���C���[hp
	float hpBar = 288;
	float hpBarMax = 288;


	int dieTimerMax = 120;
	int dieTimer = dieTimerMax;
	int  coll = 0;
	
	float shotObjAddy = 10;//shotObj�̓����蔻�蒲���p
	XMFLOAT3 bullet1;//�v���C���[�̒e�̍��W�������p

	int movieParticleTime = 0;
	float movieParticleXL = 80;
	float movieParticleXR = 40;
	bool attackParticleFlag = false;//�v���C���[�̒e�̃p�[�e�B�N�����ł�Ƃ��Ɏg��flag
	bool gameStartFlag = false;//�^�C�g������Q�[���V�[���ɓ��������Ƃ��킩��flag

	int randBill = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(scale�p)
	float billScaleY = 13;//�r���̏c�����̃X�P�[��
	int randBillRot = 0;//�r���������_���z�u����Ƃ���rand�̐��l�������(rotation�p)
	XMFLOAT3 billRotation;//�r���̃��[�e�[�V����

	//�X�v���C�g
	Sprite* reticleSprite;//�Ə�
	Sprite* bossHpWakuSprite;
	Sprite* bossHpBarSprite;
	Sprite* playerHpSprite;
	Sprite* kSkipSprite;

	DebugText* debugtext_minute = nullptr;
	DebugText* debugtext_minute2 = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	void ParticleClear();
	void ParticleTitle();
};

