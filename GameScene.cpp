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
	enemy->Init(enemyModel,{ 30.0f, 0.0f, 150.0f },true);//30,0,100

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { -30.0f, 0.0f, 150.0f }, false);

	enemyCircle = new EnemyCircle();
	enemyCircle->Init(enemyModel, { -80.0f, 0.0f, 150.0f }, false);

	enemyOneWay = new EnemyOneWay();
	enemyOneWay->Init(enemyModel, { 100.0f,20.0f,150.0f }, true);

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
	sprintf_s(moji, "%f", cameraObj->GetTarget().x);
	sprintf_s(moji2, "%f", player->GetWorldPosition().x);

//�J����
	cameraObj->UpdateCamera();

	/*camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });*/
	camera->UpdateCamera();

//�����蔻��
	CheckAllCollision(enemyCircle->GetEnemy());
	CheckAllCollision(enemyL);

//�G
	/*enemy->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemy->GetWorldPosition());
	enemy->Update();

	enemyL->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemyL->GetWorldPosition());
	enemyL->Update();*/
	
	enemyCircle->SetPlayerPosition(player->GetWorldPosition());
	enemyCircle->Update();

	enemyOneWay->Update();

//���@
	player->SetEnemyFlag(enemyL->IsDead());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->Update();
	
	
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxcommon->GetCmdlist());

	player->Draw();

	enemyCircle->Draw();
	enemyOneWay->Draw();

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
