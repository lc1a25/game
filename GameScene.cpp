#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxcommon = dxCommon;
	this->input = input;
	this->audio = audio;

	playerModel = Model::LoadFromOBJ("box");
	bulletModel = Model::LoadFromOBJ("ene-0");
	enemyModel = Model::LoadFromOBJ("box");

	player = new Player();
	player->Init(playerModel, bulletModel);

	enemy = new Enemy();
	enemy->Init(enemyModel,{30,0,100},true);

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { -30,0,100 }, false);
	//camera->cameraObj->Init();
	camera = new Camera();
	camera->Init();

	cameraObj = new CameraObj();
	cameraObj->Init({0,0,-50},{0,0,0});

	Object3d::SetCamera(camera);
	
}

void GameScene::Update()
{


	//���e�B�N���̂���
	player->SetHwnd(hwnd);
	player->SetViewPort(viewPort);
	player->SetCameraMatViewProjection(camera->GetMatViewProjection());

	//2d���e�B�N���X�v���C�g�̍��W
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//�f�o�b�O�p
	sprintf_s(moji, "%f",player->GetReticleWorldPosition().z);

	cameraObj->UpdateCamera();

	camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });
	camera->UpdateCamera();

	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);

	enemy->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemy->GetWorldPosition());
	enemy->Update();

	enemyL->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemyL->GetWorldPosition());
	enemyL->Update();

	player->SetEnemyFlag(enemyL->IsDead());
	player->Update();
	
	
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxcommon->GetCmdlist());
	player->Draw();
	//enemy->Draw();
	enemyL->Draw();
	Object3d::PostDraw();
}

void GameScene::CheckAllCollision(Enemy* enemy)
{
	XMFLOAT3 pos1, pos2;

	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//�G�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

	pos1 = player->GetWorldPosition();
	//���L�����ƓG�e�����蔻��
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		pos2 = bullet->GetWorldPosition();
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
				 ((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
				 ((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (length <= size)
		{
			player->OnCollision();

			bullet->OnCollision();
		}
	}

	pos1 = enemy->GetWorldPosition();
	//���e�ƓG�����蔻��
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
			((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
			((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (length <= size + 25)
		{
			enemy->OnCollision();

			bullet->OnCollision();
		}
	}

	
	//���e�ƓG�e�����蔻��
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
	{
	
		pos1 = playerBullet->GetWorldPosition();
		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
		{
			pos2 = enemyBullet->GetWorldPosition();

			length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
				((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
				((pos2.z - pos1.z) * (pos2.z - pos1.z));
			if (length <= size + 3)
			{
				/*enemy->OnCollision();

				playerBullet->OnCollision();
				enemyBullet->OnCollision();*/
			}
		}
	}
}
