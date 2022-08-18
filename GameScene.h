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
#include "CameraObj.h"

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

	//hwnd�̎擾
	void SetHwnd(HWND winHwnd) { hwnd = winHwnd; }

	//�r���[�|�[�g�s��̎擾
	void SetViewPort(XMMATRIX winViewPort) { viewPort = winViewPort; }


	float mouseX;
	float mouseY;
	char moji[64];
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

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* enemyL = nullptr;
	EnemyCircle* enemyCircle = nullptr;

	XMVECTOR reticlePos;//���e�B�N���p�ϐ�

	//���f��
	Model* playerModel = nullptr;
	Model* bulletModel = nullptr;
	Model* enemyModel = nullptr;


	//�����蔻��p�ϐ�
	float length = 0.0f;
	float size = 2.0f;
	//std::unique_ptr<Enemy> enemy(new Enemy());

};
