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
	skydome_model = Model::LoadFromOBJ("skydome");
	playerModel = Model::LoadFromOBJ("zikistar");
	bulletModel = Model::LoadFromOBJ("ene-0");
	enemyModel = Model::LoadFromOBJ("oneWay");
	enemyRotateModel = Model::LoadFromOBJ("rotate");
	bossModel = Model::LoadFromOBJ("boss");
	bossMiniModel = Model::LoadFromOBJ("bossMini");
	wallModel = Model::LoadFromOBJ("wallBig");
	wallBossModel = Model::LoadFromOBJ("wallBoss");
	wallFlatModel = Model::LoadFromOBJ("wallFlatGray");
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
	playerDieFlag = false;

	//オブジェクトの初期化

	skydome->SetModel(skydome_model);
	skydome->scale = { 11,11,15 };
	skydome->SetPosition({ 0,0,0 });

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

	wallFloor->SetModel(wallFlatModel);
	wallFloor->scale = { 50,7,170 };
	wallFloor->SetPosition({ 0,-65,50 });

	road->SetModel(roadModel);
	road->scale = { 10,7,300 };
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

	tutorialFlag = true;

	// 3Dオブジェクト生成
	Particle = ParticleManager::Create(dxcommon->GetDev(), camera);
	
}

void GameScene::Update()
{
	//デバッグ用playerhp-
	if (input->isKeyTrigger(DIK_P))
	{
		player->OnCollision();
	}

	//カメラの後ろ行ったら消える
	bills.remove_if([](std::unique_ptr<Bill>& bill)
	{
			return bill->billDead();
	});
	
	//ビル生成
	BillCreate();
	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->SetCameraZ(cameraObj->GetEye().z);
		bill->Update();
	}

	//csvのenemy発生
	UpdateEnemyPop();

	if (cameraObj->GetRaleIndex() <= 1)
	{
		if (enemyOneWay->GetEnemy()->GetEnemyDownCount() == 1)
		{
			tutorialFlag = false;
			sceneChange = true;	
		}
	}

	cameraObj->SetTutorialFlag(tutorialFlag);

	Particle->Update();

	//スカイドーム
	skydome->Update();

	wallFloor->Update();
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

	//ボス
	boss->SetPlayerWorldPos(player->GetWorldPosition());
	boss->GetEnemy()->SetBossHpBar(bossHpBar,bossHpBarMax);
	boss->Update();


	bossChildLUF->SetBossPos(boss->GetPos());
	bossChildLUF->SetBossVec(boss->GetBossVec());
	bossChildLUF->SetBossDead(boss->GetBossDead());
	bossChildLUF->Update();

	bossChildLUB->SetBossPos(boss->GetPos());
	bossChildLUB->SetBossVec(boss->GetBossVec());
	bossChildLUB->SetBossDead(boss->GetBossDead());
	bossChildLUB->Update();

	bossChildRUF->SetBossPos(boss->GetPos());
	bossChildRUF->SetBossVec(boss->GetBossVec());
	bossChildRUF->SetBossDead(boss->GetBossDead());
	bossChildRUF->Update();

	bossChildRUB->SetBossPos(boss->GetPos());
	bossChildRUB->SetBossVec(boss->GetBossVec());
	bossChildRUB->SetBossDead(boss->GetBossDead());
	bossChildRUB->Update();

	bossChildLDF->SetBossPos(boss->GetPos());
	bossChildLDF->SetBossVec(boss->GetBossVec());
	bossChildLDF->SetBossDead(boss->GetBossDead());
	bossChildLDF->Update();

	bossChildLDB->SetBossPos(boss->GetPos());
	bossChildLDB->SetBossVec(boss->GetBossVec());
	bossChildLDB->SetBossDead(boss->GetBossDead());
	bossChildLDB->Update();

	bossChildRDF->SetBossPos(boss->GetPos());
	bossChildRDF->SetBossVec(boss->GetBossVec());
	bossChildRDF->SetBossDead(boss->GetBossDead());
	bossChildRDF->Update();

	bossChildRDB->SetBossPos(boss->GetPos());
	bossChildRDB->SetBossVec(boss->GetBossVec());
	bossChildRDB->SetBossDead(boss->GetBossDead());
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

		if (boss->GetEnemy()->GetWorldPosition().y <= floorY)
		{
			//bossのやられた演出まち用
			bossDieTimer--;
			if (bossDieTimer <= 0)
			{
				//シーンチェンジ
				pointsLast = true;
			
				bossDieTimer = 120;
			}
		}
		else
		{
			BossCreateParticle(boss->GetPos());
		}
		
	}

	//カメラの注視点セット
	cameraObj->SetPlayerDieFlag(player->GetHp0());
	cameraObj->SetTarget(player->GetWorldPosition());
	cameraObj->SetBoss(boss->GetPos(), boss->GetBossDead());
	//プレイヤーのhpが０になったら
	if (player->GetHp0() == true)
	{
		
		//パーティクル生成
		PlayerCreateParticle(player->GetWorldPosition());
		
		//bossのやられた演出まち用
		bossDieTimer--;

		if (bossDieTimer <= 0)
		{
			//シーンチェンジ
			playerDieFlag = true;

			bossDieTimer = 120;
		}
		
	}
}

void GameScene::Draw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	
	//スカイドーム
	skydome->Draw();

	wallFloor->Draw();
	road->Draw();
	pillar->Draw();
	pillar2->Draw();
	pillar3->Draw();
	pillar4->Draw();
	pillar5->Draw();
	 
	/*for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Draw();
	}*/

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
				//player->OnCollision();
				
			}

			bullet->OnCollision();
		}
	}

	pos1 = enemy->GetWorldPosition();
	//自弾と敵当たり判定
	if (input->isKeyTrigger(DIK_Q))
	{
		enemy->OnBossCollision();
	}
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
			((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
			((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (cameraObj->GetRaleIndex() >= 7)
		{
			length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
				((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
				((pos2.z - pos1.z) * (pos2.z - pos1.z));
		}
		if (length <= size)
		{
			if (cameraObj->GetRaleIndex() <= 4)
			{
				enemy->OnCollision();

				bullet->OnCollision();
				//パーティクル生成
				EnemyCreateParticle(enemy->GetWorldPosition());
			}
			if (cameraObj->GetRaleIndex() >= 5)
			{
				
				enemy->OnBossCollision();

				if (boss->GetEnemy()->IsDead() == true)
				{
					BossCreateParticle(enemy->GetWorldPosition());
				}
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
	if (waitRailIndexCommand == true)
	{
		if (cameraObj->GetRaleIndex() >= waitRail)
		{
			waitRailIndexCommand = false;
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

		if (word.find("BILL") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);
			
			pillar->SetPosition( pos );
		}
		else if (word.find("BILL2") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			pillar2->SetPosition(pos);
		}
		else if (word.find("BILL3") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			pillar3->SetPosition(pos);
		}
		else if (word.find("BILL4") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			pillar4->SetPosition(pos);
		}
		else if (word.find("ONEWAY") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);
		
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
			enemyOneWay->Init(enemyModel,pos , LorR,attackFlag);
		}
		else if (word.find("CIRCLE") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int LR = std::atof(word.c_str());
			bool LorR = false;

			if (LR == 1)
			{
				LorR = true;
			}

			enemyCircle = new EnemyCircle();
			enemyCircle->Init(enemyRotateModel, pos, LorR);
		}
		else if (word.find("OUTWAY") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int LR = std::atof(word.c_str());
			bool LorR = false;

			if (LR == 1)
			{
				LorR = true;
			}

			enemy = new Enemy();
			enemy->Init(enemyModel, pos , enemyModel);
		}
		else if (word.find("BOSS") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			boss = new Boss();
			boss->Init(bossModel, enemyBulletModel, { pos });
		}
		else if (word.find("CHILDLUF") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLUF = new BossChild();
			bossChildLUF->Init(bossModel,   pos ,number);
		}
		else if (word.find("CHILDLUB") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLUB = new BossChild();
			bossChildLUB->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDRUF") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRUF = new BossChild();
			bossChildRUF->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDRUB") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRUB = new BossChild();
			bossChildRUB->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDLDF") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLDF = new BossChild();
			bossChildLDF->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDLDB") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLDB = new BossChild();
			bossChildLDB->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDRDF") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRDF = new BossChild();
			bossChildRDF->Init(bossModel, pos, number);
		}
		else if (word.find("CHILDRDB") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildRDB = new BossChild();
			bossChildRDB->Init(bossModel, pos, number);
		}
		else if (word.find("RAIL") == 0)
		{
			getline(line_stream, word, ',');

			int32_t waitRailIndex = atoi(word.c_str());

			waitRailIndexCommand = true;
			waitRail = waitRailIndex;
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

XMFLOAT3 GameScene::CommandPositionSet(std::istream  &line_stream, std::string &word)
{
	XMFLOAT3 position;
	getline(line_stream, word, ',');
	position.x = (float)std::atof(word.c_str());

	getline(line_stream, word, ',');
	position.y = (float)std::atof(word.c_str());

	getline(line_stream, word, ',');
	position.z = (float)std::atof(word.c_str());

	return position;
}

void GameScene::PlayerCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 5; i++)
	{

		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		/*pos.x = (float)rand() / RAND_MAX * enemy->GetWorldPosition().x - enemy->GetWorldPosition().x / 2.0f;
		pos.y = (float)rand() / RAND_MAX * enemy->GetWorldPosition().y - enemy->GetWorldPosition().y / 2.0f;
		pos.z = (float)rand() / RAND_MAX * enemy->GetWorldPosition().z - enemy->GetWorldPosition().z / 2.0f;*/


		const float rnd_vel = 0.8f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.051f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 3.0f, 0.0f, { 1,1,1 }, { 1,0,0 });
	}

}

void GameScene::EnemyCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 100; i++)
	{

		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		/*pos.x = (float)rand() / RAND_MAX * enemy->GetWorldPosition().x - enemy->GetWorldPosition().x / 2.0f;
		pos.y = (float)rand() / RAND_MAX * enemy->GetWorldPosition().y - enemy->GetWorldPosition().y / 2.0f;
		pos.z = (float)rand() / RAND_MAX * enemy->GetWorldPosition().z - enemy->GetWorldPosition().z / 2.0f;*/


		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.011f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(64, pos, vel, acc, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
	}

}

void GameScene::BossCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 30; i++)
	{

		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		/*pos.x = (float)rand() / RAND_MAX * enemy->GetWorldPosition().x - enemy->GetWorldPosition().x / 2.0f;
		pos.y = (float)rand() / RAND_MAX * enemy->GetWorldPosition().y - enemy->GetWorldPosition().y / 2.0f;
		pos.z = (float)rand() / RAND_MAX * enemy->GetWorldPosition().z - enemy->GetWorldPosition().z / 2.0f;*/


		const float rnd_vel = 0.8f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.011f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
	}
}

void GameScene::BillCreate()
{
	const float iMax = 600;
	const float iAdd = 80;
	for (float i = 0; i < iMax; i += iAdd)
	{
		//もでる　ランダムで１２３とかきめてだす
		
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -50,-32,280 + i }, { 4,13,1 });
		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 50,-32,280 + i }, { 4,13,1 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { 80,-32,280 + i }, { 4,13,1 });

	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { -80,-32,280 + i }, { 4,13,1 });

	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}

	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { 110,-32,280 + i }, { 4,13,1 });
	//	
	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { -110,-32,280 + i }, { 4,13,1 });
	//	
	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { 140,-32,280 + i }, { 4,13,1 });
	//	
	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { -140,-32,280 + i }, { 4,13,1 });
	//	
	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { 170,-32,280 + i }, { 4,13,1 });
	//	
	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}
	//for (float i = 0; i < iMax; i += iAdd)
	//{
	//	//bill生成
	//	std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
	//	newBill->Init(pillarModel, { -170,-32,280 + i }, { 4,13,1 });

	//	//bill登録
	//	bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	//}


}

