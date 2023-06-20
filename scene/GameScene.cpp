#include "GameScene.h"
#include "time.h"

void(GameScene::* GameScene::spFuncTable[])() = {
	&GameScene::Title,
	&GameScene::Game,
	&GameScene::Clear,
	&GameScene::GameOver,
};
void(GameScene::* GameScene::spFuncTableDraw[])() = {
	&GameScene::TitleDraw,
	&GameScene::GameDraw,
	&GameScene::ClearDraw,
	&GameScene::GameOverDraw,
};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio,Win* win)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxcommon = dxCommon;
	this->input = input;
	this->audio = audio;
	this->win = win;

	//モデル読み込み
	skydome_model = Model::LoadFromOBJ("skydome");
	playerModel = Model::LoadFromOBJ("player");
	bulletModel = Model::LoadFromOBJ("ene-0");
	enemyModel = Model::LoadFromOBJ("oneWay");
	enemyRotateModel = Model::LoadFromOBJ("rotate");
	bossModel = Model::LoadFromOBJ("boss");
	bossMiniModel = Model::LoadFromOBJ("bossMini");
	wallModel = Model::LoadFromOBJ("wallBig");
	wallBossModel = Model::LoadFromOBJ("wallBoss");
	wallFlatModel = Model::LoadFromOBJ("wallFlatGray");
	billModel = Model::LoadFromOBJ("bill");
	enemyBulletModel = Model::LoadFromOBJ("box_aka");
	roadModel = Model::LoadFromOBJ("road");
	shotObjModel = Model::LoadFromOBJ("shotObj");
	shotHibiObjModel = Model::LoadFromOBJ("shotHibiObj");
	kanbanModel = Model::LoadFromOBJ("kanban");
	kanbanPlaneModel = Model::LoadFromOBJ("kanbanPlane");
	kanbanShotModel = Model::LoadFromOBJ("kanbanShot");
	kanbanShot2Model = Model::LoadFromOBJ("kanbanShot2");
	kanbanShot3Model = Model::LoadFromOBJ("kanbanShot3");
	kanbanShot4Model = Model::LoadFromOBJ("kanbanShot4");
	barrierModel = Model::LoadFromOBJ("barrier",0.7);
	targetModel = Model::LoadFromOBJ("target");
	clickModel = Model::LoadFromOBJ("click");
	clearModel = Model::LoadFromOBJ("clear");

	bossHpBar = 733;
	bossHpBarMax = 733;
	dieTimer = 120;

	pointsLast = false;

	bossFlag = false;

	hpBar = 288;
	hpBarMax = 288;

	//プレイヤーが倒された用
	playerDieFlag = false;

	//オブジェクトの初期化
	skydome->SetModel(skydome_model);
	skydome->scale = { 11,11,15 };
	skydome->SetPosition({ 0,-220,skydomeZ });

	shotObj->SetModel(shotObjModel);
	shotObj->scale = { 3,3,3 };
	shotObj->SetPosition({ 0,-200,0 });

	shotHibiObj->SetModel(shotHibiObjModel);
	shotHibiObj->scale = { 3,3,3 };
	shotHibiObj->SetPosition({ 0,0,90 });

	targetObj->SetModel(targetModel);
	targetObj->scale = { 3,3,3 };
	targetObj->SetPosition({ 0,-200,100 });
	targetObj->rotation.y = 180;

	clickObj->SetModel(clickModel);
	clickObj->scale = { 3,3,3 };
	clickObj->SetPosition({ 0,0,90 });

	kanbanObj->SetModel(kanbanModel);
	kanbanObj->scale = { 1.5,1.5,1.5 };
	kanbanObj->SetPosition({ -90,-50,120 });
	
	kanbanPlaneObj->SetModel(kanbanPlaneModel);
	kanbanPlaneObj->scale = { 1.5,1.5,1.5 };
	kanbanPlaneObj->SetPosition({ 90,-50,120 });
	kanbanShotObj->SetModel(kanbanShotModel);
	kanbanShotObj->scale = { 1.5,1.5,1.5 };
	kanbanShotObj->SetPosition({ kanbanShotPos });
	kanbanShot2Obj->SetModel(kanbanShot2Model);
	kanbanShot2Obj->scale = { 1.5,1.5,1.5 };
	kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
	kanbanShot3Obj->SetModel(kanbanShot3Model);
	kanbanShot3Obj->scale = { 1.5,1.5,1.5 };
	kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
	kanbanShot4Obj->SetModel(kanbanShot4Model);
	kanbanShot4Obj->scale = { 1.5,1.5,1.5 };
	kanbanShot4Obj->SetPosition({ kanbanShotPosDown });

	titleObj->SetModel(shotObjModel);
	titleObj->scale = { 1.5,1.5,1.5 };
	titleObj->SetPosition({ 0,25,-560 });

	clickObj->SetModel(clickModel);
	clickObj->scale = { 1.5,1.5,1.5 };
	clickObj->SetPosition({ 0,15,-560 });

	clearObj->SetModel(clearModel);
	clearObj->scale = { 3,3,3 };
	clearObj->SetPosition({ 0,15,0 });

	player = new Player();
	player->Init(playerModel, bulletModel);

	startPlayer->SetModel(playerModel);
	startPlayer->scale = { 2,2,2 };
	startPlayer->SetPosition({0,0,-550});

	boss = new Boss();
	boss->Init(bossModel, enemyBulletModel, barrierModel,{ 0,0,-200.0f });

	bossChildLUF = new BossChild();
	bossChildLUF->Init(bossModel, { 0, 0, 1000.0f }, 1);//csvで読み込みする
	bossChildLUB = new BossChild();
	bossChildLUB->Init(bossModel, { 0, 0, 1000.0f }, 2);
	bossChildRUF = new BossChild();
	bossChildRUF->Init(bossModel, { 0, 0, 1000.0f }, 3);
	bossChildRUB = new BossChild();
	bossChildRUB->Init(bossModel, { 0, 0, 1000.0f }, 4);
	bossChildLDF = new BossChild();
	bossChildLDF->Init(bossModel, { 0, 0, 1000.0f }, 5);
	bossChildLDB = new BossChild();
	bossChildLDB->Init(bossModel, { 0, 0, 1000.0f }, 6);
	bossChildRDF = new BossChild();
	bossChildRDF->Init(bossModel, { 0, 0, 1000.0f }, 7);
	bossChildRDB = new BossChild();
	bossChildRDB->Init(bossModel, { 0, 0, 1000.0f }, 8);

	camera = new Camera();
	camera->Init();
	
	cameraObj = new CameraObj();
	cameraObj->Init({ 0.0f, 0.0f, 0.0f },{0,0,0});

	//床
	wallFloor->SetModel(wallFlatModel);
	wallFloor->scale = { 65,7,170 };
	wallFloor->SetPosition({ 0,-65,50 });

	//床の道路
	road->SetModel(roadModel);
	road->scale = { 10,7,600 };
	road->SetPosition({ 0,-55,650 });

	Object3d::SetCamera(camera);

	//敵の情報をcsvから抽出
	EnemyPopLoadData();

	tutorialFlag = true;

	// 3Dオブジェクト生成
	Particle = ParticleManager::Create(dxcommon->GetDev(), camera);

	billM = new BillManager();
	billM->BillSet(billModel);
	//ビル生成

	//スプライト初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	//レティクル
	spriteCommon->LoadTexture(0, L"Resource/target.png");
	reticleSprite = Sprite::Create(spriteCommon, 0);
	reticleSprite->SetPosition({ 0.0f,0.0f,0.0f });
	reticleSprite->SetSize({ 100,100 });
	reticleSprite->TransVertexBuffer();


	//ボスのhpバーの枠
	spriteCommon->LoadTexture(4, L"Resource/hpWaku.png");
	bossHpWakuSprite = Sprite::Create(spriteCommon, 4);
	bossHpWakuSprite->SetPosition({ 640.0f,50.0f,0.0f });
	bossHpWakuSprite->SetSize({ 769,38 });
	bossHpWakuSprite->TransVertexBuffer();

	//ボスのhpバー
	spriteCommon->LoadTexture(5, L"Resource/hpBar.png");
	bossHpBarSprite = Sprite::Create(spriteCommon, 5, { 0,0 });
	bossHpBarSprite->SetPosition({ 273.0f,34.0f,0.0f });

	//プレイヤーのhp
	spriteCommon->LoadTexture(6, L"Resource/heart.png");
	playerHpSprite = Sprite::Create(spriteCommon, 6);
	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ 288,96 });
	playerHpSprite->SetTexsize({ 288,96 });
	playerHpSprite->TransVertexBuffer();

	//movie　skip用
	spriteCommon->LoadTexture(11, L"Resource/kSkip.png");
	kSkipSprite = Sprite::Create(spriteCommon, 11);
	kSkipSprite->SetPosition({ 1200.0f,20.0f,0.0f });
	kSkipSprite->SetSize({ 140,32 });
	kSkipSprite->TransVertexBuffer();

	//デバッグテキスト
	debugtext_minute = new DebugText();

	const int debugTextTexNumber3 = 20;
	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");
	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 21;
	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);

}

void GameScene::Update()
{

	phaseNumber = static_cast<int>(scene_);
	(this->*spFuncTable[phaseNumber])();
	phaseDrawNumber = static_cast<int>(sceneDraw_);
	
	
//共通処理
	//照準
	reticleSprite->Update();
	reticleSprite->SetPosition({ mouseX,mouseY,0 });

	playerHpSprite->Update();//playerのhp
	kSkipSprite->Update();//ムービースキップ
	//デバッグテキスト
	debugtext_minute->Print(moji, 0, 0, 1.0f);
	debugtext_minute2->Print(moji2, 0, 100, 1.0f);

	
//デバッグ用コマンド
	//プレイヤーのｈｐへる
	if (input->isKeyTrigger(DIK_P))
	{
		player->OnCollision();
	}
	//当たり判定けす
	if (input->isKeyTrigger(DIK_C) && mutekiFlagDeb == true)
	{
		mutekiFlagDeb = false;
	}
	//当たり判定戻す
	else if (input->isKeyTrigger(DIK_C))
	{
		mutekiFlagDeb = true;
	}
	//ボスのhp けずる
	int bossHpAttack = 35;
	if (input->isKeyTrigger(DIK_Q))
	{
		for (int i = 0; i < bossHpAttack; i++)
		{
			boss->GetEnemy()->OnBossCollision();
		}		
	}
	if (input->isKeyTrigger(DIK_N))
	{
		pointsLast = true;
		scene_ = Scene::Clear;
		sceneDraw_ = SceneDraw::ClearDraw;
	}
	if (input->isKeyTrigger(DIK_M))
	{
		playerDieFlag = true;
		scene_ = Scene::GameOver;
		sceneDraw_ = SceneDraw::GameOverDraw;
	}

	//ビル更新処理
	billM->Update();

	//スカイドーム
	skydome->SetPosition({ 0,-220,skydomeZ });
	skydome->Update();

	//設置物の更新
	wallFloor->Update();
	road->Update();
	shotHibiObj->Update();
	kanbanObj->Update();
	kanbanPlaneObj->Update();
	shotObj->Update();
	targetObj->Update();
	
	//2dレティクルスプライトの座標
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//デバッグ用
	sprintf_s(moji, "%f", boss->GetPos().y);
	sprintf_s(moji2, "%f", boss->GetPos().z);

	//カメラの注視点セット
	cameraObj->SetTargetS(startPlayer->GetPosition());
	cameraObj->SetPlayerDieFlag(player->GetHp0());
	cameraObj->SetTarget(player->GetWorldPosition());
	cameraObj->SetBoss(boss->GetPos(), boss->GetBossDead());
	cameraObj->UpdateCamera();

	//カメラ
	camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });
	camera->UpdateCamera();

	//レティクル
	player->SetHwnd(hwnd);
	player->SetViewPort(viewPort);
	player->SetCameraMatViewProjection(cameraObj->GetMatViewProjection());
	player->Update();

	startPlayer->Update();
	Particle->Update();
}

void GameScene::Draw()
{	
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());

	skydome->Draw();//スカイドーム
	wallFloor->Draw();//床
	road->Draw();//道

	billM->Draw();

	startPlayer->Draw();
	Object3d::PostDraw();
	// 3Dオブクジェクトの描画おわり
	(this->*spFuncTableDraw[phaseDrawNumber])();
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	
	Particle->Draw(dxcommon->GetCmdlist());
	
	Object3d::PostDraw();
// 3Dオブクジェクトの描画おわり


	
}

void GameScene::CheckAllCollision(Enemy* enemy)
{
	if (mutekiFlagDeb == true)
	{
		return;
	}
	if (mutekiFlag == true)
	{
		mutekiCoolTime--;
		if (mutekiCoolTime <= mutekiCoolTimeMin)
		{
			mutekiFlag = false;
			mutekiCoolTime = mutekiCoolTimeMax;
		}
	}
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
			if (boss->GetEnemy()->IsDead() == false && mutekiFlag == false)
			{
				player->OnCollision();
				//パーティクル生成
				Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
					{ 1,1,1 }, { 1,0,0 });

				mutekiFlag = true;
			}

			bullet->OnCollision();
		}
	}
	
	//自弾と敵当たり判定
	pos1 = enemy->GetWorldPosition();

	//当たり判定の調整
	float pos1Add = 4;
	float pos1AddZ = 4;//z座標に追加調整
	float pos1BossAdd = 12;//ボスの調整
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		//雑魚敵と弾の当たり判定
		if (pos2.z <= pos1.z + pos1Add + pos1AddZ && pos2.z >= pos1.z - pos1Add - pos1AddZ &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if (cameraObj->GetRaleIndex() <= 5)
			{
				enemy->OnCollision();

				bullet->OnCollision();
				//パーティクル生成
				Particle->CreateParticle(30, 64, enemy->GetWorldPosition(), { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 1.0f,
					{ 1,1,1 }, { 1,0.5,0 });
			}
			
		}
		//ボスと弾の当たり判定
		if (pos2.z <= pos1.z + pos1BossAdd && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1BossAdd && pos2.x >= pos1.x - pos1BossAdd &&
			pos2.y <= pos1.y + pos1BossAdd && pos2.y >= pos1.y - pos1Add)
		{
			if(cameraObj->GetRaleIndex() >= 6 )
			{
				enemy->OnBossCollision();
				if (boss->GetBarrierFlag() == false || boss->GetBossMovieFlag() == false)
				{
					Particle->CreateParticle(30, 32, enemy->GetWorldPosition(), { 1.3,1.3,1.3 }, { 0.01,0.01,0.01 }, 25.0f, 2.0f,
						{ 1,1,1 }, { 1,0.5,0 });
				}
			
				bullet->OnCollision();
			}
		}
	}
}

void GameScene::CheckBossANDChildCollision(Enemy* bossChild)
{
	if (mutekiFlagDeb == true)
	{
		return;
	}
	if (mutekiFlag == true)
	{
		mutekiCoolTime--;
		if (mutekiCoolTime <= mutekiCoolTimeMin)
		{
			mutekiFlag = false;
			mutekiCoolTime = mutekiCoolTimeMax;
		}
	}
	XMFLOAT3 pos1, pos2;
	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//敵弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bossChild->GetBullets();


	pos1 = boss->GetEnemy()->GetWorldPosition();
	//bossとbossmini 弾当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		pos2 = bullet->GetWorldPosition();
		length = ((pos2.x - pos1.x) * (pos2.x - pos1.x)) +
			((pos2.y - pos1.y) * (pos2.y - pos1.y)) +
			((pos2.z - pos1.z) * (pos2.z - pos1.z));
		if (length <= size)
		{
			bullet->OnCollision();
		}
	}

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
			if (boss->GetEnemy()->IsDead() == false && mutekiFlag == false)
			{
				player->OnCollision();
				//パーティクル生成
				Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
					{ 1,1,1 }, { 1,0,0 });
				mutekiFlag = true;
			}

			bullet->OnCollision();
		}
	}

	//自弾と敵当たり判定
	pos1 = bossChild->GetWorldPosition();

	//当たり判定の調整
	float pos1Add = 4;

	//ボスの周りをまわる敵の当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();

		if (pos2.z <= pos1.z + pos1Add && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if(boss->GetBarrierPhaseFlag() == true)
			{
				
				if (bossChild->IsDead() == true)
				{
					Particle->CreateParticle(30, 32, bossChild->GetWorldPosition(), { 1.3,1.3,1.3 }, { 0.01,0.01,0.01 }, 25.0f, 2.0f,
						{ 1,1,1 }, { 1,0.5,0 });
				}
				else
				{
					bossChild->OnBossMiniCollision();
					Particle->CreateParticle(30, 32, bossChild->GetWorldPosition(), { 1.3,1.3,1.3 }, { 0.01,0.01,0.01 }, 25.0f, 2.0f,
						{ 1,1,1 }, { 1,0.5,0 });
				}
				bullet->OnCollision();
			}
		}
	}
	
}

void GameScene::CheckTargetCollision()
{
	if (mutekiFlagDeb == true)
	{
		return;
	}
	XMFLOAT3 pos1, pos2;

	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = targetObj->GetPosition();
	pos1.y = pos1.y + 10;

	float pos1Addx = 6;
	float pos1Addy = 6;
	float pos1Addz = 8;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		if (count == 1)
		{
		
		}
		pos2 = bullet->GetWorldPosition();
		//チュートリアルの的にダメージ
		if (pos2.z <= pos1.z + pos1Addz && pos2.z >= pos1.z &&//z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			targetHp >= 1 //alive
			)
		{
			bullet->OnCollision();
			targetHp--;
			if (targetHp >= 1)
			{
				//パーティクル生成
				Particle->CreateParticle(30, 32, bullet->GetWorldPosition(), { 2.8f, 2.8f, 2.8f }, { 0.02,0.02,0.02 }, 12.0f, 2.0f,
					{ 1,0,0 }, { 0.5,0.3,0.17 });
			}
			else if(count == 0)
			{
				count++;
				Particle->CreateParticle(30, 32, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z }
					, { 2.8f, 2.8f, 2.8f }, { 0,-0.08,0 }, 12.0f, 2.0f,
					{ 0,1,0 }, { 0.5,0.3,0.17 });
				targetHp = targetHpMax;
				targetObj->position.x = 50;
			
				//targetObj->rotation.y = 190;
			}
			else if (count == 1)
			{
				count++;
				Particle->CreateParticle(30, 32, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z }
					, { 2.8f, 2.8f, 2.8f }, { 0,-0.08,0 }, 12.0f, 2.0f,
					{ 0,1,0 }, { 0.5,0.3,0.17 });
				targetHp = targetHpMax;
				targetObj->position.x = -50;
				targetObj->rotation.y = 165;
			}
			else if (count == 2)
			{
				count++;
				Particle->CreateParticle(30, 32, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z }
					, { 2.8f, 2.8f, 2.8f }, { 0,-0.08,0 }, 12.0f, 2.0f,
					{ 0,1,0 }, { 0.5,0.3,0.17 });
				targetHp = targetHpMax;
				tutorialFlag = false;
				targetObj->position.y = -250;
				targetObj->rotation.y = 165;
			}

		}
	}
}

void GameScene::CheckBillCollision(BillManager* bill)
{
	XMFLOAT3 pos1, pos2;

	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = bill->GetPos();

	int pos1Add = 20;
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();

		if (pos2.z <= pos1.z + pos1Add && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			bullet->OnCollision();
		}

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

			//生成
			std::unique_ptr<EnemyOneWay> newOneWay = std::make_unique<EnemyOneWay>();
			newOneWay->Init(enemyModel, pos, LorR, attackFlag);

			//登録
			oneWays.push_back(std::move(newOneWay));//move はユニークから譲渡するため
		}
		else if (word.find("MOVIE") == 0)
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

			//生成
			std::unique_ptr<EnemyOneWay> newOneWay2 = std::make_unique<EnemyOneWay>();
			newOneWay2->Init(enemyModel, pos, LorR, attackFlag,true);

			//登録
			oneWayMovies.push_back(std::move(newOneWay2));//move はユニークから譲渡するため
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
			//0が攻撃　1が攻撃しない
			int attack = std::atof(word.c_str());
			bool attackFlag = false;
			if (attack == 1)
			{
				attackFlag = false;
			}
			else
			{
				attackFlag = true;
			}

			//生成
			std::unique_ptr<EnemyCircle> newCircle = std::make_unique<EnemyCircle>();
			newCircle->Init(enemyRotateModel, pos, LorR, attackFlag);

			//登録
			circles.push_back(std::move(newCircle));//move はユニークから譲渡するため

		}
		else if (word.find("BOSS") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			boss = new Boss();
			boss->Init(bossModel, enemyBulletModel, barrierModel,{ pos });
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

void GameScene::Title()
{
	if (input->isMouseKey())
	{
		cameraObj->SetStartMovieSkip(true);
		scene_ = Scene::Game;
		sceneDraw_ = SceneDraw::GameDraw;
	}
	titleObj->Update();
	clickCount++;
	clickObj->Update();

	for (std::unique_ptr<EnemyOneWay>& oneWay2 : oneWayMovies)
	{
		oneWay2->Update();
	}
}

void GameScene::Game()
{
	//sprite
		//ボスのhp
	bossHpBarSprite->SetSize({ bossHpBar,32 });
	bossHpBarSprite->TransVertexBuffer();

	bossHpBarSprite->Update();//ボスのhpバー
	bossHpWakuSprite->Update();//ボスのhpバーの枠

	//playerのhp
	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ hpBar,96 });
	playerHpSprite->SetTexsize({ hpBar,96 });
	playerHpSprite->TransVertexBuffer();

	//3d設置物
		//スカイドーム
		//カメラの移動量より少し遅く動く
	skydomeZ += cameraObj->GetEyeVec().z * skydomeVec;

	//スタートムービー
	if (cameraObj->GetStartMovieFlag() == false)
	{
		//playerを上ななめ前にとばす
		startPlayer->position.z += startPlayerAddZ;
		startPlayer->position.y += startPlayerAddY;
	}

	//ムービースキップ
	if (input->isKeyTrigger(DIK_K))
	{
		cameraObj->SetStartMovieSkip(false);
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;//ムービースキップのスプライト
	}

	//スタートムービー後の自機(仮)の場所
	if (startPlayer->GetPosition().z >= startPlayerAfterPos.z && boss->GetBossDead() == false)
	{
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;//ムービースキップのスプライト
	}
	else
	{
		//ムービー中のパーティクル
		movieParticleTime++;
		//60frame から　120frame　のあいだに出すパーティクル (ビルが攻撃されている感じ)
		if (movieParticleTime >= 60 && movieParticleTime <= 120)
		{
			movieParticleXL = particleMinPosXTitle;
			movieParticleXR = particleMaxPosXTitle;
			Particle->CreateParticle(particleCountTitle, particleLifeTitle, { -movieParticleXL, 0,-320 }, particleVelocityTitle,
				particleAccelTitle, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, 20,-320 }, particleVelocityTitle,
				particleAccelTitle, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			//80frame から　でるパーティクル (ずっと同じ場所に出てると変だから)
			if (movieParticleTime >= 80)
			{
				Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, 20,-320 }, particleVelocityTitle,
					particleAccelTitle, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			}
		}
		//120frame から　180frame　のあいだに出すパーティクル (ビルが攻撃されている感じ)
		else if (movieParticleTime >= 120 && movieParticleTime <= 180)
		{
			movieParticleXL = particleMaxPosXTitle;
			movieParticleXR = particleMinPosXTitle;
			Particle->CreateParticle(particleCountTitle, particleLifeTitle, { -movieParticleXL, 0,-320 }, particleVelocityTitle,
				particleAccelTitle, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			//160frame から　でるパーティクル (ずっと同じ場所に出てると変だから)
			if (movieParticleTime >= 160)
			{
				Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, 20,-320 }, particleVelocityTitle,
					particleAccelTitle, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			}
		}
		else if (movieParticleTime >= 180)
		{
			//180frameを0に
			movieParticleTime = 0;
		}
	}

	//csvのenemy発生
	UpdateEnemyPop();

	//チュートリアルがおわっているかをカメラに教える
	cameraObj->SetTutorialFlag(tutorialFlag);

	//ムービーが終わった後のチュートリアル用のobj　と　playerの位置設定
	if (cameraObj->GetStartGameFlag() == true && setObjectFlag == false)
	{
		targetObj->SetPosition({ 0,0,100 });
		player->SetPlayerPos({ 0,0,0 });
		setObjectFlag = true;
	}
	//弾の発射説明obj　　(画面右の看板)(アニメーションする)
	if (cameraObj->GetStartGameFlag() == true)
	{
		kanbanTime++;
	}
	if (kanbanTime >= kanbanTimeMax)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPos });
		kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot4Obj->SetPosition({ kanbanShotPosDown });
	}
	if (kanbanTime >= kanbanTimeMax * 2)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot3Obj->SetPosition({ kanbanShotPos });
		kanbanShot4Obj->SetPosition({ kanbanShotPosDown });
	}
	if (kanbanTime >= kanbanTimeMax * 3)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot4Obj->SetPosition({ kanbanShotPos });
	}
	if (kanbanTime >= kanbanTimeMax * 4)
	{
		kanbanShotObj->SetPosition({ kanbanShotPos });
		kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot4Obj->SetPosition({ kanbanShotPosDown });
		kanbanTime = 0;
	}
	//弾の発射説明obj　の更新処理
	kanbanShotObj->Update();
	kanbanShot2Obj->Update();
	kanbanShot3Obj->Update();
	kanbanShot4Obj->Update();


	//自機
	player->SetStartFlag(cameraObj->GetStartGameFlag());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->SetPlayerHpBar(hpBar);

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//自機の弾のパーティクル
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet1 = bullet->GetWorldPosition();

		//攻撃時パーティクルがでる
		if (input->isMouseKey())
		{
			attackParticleFlag = true;
		}
		if (attackParticleFlag == true)
		{
			Particle->CreateParticle(30, 8, bullet1, { 0.5,0.5,0.5 }, { 0.03,0.03,0.03 }, 5.0f, 1.0f, { 1,1,1 }, { 1,0,0 });
		}
	}

	// 敵
	//oneway更新処理
	for (std::unique_ptr<EnemyOneWay>& oneWay : oneWays)
	{
		CheckAllCollision(oneWay->GetEnemy());
		oneWay->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay->SetPlayerPosition(player->GetWorldPosition());
		oneWay->Update();
	}

	for (std::unique_ptr<EnemyOneWay>& oneWay2 : oneWayMovies)
	{
		CheckAllCollision(oneWay2->GetEnemy());
		oneWay2->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay2->Update();
	}

	//circle更新処理
	for (std::unique_ptr<EnemyCircle>& circle : circles)
	{
		CheckAllCollision(circle->GetEnemy());
		circle->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		circle->SetPlayerPosition(player->GetWorldPosition());
		circle->Update();
	}

	//ボス
	boss->SetPlayerWorldPos(player->GetWorldPosition());
	boss->GetEnemy()->SetBossHpBar(bossHpBar, bossHpBarMax);
	boss->Update();

	//ボスの周りをまわる敵

	 //ボスの周りをまわる攻撃のため
	 //ボスに追従するため
	bossChildLUF->SetBossPos(boss->GetPos());
	bossChildLUF->SetBossVec(boss->GetBossVec());
	bossChildLUF->Update();

	bossChildLUB->SetBossPos(boss->GetPos());
	bossChildLUB->SetBossVec(boss->GetBossVec());
	bossChildLUB->Update();

	bossChildRUF->SetBossPos(boss->GetPos());
	bossChildRUF->SetBossVec(boss->GetBossVec());
	bossChildRUF->Update();

	bossChildRUB->SetBossPos(boss->GetPos());
	bossChildRUB->SetBossVec(boss->GetBossVec());
	bossChildRUB->Update();

	bossChildLDF->SetBossPos(boss->GetPos());
	bossChildLDF->SetBossVec(boss->GetBossVec());
	bossChildLDF->Update();

	bossChildLDB->SetBossPos(boss->GetPos());
	bossChildLDB->SetBossVec(boss->GetBossVec());
	bossChildLDB->Update();

	bossChildRDF->SetBossPos(boss->GetPos());
	bossChildRDF->SetBossVec(boss->GetBossVec());
	bossChildRDF->Update();

	bossChildRDB->SetBossPos(boss->GetPos());
	bossChildRDB->SetBossVec(boss->GetBossVec());
	bossChildRDB->Update();


	//ボスがバリアを張ったか
	bossChildLUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());

	//ボス系当たり判定
	CheckAllCollision(boss->GetEnemy());
	CheckBossANDChildCollision(bossChildLUF->GetEnemy());
	CheckBossANDChildCollision(bossChildLUB->GetEnemy());
	CheckBossANDChildCollision(bossChildRUF->GetEnemy());
	CheckBossANDChildCollision(bossChildRUB->GetEnemy());
	CheckBossANDChildCollision(bossChildLDF->GetEnemy());
	CheckBossANDChildCollision(bossChildLDB->GetEnemy());
	CheckBossANDChildCollision(bossChildRDF->GetEnemy());
	CheckBossANDChildCollision(bossChildRDB->GetEnemy());
	CheckTargetCollision();

	//ボスのバリアを割る
	if (
		bossChildLUF->GetMiniDead() == true &&
		bossChildLUB->GetMiniDead() == true &&
		bossChildRUF->GetMiniDead() == true &&
		bossChildRUB->GetMiniDead() == true &&
		bossChildLDF->GetMiniDead() == true &&
		bossChildLDB->GetMiniDead() == true &&
		bossChildRDF->GetMiniDead() == true &&
		bossChildRDB->GetMiniDead() == true
		)
	{
		boss->SetBarrierFlag(false);
	}

	//hp
	bossHpBar = boss->GetEnemy()->GetHpBarX();
	hpBar = player->GetHpBar();

	//ボス戦はじまる (カメラが止まる)
	if (cameraObj->GetEndFlag() == true)
	{
		bossFlag = true;
	}

	//ボスが倒されたら
	if (boss->GetEnemy()->IsDead() == true)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);

		//ボスの演出がおわったら
		if (boss->GetEnemy()->GetWorldPosition().y <= floorY)
		{
			//シーンチェンジまち用
			dieTimer--;
			if (dieTimer <= 0)
			{
				player->SetEndMovieFlag(true);
				//シーンチェンジ
				if (player->GetWorldPosition().z >= endPlayerPos)
				{
					pointsLast = true;
					scene_ = Scene::Clear;
					sceneDraw_ = SceneDraw::ClearDraw;
				}
			}
			else
			{
				cameraObj->SetEndMovieFlag(true);
			}
		}
		else
		{
			Particle->CreateParticle(30, 48, boss->GetPos(), { 0.5,0.5,0.5 }, { 0.05,0.05,0.05 }, 10.0f, 2.0f,
				{ 1,1,1 }, { 1,0.5,0 });
		}
	}



	//プレイヤーのhpが０になったら
	if (player->GetHp0() == true)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);
		//パーティクル生成
		Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
			{ 1,1,1 }, { 1,0,0 });

		//bossのやられた演出まち用
		dieTimer--;

		if (dieTimer <= 0)
		{
			//シーンチェンジ
			playerDieFlag = true;
			scene_ = Scene::GameOver;
			sceneDraw_ = SceneDraw::GameOverDraw;
			dieTimer = dieTimerMax;
		}

	}
	//ゲームが終わったらビルを消す
	if (pointsLast == true || playerDieFlag == true)
	{
		billM->SetBillDeadFlag(true);
	}

	//条件を満たしたlistけす

	oneWays.remove_if([](std::unique_ptr<EnemyOneWay>& oneWay)
		{
			return oneWay->GetIsDead();
		});
	oneWayMovies.remove_if([](std::unique_ptr<EnemyOneWay>& oneWayMovie)
		{
			return oneWayMovie->GetIsDead();
		});

	circles.remove_if([](std::unique_ptr<EnemyCircle>& circle)
		{
			return circle->GetIsDead();
		});

}

void GameScene::Clear()
{
	int addParticle = 110;
	float particleY = -30;
	//ムービー中のパーティクル
	movieParticleTime++;
	//60frame から　120frame　のあいだに出すパーティクル (ビルが攻撃されている感じ)
	if (movieParticleTime >= 60 && movieParticleTime <= 120)
	{
		movieParticleXL = particleMinPosXTitle;
		movieParticleXR = particleMaxPosXTitle;
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleY,cameraObj->GetEye().z + addParticle }, particleVelocityClear,
			particleAccelClear, 10.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleY,cameraObj->GetEye().z + addParticle }, particleVelocityClear,
			particleAccelClear, 10.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
		//80frame から　でるパーティクル (ずっと同じ場所に出てると変だから)
		if (movieParticleTime >= 80)
		{
			Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleY,cameraObj->GetEye().z + addParticle }, particleVelocityClear,
				particleAccelClear, 10.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
		}
	}
	//120frame から　180frame　のあいだに出すパーティクル (ビルが攻撃されている感じ)
	else if (movieParticleTime >= 120 && movieParticleTime <= 180)
	{
		movieParticleXL = particleMaxPosXTitle;
		movieParticleXR = particleMinPosXTitle;
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleY,cameraObj->GetEye().z + addParticle }, particleVelocityClear,
			particleAccelClear, 10.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
		//160frame から　でるパーティクル (ずっと同じ場所に出てると変だから)
		if (movieParticleTime >= 160)
		{
			Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleY,cameraObj->GetEye().z + addParticle }, particleVelocityClear,
				particleAccelClear, 10.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
		}
	}
	else if (movieParticleTime >= 180)
	{
		//180frameを0に
		movieParticleTime = 0;
	}
	clearObj->rotation.y++;
	clearObj->SetPosition({ 0,0,cameraObj->GetEye().z +50 });
	clickCount++;
	clickObj->SetPosition({ 0,-15,cameraObj->GetEye().z + 50 });
	clearObj->Update();
	clickObj->Update();

}

void GameScene::GameOver()
{
	shotObj->rotation.y++;
	shotObj->SetPosition({ 0,0,cameraObj->GetEye().z + 50 });

	shotObj->Update();
	clickCount++;
	clickObj->SetPosition({ 0,-15,cameraObj->GetEye().z + 50 });
	clickObj->Update();
}

void GameScene::TitleDraw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	titleObj->Draw();
	if (clickCount >= clickAliveCount)
	{
		clickObj->Draw();
		if (clickCount >= clickDeadCount)
		{
			clickCount = clickCountReset;
		}
	}
	for (std::unique_ptr<EnemyOneWay>& oneWay2 : oneWayMovies)
	{
		oneWay2->Draw();
	}
	Object3d::PostDraw();
	// 3Dオブクジェクトの描画おわり
	spriteCommon->PreDraw();
	reticleSprite->Draw();
}

void GameScene::GameDraw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	kanbanObj->Draw();
	kanbanPlaneObj->Draw();
	kanbanShotObj->Draw();
	kanbanShot2Obj->Draw();
	kanbanShot3Obj->Draw();
	kanbanShot4Obj->Draw();
	
	if (targetHp >= 1)
	{
		targetObj->Draw();
	}

	if (mutekiFlag == true && player->GetHp0() == false)
	{
		tenmetuCount++;
		if (tenmetuCount >= tenmetuAliveCount)
		{
			player->Draw();
			if (tenmetuCount >= tenmetuDeadCount)
			{
				tenmetuCount = tenmetuCountReset;
			}
		}
	}
	else
	{
		player->Draw();
	}

	for (std::unique_ptr<EnemyOneWay>& oneWay : oneWays)
	{
		oneWay->Draw();
	}
	for (std::unique_ptr<EnemyOneWay>& oneWay2 : oneWayMovies)
	{
		oneWay2->Draw();
	}
	for (std::unique_ptr<EnemyCircle>& circle : circles)
	{
		circle->Draw();
	}

	if (cameraObj->GetEndFlag() == true)
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
	Object3d::PostDraw();
	// 3Dオブクジェクトの描画おわり

	spriteCommon->PreDraw();
	reticleSprite->Draw();
	if (movieSkipFlag == false)
	{
		kSkipSprite->Draw();
	}
	if (tutorialFlag == true)
	{
	}
	else
	{
		playerHpSprite->Draw();
	}
	if (bossFlag == true)
	{
		bossHpBarSprite->Draw();
		bossHpWakuSprite->Draw();
	}

	//デバッグテキスト
	//debugtext_minute->DrawAll();
	//debugtext_minute2->DrawAll();
}

void GameScene::ClearDraw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	if (clickCount >= clickAliveCount)
	{
		clickObj->Draw();
		if (clickCount >= clickDeadCount)
		{
			clickCount = clickCountReset;
		}
	}
	clearObj->Draw();
	Object3d::PostDraw();
	// 3Dオブクジェクトの描画おわり
	spriteCommon->PreDraw();
	//endSprite->Draw();
}

void GameScene::GameOverDraw()
{
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());
	if (clickCount >= clickAliveCount)
	{
		clickObj->Draw();
		if (clickCount >= clickDeadCount)
		{
			clickCount = clickCountReset;
		}
	}
	shotObj->Draw();
	Object3d::PostDraw();
	// 3Dオブクジェクトの描画おわり
}



