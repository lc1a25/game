#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxcommon = dxCommon;
	this->input = input;
	this->audio = audio;

	playerModel = Model::LoadFromOBJ("boxplayer-0");
	bulletModel = Model::LoadFromOBJ("ene-0");
	enemyModel = Model::LoadFromOBJ("box");
	bossModel = Model::LoadFromOBJ("boss");

	player = new Player();
	player->Init(playerModel, bulletModel);

	enemy = new Enemy();
	enemy->Init(enemyModel, { 30.0f, -80.0f, 700.0f });//30,0,100

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { 50.0f, 50.0f, 500.0f });

	enemyCircle = new EnemyCircle();
	enemyCircle->Init(enemyModel, { -50.0f, 50.0f, 400.0f }, false);

	enemyCircle2 = new EnemyCircle();
	enemyCircle2->Init(enemyModel, { 30.0f, -10.0f, 200.0f }, true);

	enemyOneWay = new EnemyOneWay();
	enemyOneWay->Init(enemyModel, { -150.0f,0.0f,300.0f }, false);

	enemyOneWay2 = new EnemyOneWay();
	enemyOneWay2->Init(enemyModel, { 30.0f,-100.0f,650.0f }, true);

	boss = new Boss();
	boss->Init(bossModel, { 0,0,950.0f });

	camera = new Camera();
	camera->Init();

	cameraObj = new CameraObj();
	cameraObj->Init({0,0,-50},{0,0,0});

	Object3d::SetCamera(camera);
	
}

void GameScene::Update()
{
	//2dレティクルスプライトの座標
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//デバッグ用
	sprintf_s(moji, "%0.3f",player->GetWorldPosition().x);
	sprintf_s(moji2, "%0.3f", cameraObj->GetTimerate());

//カメラ
	cameraObj->UpdateCamera();

	camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });
	camera->UpdateCamera();

//当たり判定
	CheckAllCollision(enemyCircle->GetEnemy());
	CheckAllCollision(enemyCircle2->GetEnemy());
	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);
	CheckAllCollision(enemyOneWay->GetEnemy());
	CheckAllCollision(enemyOneWay2->GetEnemy());

//レティクルのため
	player->SetHwnd(hwnd);
	player->SetViewPort(viewPort);
	player->SetCameraMatViewProjection(cameraObj->GetMatViewProjection());

//自機
	player->SetEnemyFlag(enemyL->IsDead());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraTargetVec(cameraObj->GetTargetVec());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->Update();

	enemy->SetCameraZ(cameraObj->GetEye().z);

// 敵
	enemy->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemy->GetWorldPosition());
	enemy->Update();

	enemyL->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemyL->GetWorldPosition());
	enemyL->Update();

	enemyCircle->SetPlayerPosition(player->GetWorldPosition());
	enemyCircle->Update();

	enemyCircle2->SetPlayerPosition(player->GetWorldPosition());
	enemyCircle2->Update();

	enemyOneWay->Update();
	enemyOneWay2->Update();

	boss->Update();

	//pointsLast = cameraObj->GetEndFlag();
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxcommon->GetCmdlist());

	player->Draw();
	
	enemy->Draw();
	enemyL->Draw();
	enemyCircle->Draw();
	enemyCircle2->Draw();
	enemyOneWay->Draw();
	enemyOneWay2->Draw();
	boss->Draw();

	Object3d::PostDraw();
}

void GameScene::CheckAllCollision(Enemy* enemy)
{
	XMFLOAT3 pos1, pos2;

	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//敵弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

	pos1 = player->GetWorldPosition();
	//自キャラと敵弾当たり判定
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
	//自弾と敵当たり判定
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

	
	//自弾と敵弾当たり判定
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
