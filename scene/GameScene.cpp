#include "GameScene.h"


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
	pillarModel = Model::LoadFromOBJ("bill2");
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
	shotObj->SetPosition({ 0,-200,90 });

	shotHibiObj->SetModel(shotHibiObjModel);
	shotHibiObj->scale = { 3,3,3 };
	shotHibiObj->SetPosition({ 0,0,90 });

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
	
	barrier->SetModel(barrierModel);
	barrier->scale = { 1.8,1.8,1.8 };
	barrier->SetPosition({ 0,0,20 });

	player = new Player();
	player->Init(playerModel, bulletModel);


	startPlayer->SetModel(playerModel);
	startPlayer->scale = { 2,2,2 };
	startPlayer->SetPosition({0,0,-550});//500,0,-50

	enemy = new Enemy();
	enemy->Init(enemyModel, { 30.0f, -300.0f, -200.0f }, enemyModel);//30,0,100
	
	enemyL = new Enemy();
	enemyL->Init(enemyModel, { 50.0f, -300.0f, -200.0f }, enemyModel);

	boss = new Boss();
	boss->Init(bossModel, enemyBulletModel, { 0,0,-200.0f });

	bossChildLUF = new BossChild();
	bossChildLUF->Init(bossModel, { 0, 0, 1000.0f }, 1);
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

	//ビル生成
	BillCreate();

	//描画初期化処理　ここから
	//スプライト初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	//レティクル
	spriteCommon->LoadTexture(0, L"Resource/target.png");
	
	reticleSprite = Sprite::Create(spriteCommon, 0);
	//説明のテキスト
	spriteCommon->LoadTexture(1, L"Resource/setu.png");
	explanSprite = Sprite::Create(spriteCommon, 1);

	//タイトル
	spriteCommon->LoadTexture(2, L"Resource/title.png");
	titleSprite = Sprite::Create(spriteCommon, 2);

	//エンド
	spriteCommon->LoadTexture(3, L"Resource/end.png");
	endSprite = Sprite::Create(spriteCommon, 3);

	//ボスのhpバーの枠
	spriteCommon->LoadTexture(4, L"Resource/hpWaku.png");
	bossHpWakuSprite = Sprite::Create(spriteCommon, 4);

	//ボスのhpバー
	spriteCommon->LoadTexture(5, L"Resource/hpBar.png");
	bossHpBarSprite = Sprite::Create(spriteCommon, 5, { 0,0 });

	//プレイヤーのhp
	spriteCommon->LoadTexture(6, L"Resource/heart.png");
	playerHpSprite = Sprite::Create(spriteCommon, 6);

	//ゲームオーバー
	spriteCommon->LoadTexture(7, L"Resource/gameover.png");
	gameOverSprite = Sprite::Create(spriteCommon, 7);

	//シーン遷移用
	spriteCommon->LoadTexture(10, L"Resource/backBlack.png");
	backBlack = Sprite::Create(spriteCommon, 10);

	//シーン遷移skip用
	spriteCommon->LoadTexture(11, L"Resource/kSkip.png");
	kSkipSprite = Sprite::Create(spriteCommon, 11);

	//デバッグテキスト
	debugtext_minute = new DebugText();

	const int debugTextTexNumber3 = 20;
	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");
	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	
	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 21;
	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);
	
	reticleSprite->SetPosition({ 0.0f,0.0f,0.0f });
	reticleSprite->SetSize({ 100,100 });
	reticleSprite->TransVertexBuffer();

	explanSprite->SetPosition({ 640.0f,360.0f,0.0f });
	explanSprite->SetSize({ 1280,720 });
	explanSprite->TransVertexBuffer();

	titleSprite->SetPosition({ 640.0f,360.0f,0.0f });
	titleSprite->SetSize({ 1280,720 });
	titleSprite->TransVertexBuffer();

	endSprite->SetPosition({ 640.0f,360.0f,0.0f });
	endSprite->SetSize({ 1280,720 });
	endSprite->TransVertexBuffer();

	gameOverSprite->SetPosition({ 640.0f,360.0f,0.0f });
	gameOverSprite->SetSize({ 1280,720 });
	gameOverSprite->TransVertexBuffer();

	bossHpWakuSprite->SetPosition({ 640.0f,50.0f,0.0f });
	bossHpWakuSprite->SetSize({ 769,38 });
	bossHpWakuSprite->TransVertexBuffer();

	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ 288,96 });
	playerHpSprite->SetTexsize({ 288,96 });
	playerHpSprite->TransVertexBuffer();

	backBlack->SetPosition({ backBlackX,360.0f,0.0f });
	backBlack->SetSize({ 1280,720 });

	kSkipSprite->SetPosition({ 1200.0f,20.0f,0.0f });
	kSkipSprite->SetSize({ 140,32 });
	kSkipSprite->TransVertexBuffer();

	bossHpBarSprite->SetPosition({ 273.0f,34.0f,0.0f });

	
}

void GameScene::Update()
{
	

	bossHpX = GetbossHpBar();
	hp = GetHpBar();

	bossHpBarSprite->SetSize({ bossHpX,32 });
	bossHpBarSprite->TransVertexBuffer();


	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ hp,96 });
	playerHpSprite->SetTexsize({ hp,96 });
	playerHpSprite->TransVertexBuffer();

	
	explanSprite->Update();
	titleSprite->Update();
	endSprite->Update();
	gameOverSprite->Update();
	bossHpBarSprite->Update();
	bossHpWakuSprite->Update();

	backBlack->SetPosition({ backBlackX,360.0f,0.0f });
	backBlack->Update();
	backBlack->TransVertexBuffer();

	//照準
	reticleSprite->Update();
	reticleSprite->SetPosition({ mouseX,mouseY,0 });

	playerHpSprite->Update();
	kSkipSprite->Update();
	//デバッグテキスト
	debugtext_minute->Print(moji, 0, 0, 1.0f);
	debugtext_minute2->Print(moji2, 0, 100, 1.0f);

	if (sceneChange == true)
	{
		backBlackX += 100;
	}

	//デバッグ用　プレイヤーのｈｐへる
	if (input->isKeyTrigger(DIK_P))
	{
		player->OnCollision();
	}
	//デバッグ用　当たり判定けす
	if (input->isKeyTrigger(DIK_C) && mutekiFlag == true)
	{
		mutekiFlag = false;
	}
	//デバッグ用　当たり判定戻す
	else if (input->isKeyTrigger(DIK_C))
	{
		mutekiFlag = true;
	}
	//デバッグ用　ボスのhp けずる
	int bossHpAttack = 35;
	if (input->isKeyTrigger(DIK_Q))
	{
		for (int i = 0; i < bossHpAttack; i++)
		{
			boss->GetEnemy()->OnBossCollision();
		}		
	}

	//自機の弾のパーティクル
	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet1 = bullet->GetWorldPosition();

		if (input->isMouseKey())
		{
			attackParticleFlag = true;

		}
		if (attackParticleFlag == true)
		{
			PlayerCreateParticle(bullet1);
		}
	}

	//ボスのバリア
	if (boss->GetBarrierFlag() == true)
	{
		barrier->SetPosition({ boss->GetPos().x,boss->GetPos().y - 10 , boss->GetPos().z - 5 });
		if (barrier->scale.x <= 8)
		{
			barrier->scale.x++;
			barrier->scale.y++;
			barrier->scale.z++;
		}
		else
		{
			barrier->scale = { 9,9,9 };
		}

	}
	else if (boss->GetBarrierFlag() == false)
	{
		barrier->scale = { 0,0,0 };
	}

	barrier->Update();


	//タイトルからスタートムービー演出へ
	if (cameraObj->GetStartMovieFlag() == false && gameStart == false)
	{
		if (input->isMouseKey())
		{
			cameraObj->SetStartMovieSkip(true);
			gameStart = true;
		}
	}
	//スタートムービー
	if (gameStart == true && cameraObj->GetStartMovieFlag() == false)
	{
		startPlayer->position.z += 4;
		startPlayer->position.y += 0.3;
	}

	//ムービースキップ
	if (input->isKeyTrigger(DIK_K))
	{
		cameraObj->SetStartMovieSkip(false);
		startPlayer->SetPosition({ 0,-200,610 });
		movieSkipFlag = true;
	}
	
	//スタートムービー後の自機(仮)の場所
	if (startPlayer->GetPosition().z >= 600 && boss->GetBossDead() == false)
	{
		startPlayer->SetPosition({0,-200,610});
		movieSkipFlag = true;
	}

	//csvのenemy発生
	UpdateEnemyPop();

	//チュートリアル
	if (mojiHp <= -1)
	{
		tutorialFlag = false;
		sceneChange = true;	
	}

	cameraObj->SetTutorialFlag(tutorialFlag);
	//リストけす
	bills.remove_if([](std::unique_ptr<Bill>& bill)
	{
		return bill->billDead();
	});

	oneWays.remove_if([](std::unique_ptr<EnemyOneWay>& oneWay)
	{
		return oneWay->GetIsDead();
	});

	circles.remove_if([](std::unique_ptr<EnemyCircle>& circle)
	{
		return circle->GetIsDead();
	});
	
	//ビル更新処理
	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->SetCameraZ(cameraObj->GetEye().z);
		bill->Update();
	}

	//スカイドーム
	 //カメラの移動量より少し遅く動く
	skydomeZ += cameraObj->GetEyeVec().z * 0.8;
	skydome->SetPosition({ 0,-220,skydomeZ });
	skydome->Update();

	//設置物の更新
	wallFloor->Update();
	road->Update();
	shotHibiObj->Update();
	kanbanObj->Update();
	kanbanPlaneObj->Update();

	if (cameraObj->GetStartGameFlag() == true && setObjectFlag == false)
	{
		shotObj->SetPosition({ 0,0,90 });
		player->SetPlayerPos({ 0,0,0 });
		setObjectFlag = true;
	}
	shotObj->Update();
	//弾の発射説明
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
	
	kanbanShotObj->Update();
	kanbanShot2Obj->Update();
	kanbanShot3Obj->Update();
	kanbanShot4Obj->Update();
	
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

	//自機
	player->SetStartFlag(cameraObj->GetStartGameFlag());
	player->SetEnemyFlag(enemyL->IsDead());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->SetPlayerHpBar(hpBar);
	player->Update();

	// 敵
	//oneway更新処理
	for (std::unique_ptr<EnemyOneWay>& oneWay : oneWays)
	{
		CheckAllCollision(oneWay->GetEnemy());
		oneWay->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay->SetPlayerPosition(player->GetWorldPosition());
		oneWay->Update();
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

	bossChildLUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());

	//当たり判定
	
		CheckAllCollision(boss->GetEnemy());
		CheckBossANDChildCollision(bossChildLUF->GetEnemy());
		CheckBossANDChildCollision(bossChildLUB->GetEnemy());
		CheckBossANDChildCollision(bossChildRUF->GetEnemy());
		CheckBossANDChildCollision(bossChildRUB->GetEnemy());
		CheckBossANDChildCollision(bossChildLDF->GetEnemy());
		CheckBossANDChildCollision(bossChildLDB->GetEnemy());
		CheckBossANDChildCollision(bossChildRDF->GetEnemy());
		CheckBossANDChildCollision(bossChildRDB->GetEnemy());
		CheckPillarCollision();
	

	if (
		bossChildLUF->GetMiniDead() == true&&
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

	//おわり
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
				if (player->GetWorldPosition().z >= 1400)
				{
						pointsLast = true;
				}
			}
			else
			{
				cameraObj->SetEndMovieFlag(true);
			}
		}
		else
		{
			BossCreateParticle(boss->GetPos());
		}
		
	}

	

	//プレイヤーのhpが０になったら
	if (player->GetHp0() == true)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);
		//パーティクル生成
		PlayerCreateParticle(player->GetWorldPosition());
		
		//bossのやられた演出まち用
		dieTimer--;

		if (dieTimer <= 0)
		{
			//シーンチェンジ
			playerDieFlag = true;

			dieTimer = 120;
		}
		
	}
	//ゲームが終わったらビルを消す
	if (pointsLast == true || playerDieFlag == true)
	{
		for (std::unique_ptr<Bill>& bill : bills)
		{
			bill->SetBillDeadFlag(true);
		}
	}

	startPlayer->Update();
	Particle->Update();
}

void GameScene::Draw()
{
	
	
	/// ここに3Dオブジェクトの描画処理
	Object3d::PreDraw(dxcommon->GetCmdlist());

	//スカイドーム
	skydome->Draw();

	wallFloor->Draw();
	road->Draw();

	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Draw();
	}
	if (mojiHp >= 9)
	{
		shotObj->Draw();
	}
	else if (mojiHp >= 0)
	{
		shotHibiObj->Draw();
	}

	kanbanObj->Draw();

	kanbanPlaneObj->Draw();
	kanbanShotObj->Draw();
	kanbanShot2Obj->Draw();
	kanbanShot3Obj->Draw();
	kanbanShot4Obj->Draw();

	startPlayer->Draw();

	player->Draw();
	
	enemy->Draw();
	
	for (std::unique_ptr<EnemyOneWay>& oneWay : oneWays)
	{
		oneWay->Draw();
	}
	for (std::unique_ptr<EnemyCircle>& circle : circles)
	{
		circle->Draw();
	}
	
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

	barrier->Draw();
	
	Particle->Draw(dxcommon->GetCmdlist());
	
	Object3d::PostDraw();
// 3Dオブクジェクトの描画おわり

	spriteCommon->PreDraw();
	if (gameflag == 0)
	{

		titleSprite->Draw();
		reticleSprite->Draw();
	
		//debugtext_minute->DrawAll();
		//debugtext_minute2->DrawAll();
	}
	else if (gameflag == 1)
	{
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
	else if (gameflag == 2)
	{
		endSprite->Draw();
	}
	else if (gameflag == 3)
	{
		gameOverSprite->Draw();
	}
}

void GameScene::CheckAllCollision(Enemy* enemy)
{
	if (mutekiFlag == true)
	{
		return;
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
			if (boss->GetEnemy()->IsDead() == false)
			{
				player->OnCollision();
				//パーティクル生成
				PlayerCreateParticle(player->GetWorldPosition());
			}

			bullet->OnCollision();
		}
	}

	

	
	//自弾と敵当たり判定
	pos1 = enemy->GetWorldPosition();

	//当たり判定の調整
	float pos1Add = 4;
	float pos1BossAdd = 12;
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		
		if (pos2.z <= pos1.z + pos1Add && pos2.z >= pos1.z - pos1Add &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if (cameraObj->GetRaleIndex() <= 5)
			{
				enemy->OnCollision();

				bullet->OnCollision();
				//パーティクル生成
				EnemyCreateParticle(enemy->GetWorldPosition());
			}
			
		}
		if (pos2.z <= pos1.z + pos1BossAdd && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1BossAdd && pos2.x >= pos1.x - pos1BossAdd &&
			pos2.y <= pos1.y + pos1BossAdd && pos2.y >= pos1.y - pos1Add)
		{
			if(cameraObj->GetRaleIndex() >= 6 )
			{
				enemy->OnBossCollision();
				if (boss->GetBarrierFlag() == false)
				{
					BossCreateParticle(enemy->GetWorldPosition());
				}
			
				if (boss->GetEnemy()->IsDead() == true)
				{
					BossCreateParticle(enemy->GetWorldPosition());
				}
				bullet->OnCollision();
			}
		}
	}
}

void GameScene::CheckBossANDChildCollision(Enemy* bossChild)
{
	if (mutekiFlag == true)
	{
		return;
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
			if (boss->GetEnemy()->IsDead() == false)
			{
				player->OnCollision();
				//パーティクル生成
				PlayerCreateParticle(player->GetWorldPosition());
			}

			bullet->OnCollision();
		}
	}

	//自弾と敵当たり判定
	pos1 = bossChild->GetWorldPosition();

	//当たり判定の調整
	float pos1Add = 4;

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
					BossCreateParticle(bossChild->GetWorldPosition());
				}
				else
				{
					bossChild->OnBossMiniCollision();
					BossCreateParticle(bossChild->GetWorldPosition());
				}
				bullet->OnCollision();
			}
		}
	}
	
}

void GameScene::CheckPillarCollision()
{
	if (mutekiFlag == true)
	{
		return;
	}
	XMFLOAT3 pos1, pos2;

	//自弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = shotObj->GetPosition();
	pos1.y = pos1.y + 10;

	float pos1Addx = 32;
	float pos1Addy = 16;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		
		if (pos2.z >= pos1.z && //z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			mojiHp >= 1 //alive
			)
		{
			bullet->OnCollision();
			mojiHp--;
			//パーティクル生成
			CreateParticle(30,48, bullet->GetWorldPosition(), 2.8f,0.02f,
				12.0f,0.0f, { 1,0,0 }, { 0.5,0.3,0.17 });
		}
		else if (pos2.z >= pos1.z && //z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			mojiHp >= 0 //alive
			)
		{
			mojiHp=-1;
			MojiBreakParticle({ shotObj->GetPosition().x ,  shotObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z });
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

		/*	enemyCircle = new EnemyCircle();
			enemyCircle->Init(enemyRotateModel, pos, LorR, attackFlag);*/
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
		XMFLOAT3 pos{};

		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = 0.8f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.031f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 5.0f, 1.0f, { 1,1,1 }, { 1,0,0 });
	}

}

void GameScene::EnemyCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 30; i++)
	{
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.02f;
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//Add(5, pos, vel, acc);
		Particle->Add(64, pos, vel, acc, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
	}

}

void GameScene::BossCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 30; i++)
	{
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y + 10;
		pos.z = position.z;

		const float rnd_vel = 1.3f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.011f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 25.0f, 2.0f, { 1,1,1 }, { 1,0.5,0 });
	}
}

void GameScene::MojiCreateParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 30; i++)
	{
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = 2.8f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.02f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(48, pos, vel, acc, 12.0f, 0.0f, { 1,0,0 }, { 0.5,0.3,0.17 });
	}
}

void GameScene::MojiBreakParticle(XMFLOAT3 position)
{
	//パーティクル
	for (int i = 0; i < 30; i++)
	{

		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = 2.8f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.081f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(64, pos, vel, acc, 10.0f, 0.0f, { 0,1,0 }, { 0.5,0.3,0.17 });
	}
}

void GameScene::CreateParticle(int particleCount,int lifeTime, XMFLOAT3 position, float vec, float accel,
	float start_scale, float end_scale,XMFLOAT3 start_color, XMFLOAT3 end_color)
{
	//パーティクル
	for (int i = 0; i < particleCount; i++)
	{
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = vec;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = accel;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		Particle->Add(lifeTime, pos, vel, acc, start_scale, end_scale, start_color, end_color);
	}
}

void GameScene::BillCreate()
{
	const float iMax = 600;
	const float iAdd = 40;
	const float startZ = 130;
	const float iMax2 = 1000;

	for (float i = 0; i < iMax; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 80,-32,startZ + i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -80,-32,startZ + i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 200,-32,0 - i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -200,-32,0 - i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 80,-32,0 - i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill生成
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -80,-32,0 - i }, { 4,13,4 });

		//bill登録
		bills.push_back(std::move(newBill));//move はユニークから譲渡するため
	}


}


