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

	//モデル読み込み
	/*playerModel = Model::LoadFromOBJ("boxplayer-0");*/
	playerModel = Model::LoadFromOBJ("zikistar");
	bulletModel = Model::LoadFromOBJ("ene-0");
	enemyModel = Model::LoadFromOBJ("oneWay");
	enemyRotateModel = Model::LoadFromOBJ("rotate");
	bossModel = Model::LoadFromOBJ("boss");
	bossMiniModel = Model::LoadFromOBJ("bossMini");
	wallModel = Model::LoadFromOBJ("wallBig");
	wallBossModel = Model::LoadFromOBJ("wallBoss");
	wallFlatModel = Model::LoadFromOBJ("wallFlat");
	pillarModel = Model::LoadFromOBJ("bill");
	enemyBulletModel = Model::LoadFromOBJ("box_aka");
	roadModel = Model::LoadFromOBJ("road");


	bossHpBar = 733;
	bossHpBarMax = 733;
	bossDieTimer = 120;

	pointsLast = false; 

	bossFlag = false;

	hpBar = 288;
	hpBarMax = 288;
	//プレイヤーが倒された用
	hp0 = false;

	//オブジェクトの初期化
	player = new Player();
	player->Init(playerModel, bulletModel);

	enemy = new Enemy();
	enemy->Init(enemyModel, { 30.0f, -300.0f, -100.0f }, enemyModel);//30,0,100

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { 50.0f, 50.0f, -500.0f }, enemyModel);

	enemyCircle = new EnemyCircle();
	enemyCircle->Init(enemyRotateModel, { -50.0f, 50.0f, -200.0f }, false);//-50.0f, 50.0f, 400.0f

	enemyCircle2 = new EnemyCircle();
	enemyCircle2->Init(enemyRotateModel, { 30.0f, -10.0f, -200.0f }, true);

	enemyOneWay = new EnemyOneWay();
	enemyOneWay->Init(enemyModel, { 0.0f,0.0f,-300.0f }, false);

	enemyOneWay2 = new EnemyOneWay();
	enemyOneWay2->Init(enemyModel, { 30.0f,-300.0f,-200.0f }, true);//30.0f,-100.0f,650.0f

	boss = new Boss();
	boss->Init(bossModel, enemyBulletModel, { 0,0,-100.0f });

	bossChildLUF = new BossChild();
	bossChildLUF->Init(bossModel,{ 0, 0, -100.0f}, 1);
	bossChildLUB = new BossChild();
	bossChildLUB->Init(bossModel, { 0, 0, -100.0f }, 2);
	bossChildRUF = new BossChild();
	bossChildRUF->Init(bossModel, { 0, 0, -100.0f }, 3);
	bossChildRUB = new BossChild();
	bossChildRUB->Init(bossModel, { 0, 0, -100.0f }, 4);
	bossChildLDF = new BossChild();
	bossChildLDF->Init(bossModel, { 0, 0, -100.0f }, 5);
	bossChildLDB = new BossChild();
	bossChildLDB->Init(bossModel, { 0, 0, -100.0f }, 6);
	bossChildRDF = new BossChild();
	bossChildRDF->Init(bossModel, { 0, 0, -100.0f }, 7);
	bossChildRDB = new BossChild();
	bossChildRDB->Init(bossModel, { 0, 0, -100.0f }, 8);


	camera = new Camera();
	camera->Init();

	cameraObj = new CameraObj();
	cameraObj->Init({0,0,-50},{0,0,0});

	//wall->SetModel(wallModel);
	//wall->scale = { 10,7,70 };
	//wall->SetPosition({ 0,-64,50 });

	//wall2->SetModel(wallModel);
	//wall2->scale = { 10,7,70 };
	//wall2->SetPosition({ 0,110,50 });//0,110,50

	//wallBoss->SetModel(wallModel);
	//wallBoss->scale = { 15,7,7 };
	//wallBoss->SetPosition({ 0,-64,1000 });

	wallBossBack->SetModel(wallFlatModel);
	wallBossBack->scale = { 50,7,170 };
	wallBossBack->SetPosition({ 0,-65,50 });

	road->SetModel(roadModel);
	road->scale = { 10,7,250 };
	road->SetPosition({ 0,-55,650 });
	
			
	pillar->SetModel(pillarModel);
	pillar->scale = { 4,13,1 };
	pillar->SetPosition({ -30,-32,280 });

	pillar2->SetModel(pillarModel);
	pillar2->scale = { 4,20,1 };
	pillar2->SetPosition({ 40,-32,280 });

	pillar3->SetModel(pillarModel);
	pillar3->scale = { 4,20,1 };
	pillar3->SetPosition({ -50,-32,480 });

	pillar4->SetModel(pillarModel);
	pillar4->scale = { 4,18,1 };
	pillar4->SetPosition({ 40,-32,480 });

	pillar5->SetModel(pillarModel);
	pillar5->scale = { 4,18,1 };
	pillar5->SetPosition({ -50,-32,680 });
	
	Object3d::SetCamera(camera);

	EnemyPopLoadData();

	tutorialFlag2 = false;

	// 3Dオブジェクト生成
	Particle = ParticleManager::Create(dxcommon->GetDev(), camera);
	
}

void GameScene::Update()
{
	//
	UpdateEnemyPop();

	if (cameraObj->GetRaleIndex() <= 1)
	{
		if (enemyOneWay->GetEnemy()->GetEnemyDownCount() == 1)
		{
			tutorialFlag = false;
		}
	}

	cameraObj->SetTutorialFlag(tutorialFlag);

	

	Particle->Update();
	
	//設置物
	/*wall->Update();
	wall2->Update();
	wallBoss->Update();*/
	wallBossBack->Update();
	road->Update();

	pillar->Update();
	pillar2->Update();
	pillar3->Update();
	pillar4->Update();
	pillar5->Update();

	//2dレティクルスプライトの座標
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//デバッグ用
	sprintf_s(moji, "%d", cameraObj->GetRaleIndex());
	//sprintf_s(moji2, "%f", pos.z);
	//sprintf_s(moji2, "%0.3f", bossChildRUF->GetEnemy()->angle);
	//sprintf_s(moji, "%0.3f", bossChildLUF->GetEnemy()->angle);

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
	player->SetPlayerHpBar(hpBar);
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
	boss->GetEnemy()->SetBossHpBar(bossHpBar,bossHpBarMax);
	boss->Update();


	bossChildLUF->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildLUF->SetBossVec(boss->GetBossVec());
	bossChildLUF->Update();

	bossChildLUB->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildLUB->SetBossVec(boss->GetBossVec());
	bossChildLUB->Update();

	bossChildRUF->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildRUF->SetBossVec(boss->GetBossVec());
	bossChildRUF->Update();

	bossChildRUB->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildRUB->SetBossVec(boss->GetBossVec());
	bossChildRUB->Update();

	bossChildLDF->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildLDF->SetBossVec(boss->GetBossVec());
	bossChildLDF->Update();

	bossChildLDB->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildLDB->SetBossVec(boss->GetBossVec());
	bossChildLDB->Update();

	bossChildRDF->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildRDF->SetBossVec(boss->GetBossVec());
	bossChildRDF->Update();

	bossChildRDB->SetBossPos(boss->GetEnemy()->GetWorldPosition());
	bossChildRDB->SetBossVec(boss->GetBossVec());
	bossChildRDB->Update();

	bossChildLUF->SetChildShotRange(cameraObj->GetEye());
	bossChildLUB->SetChildShotRange(cameraObj->GetEye());
	bossChildRUF->SetChildShotRange(cameraObj->GetEye());
	bossChildRUB->SetChildShotRange(cameraObj->GetEye());
	bossChildLDF->SetChildShotRange(cameraObj->GetEye());
	bossChildLDB->SetChildShotRange(cameraObj->GetEye());
	bossChildRDF->SetChildShotRange(cameraObj->GetEye());
	bossChildRDB->SetChildShotRange(cameraObj->GetEye());



	//当たり判定
	CheckAllCollision(enemyCircle->GetEnemy());
	CheckAllCollision(enemyCircle2->GetEnemy());
	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);
	CheckAllCollision(enemyOneWay->GetEnemy());
	CheckAllCollision(enemyOneWay2->GetEnemy());
	CheckAllCollision(boss->GetEnemy());
	CheckBossANDChildCollision(bossChildLUF->GetEnemy());
	CheckBossANDChildCollision(bossChildLUB->GetEnemy());
	CheckBossANDChildCollision(bossChildRUF->GetEnemy());
	CheckBossANDChildCollision(bossChildRUB->GetEnemy());
	CheckBossANDChildCollision(bossChildLDF->GetEnemy());
	CheckBossANDChildCollision(bossChildLDB->GetEnemy());
	CheckBossANDChildCollision(bossChildRDF->GetEnemy());
	CheckBossANDChildCollision(bossChildRDB->GetEnemy());

	//hp
	bossHpBar = boss->GetEnemy()->GetHpBarX();
	hpBar = player->GetHpBar();

	//おわり
	if (cameraObj->GetEndFlag() == true)
	{
		bossFlag = true;
	}
	//ボスが倒されたら
	if (boss->GetEnemy()->IsDead() == true)
	{
		enemyPopCommands.str("");
		enemyPopCommands.clear(std::stringstream::goodbit);
		bossDieTimer--;
		if (bossDieTimer <= 0)
		{
			pointsLast = true;
			
			bossDieTimer = 120;
		}
	}

	//プレイヤーのhpが０になったら
	if (player->GetHp0() == true)
	{
		enemyPopCommands.str("");
		enemyPopCommands.clear(std::stringstream::goodbit);
		//EnemyPopLoadData();
		
		//シーンチェンジ
		hp0 = true;
	}
}

void GameScene::Draw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	//wallBoss->Draw();
	wallBossBack->Draw();
	 //wall->Draw();
	road->Draw();
	//wall2->Draw();
	pillar->Draw();
	pillar2->Draw();
	pillar3->Draw();
	pillar4->Draw();
	pillar5->Draw();
	 
	player->Draw();
	
	enemy->Draw();
	//enemyL->Draw();
	enemyCircle->Draw();
	//enemyCircle2->Draw();
	enemyOneWay->Draw();
	//enemyOneWay2->Draw();


	
	if(cameraObj->GetEndFlag() == true)
	{ 
		boss->Draw();
		bossChildLUF->Draw();
		bossChildLUB->Draw();
		bossChildRUF->Draw();
		bossChildRUB->Draw();
		bossChildLDF->Draw();
		bossChildLDB->Draw();
		bossChildRDF->Draw();
		bossChildRDB->Draw();
	}
	
	
	
	Particle->Draw(dxcommon->GetCmdlist());
	
	Object3d::PostDraw();
// 3Dオブクジェクトの描画おわり
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
			if (boss->GetEnemy()->IsDead() == false)
			{
				player->OnCollision();
			}

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
		if (cameraObj->GetRaleIndex() >= 7)
		{
			length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
				((pos2.y - pos1.y - 8) * (pos2.y - pos1.y - 8)) +
				((pos2.z - pos1.z) * (pos2.z - pos1.z));
		}
		if (length <= size)
		{
			if (cameraObj->GetRaleIndex() <= 4)
			{
				enemy->OnCollision();

				bullet->OnCollision();
				//パーティクル
				for (int i = 0; i < 100; i++)
				{

					const float rnd_pos = 10.0f;
					XMFLOAT3 pos{};

				
					pos.x = enemy->GetWorldPosition().x;
					pos.y = enemy->GetWorldPosition().y;
					pos.z = enemy->GetWorldPosition().z;
					

					const float rnd_vel = 0.1f;
					XMFLOAT3 vel{};
					vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
					vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
					vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

					XMFLOAT3 acc{};
					const float rnd_acc = 0.011f;
					acc.y = -(float)rand() / RAND_MAX * rnd_acc;

					//Add(5, pos, vel, acc);
					Particle->Add(64, pos, vel, acc, 15.0f, 0.0f,{1,1,1},{1,0.5,0});
				}
			}
			if (cameraObj->GetRaleIndex() >= 5)
			{
				enemy->OnBossCollision();

				bullet->OnCollision();
			}
			
		}
		else
		{
			enemy->bossDamage = false;
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

void GameScene::CheckBossANDChildCollision(Enemy* bossChild)
{
	XMFLOAT3 pos1, pos2;

	//敵弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bossChild->GetBullets();

	pos1 = boss->GetEnemy()->GetWorldPosition();
	//自キャラと敵弾当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		pos2 = bullet->GetWorldPosition();
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
			((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
			((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (length <= size + 25)
		{
			//player->OnCollision();

			bullet->OnCollision();
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
			

			getline(line_stream, word, ',');
			//0が攻撃　1が攻撃しない
			int attack = std::atof(word.c_str());
			bool attackFlag = true;
			if (attack == 1)
			{
				attackFlag = false;
			}

			enemyOneWay = new EnemyOneWay();
			enemyOneWay->Init(enemyModel, { x,y,z }, LorR,attackFlag);
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
			enemyCircle->Init(enemyRotateModel, { x, y, z }, LorR);
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
			enemy->Init(enemyModel, { x, y, z }, enemyModel);
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
			boss->Init(bossModel, enemyBulletModel, { x, y, z });
		}
		else if (word.find("CHILDLUF") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLUF = new BossChild();
			bossChildLUF->Init(bossModel,  { x, y, z },number);
		}
		else if (word.find("CHILDLUB") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLUB = new BossChild();
			bossChildLUB->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDRUF") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRUF = new BossChild();
			bossChildRUF->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDRUB") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRUB = new BossChild();
			bossChildRUB->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDLDF") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLDF = new BossChild();
			bossChildLDF->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDLDB") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLDB = new BossChild();
			bossChildLDB->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDRDF") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRDF = new BossChild();
			bossChildRDF->Init(bossModel, { x, y, z }, number);
		}
		else if (word.find("CHILDRDB") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRDB = new BossChild();
			bossChildRDB->Init(bossModel, { x, y, z }, number);
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
