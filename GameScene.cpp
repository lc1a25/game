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
	enemyModel = Model::LoadFromOBJ("box_aka");
	bossModel = Model::LoadFromOBJ("boss");
	bossMiniModel = Model::LoadFromOBJ("bossMini");
	wallModel = Model::LoadFromOBJ("wallBig");
	wallBossModel = Model::LoadFromOBJ("wallBoss");
	wallFlatModel = Model::LoadFromOBJ("wall");
	pillarModel = Model::LoadFromOBJ("boxGreen");

	player = new Player();
	player->Init(playerModel, bulletModel);

	enemy = new Enemy();
	enemy->Init(enemyModel, { 30.0f, -300.0f, -100.0f });//30,0,100

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { 50.0f, 50.0f, -500.0f });

	enemyCircle = new EnemyCircle();
	enemyCircle->Init(enemyModel, { -50.0f, 50.0f, -200.0f }, false);//-50.0f, 50.0f, 400.0f

	enemyCircle2 = new EnemyCircle();
	enemyCircle2->Init(enemyModel, { 30.0f, -10.0f, -200.0f }, true);

	enemyOneWay = new EnemyOneWay();
	enemyOneWay->Init(enemyModel, { 0.0f,0.0f,-300.0f }, false);

	enemyOneWay2 = new EnemyOneWay();
	enemyOneWay2->Init(enemyModel, { 30.0f,-300.0f,-200.0f }, true);//30.0f,-100.0f,650.0f

	boss = new Boss();
	boss->Init(bossModel,bossMiniModel, { 0,0,-100.0f });

	camera = new Camera();
	camera->Init();

	cameraObj = new CameraObj();
	cameraObj->Init({0,0,-50},{0,0,0});

	wall->SetModel(wallModel);
	wall->scale = { 10,7,70 };
	wall->SetPosition({ 0,-64,50 });

	wallBoss->SetModel(wallModel);
	wallBoss->scale = { 15,7,7 };
	wallBoss->SetPosition({ 0,-64,1000 });

	wallBossBack->SetModel(wallFlatModel);
	wallBossBack->scale = { 92,48,1 };
	wallBossBack->SetPosition({ 0,-65,1045 });
	
			
	pillar->SetModel(pillarModel);
	pillar->scale = { 2,15,1 };
	pillar->SetPosition({ -30,-32,180 });

	pillar2->SetModel(pillarModel);
	pillar2->scale = { 2,20,1 };
	pillar2->SetPosition({ 40,-32,180 });

	pillar3->SetModel(pillarModel);
	pillar3->scale = { 2,20,1 };
	pillar3->SetPosition({ -50,-32,530 });

	pillar4->SetModel(pillarModel);
	pillar4->scale = { 2,18,1 };
	pillar4->SetPosition({ 40,-32,530 });
	
	Object3d::SetCamera(camera);

	EnemyPopLoadData();
	
}

void GameScene::Update()
{
	UpdateEnemyPop();

	//設置物
	wall->Update();
	wallBoss->Update();
	wallBossBack->Update();
	pillar->Update();
	pillar2->Update();
	pillar3->Update();
	pillar4->Update();

	//2dレティクルスプライトの座標
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//デバッグ用
	sprintf_s(moji, "%d", cameraObj->GetRaleIndex());
	//sprintf_s(moji2, "%d", waitRale);
	sprintf_s(moji2, "%0.3f",cameraObj->GetEye().z);

//カメラ
	cameraObj->UpdateCamera();

	camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });
	camera->UpdateCamera();



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

	enemy->SetCameraZ(cameraObj->GetEyeVec().z);
	enemyOneWay->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
	enemyCircle->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
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

	enemyOneWay->SetPlayerPosition(player->GetWorldPosition());
	enemyOneWay->Update();
	enemyOneWay2->Update();

	boss->SetPlayerWorldPos(player->GetWorldPosition());
	boss->Update();

	//当たり判定
	CheckAllCollision(enemyCircle->GetEnemy());
	CheckAllCollision(enemyCircle2->GetEnemy());
	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);
	CheckAllCollision(enemyOneWay->GetEnemy());
	CheckAllCollision(enemyOneWay2->GetEnemy());
	CheckAllCollision(boss->GetEnemy());

	if (boss->GetEnemy()->IsDead() == true)
	{
		bossDieTimer--;
		if (bossDieTimer <= 0)
		{
			pointsLast = true;
			enemyPopCommands.str("");
			enemyPopCommands.clear(std::stringstream::goodbit);
			bossDieTimer = 120;
		}
	}
	
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxcommon->GetCmdlist());
	wallBoss->Draw();
	wallBossBack->Draw();
	wall->Draw();
	pillar->Draw();
	pillar2->Draw();
	pillar3->Draw();
	pillar4->Draw();

	player->Draw();
	
	enemy->Draw();
	//enemyL->Draw();
	enemyCircle->Draw();
	//enemyCircle2->Draw();
	enemyOneWay->Draw();
	//enemyOneWay2->Draw();
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
			if (cameraObj->GetRaleIndex() <= 6)
			{
				enemy->OnCollision();

				bullet->OnCollision();
			}
			if (cameraObj->GetRaleIndex() >= 7)
			{
				enemy->OnBossCollision();

				bullet->OnCollision();
			}
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

void GameScene::CheckPillarCollision()
{
	XMFLOAT3 pos1, pos2;

	pos1 = player->GetWorldPosition();

	pos2 = pillar->GetPosition();

	if (pos1.x <= pos2.x + 16 && pos1.x >= pos2.x &&
		pos1.y <= pos2.y && pos1.y >= pos2.y + 16 &&
		pos1.z <= pos2.z + 16 && pos1.z >= pos2.z)
	{
		coll = 1;
	}
	else
	{
		coll = 0;
	}

}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::EnemyPopLoadData()
{
	//ファイルを開く
	std::fstream file;
	file.open("Resource/enemy.csv");

	//ファイルの内容をコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPop()
{
	//待機処理
	//待機したい時まで　return　してif文より下の処理をさせない
	if (waitCommand == true)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			waitCommand = false;
		}
		return;
	}

	//待機処理
	//待機したい時まで　return　してif文より下の処理をさせない
	if (waitRaleIndexCommand == true)
	{
		if (cameraObj->GetRaleIndex() >= waitRale)
		{
			waitRaleIndexCommand = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line))
	{
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}

		if (word.find("ONEWAY") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int LR = std::atof(word.c_str());
			bool LorR = false;
			if (LR == 1)
			{
				LorR = true;
			}
			

			enemyOneWay = new EnemyOneWay();
			enemyOneWay->Init(enemyModel, { x,y,z }, LorR);
		}
		else if (word.find("CIRCLE") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int LR = std::atof(word.c_str());
			bool LorR = false;

			if (LR == 1)
			{
				LorR = true;
			}

			enemyCircle = new EnemyCircle();
			enemyCircle->Init(enemyModel, { x, y, z }, LorR);
		}
		else if (word.find("OUTWAY") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int LR = std::atof(word.c_str());
			bool LorR = false;

			if (LR == 1)
			{
				LorR = true;
			}

			enemy = new Enemy();
			enemy->Init(enemyModel, { x, y, z });
		}
		else if (word.find("BOSS") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			boss = new Boss();
			boss->Init(bossModel,bulletModel, { x, y, z });
		}
		else if (word.find("RALE") == 0)
		{
			getline(line_stream, word, ',');

			int32_t waitRaleIndex = atoi(word.c_str());

			waitRaleIndexCommand = true;
			waitRale = waitRaleIndex;
			break;//ループから抜けて待機処理に行く
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			waitCommand = true;
			waitTimer = waitTime;

			break;//ループから抜けて待機処理に行く
		}

	}
}
