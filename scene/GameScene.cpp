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
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxcommon = dxCommon;
	this->input = input;
	this->audio = audio;
	this->win = win;

	//���f���ǂݍ���
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
	deadModel = Model::LoadFromOBJ("dead");

	bossHpBar.x = 733;
	bossHpBarMax = 733;
	dieTimer = 120;

	pointsLast = false;

	bossFlag = false;

	hpBar.x = 288;
	hpBarMax = 288;

	//�v���C���[���|���ꂽ�p
	playerDieFlag = false;

	//�I�u�W�F�N�g�̏�����
	skydome->SetModel(skydome_model);
	skydome->scale = { skydomeScale };
	skydome->SetPosition({ skydomePos });

	deadObj->SetModel(deadModel);
	deadObj->scale = { deadObjScale };
	deadObj->SetPosition({ deadObjPos });

	targetObj->SetModel(targetModel);
	targetObj->scale = { targetObjScale };
	targetObj->SetPosition({ targetObjPosition });
	targetObj->rotation.y = targetObjRotY;

	clickObj->SetModel(clickModel);
	clickObj->scale = { clickObjScale };
	clickObj->SetPosition({ clickObjPos });

	kanbanObj->SetModel(kanbanModel);
	kanbanObj->scale = { kanbanObjScale };
	kanbanObj->SetPosition({ kanbanObjPos });
	
	kanbanPlaneObj->SetModel(kanbanPlaneModel);
	kanbanPlaneObj->scale = { kanbanPlaneObjScale };
	kanbanPlaneObj->SetPosition({ kanbanPlaneObjPos });
	kanbanShotObj->SetModel(kanbanShotModel);
	kanbanShotObj->scale = { kanbanPlaneObjScale };
	kanbanShotObj->SetPosition({ kanbanShotPos });
	kanbanShot2Obj->SetModel(kanbanShot2Model);
	kanbanShot2Obj->scale = { kanbanPlaneObjScale };
	kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
	kanbanShot3Obj->SetModel(kanbanShot3Model);
	kanbanShot3Obj->scale = { kanbanPlaneObjScale };
	kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
	kanbanShot4Obj->SetModel(kanbanShot4Model);
	kanbanShot4Obj->scale = { kanbanPlaneObjScale };
	kanbanShot4Obj->SetPosition({ kanbanShotPosDown });

	titleObj->SetModel(shotObjModel);
	titleObj->scale = { titleObjScale };
	titleObj->SetPosition({ titleObjPos });

	clearObj->SetModel(clearModel);
	clearObj->scale = { clearObjScale };
	clearObj->SetPosition({ clearObjPos });

	player = new Player();
	player->Init(playerModel, bulletModel);

	startPlayer->SetModel(playerModel);
	startPlayer->scale = { startPlayerScale };
	startPlayer->SetPosition({ startPlayerPos });

	boss = new Boss();
	boss->Init(bossModel, enemyBulletModel, barrierModel,{ 0,0,0 });//csv�ō��W�ǂݍ��݂���

	bossChildLUF = new BossChild();
	bossChildLUF->Init(bossModel, { bossPosInit }, 1);//csv�ō��W�ǂݍ��݂���
	bossChildLUB = new BossChild();
	bossChildLUB->Init(bossModel, { bossPosInit }, 2);//csv�ō��W�ǂݍ��݂���
	bossChildRUF = new BossChild();
	bossChildRUF->Init(bossModel, { bossPosInit }, 3);//csv�ō��W�ǂݍ��݂���
	bossChildRUB = new BossChild();		  
	bossChildRUB->Init(bossModel, { bossPosInit }, 4);//csv�ō��W�ǂݍ��݂���
	bossChildLDF = new BossChild();		  
	bossChildLDF->Init(bossModel, { bossPosInit }, 5);//csv�ō��W�ǂݍ��݂���
	bossChildLDB = new BossChild();		  
	bossChildLDB->Init(bossModel, { bossPosInit }, 6);//csv�ō��W�ǂݍ��݂���
	bossChildRDF = new BossChild();		  
	bossChildRDF->Init(bossModel, { bossPosInit }, 7);//csv�ō��W�ǂݍ��݂���
	bossChildRDB = new BossChild();		  
	bossChildRDB->Init(bossModel, { bossPosInit }, 8);//csv�ō��W�ǂݍ��݂���

	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(lightGroup);

	camera = new Camera();
	camera->Init();
	
	cameraObj = new CameraObj();
	cameraObj->Init({ cameraPosInit },{ cameraRotInit });

	//��
	wallFloor->SetModel(wallFlatModel);
	wallFloor->scale = { floorScale };
	wallFloor->SetPosition({ floorPos });

	//���̓��H
	road->SetModel(roadModel);
	road->scale = { roadScale };
	road->SetPosition({ roadPos });

	Object3d::SetCamera(camera);

	//�G�̏���csv���璊�o
	EnemyPopLoadData();

	tutorialFlag = true;

	// 3D�I�u�W�F�N�g����
	Particle = ParticleManager::Create(dxcommon->GetDev(), camera);
	Particle->LoadTexture(L"Resource/effect1.png");

	Particle2 = ParticleManager::Create(dxcommon->GetDev(), camera);
	Particle2->LoadTexture(L"Resource/Thunder.png");

	particleEnemy = ParticleManager::Create(dxcommon->GetDev(), camera);
	particleEnemy->LoadTexture(L"Resource/effect2.png");
	billM = new BillManager();
	billM->BillSet(billModel);
	//�r������

	//�X�v���C�g������
	spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	//���e�B�N��
	spriteCommon->LoadTexture(0, L"Resource/target.png");
	reticleSprite = Sprite::Create(spriteCommon, 0);
	reticleSprite->SetPosition({ reticleSpritePos });
	reticleSprite->SetSize({ reticleSpriteSize });
	reticleSprite->TransVertexBuffer();


	//�{�X��hp�o�[�̘g
	spriteCommon->LoadTexture(4, L"Resource/hpWaku.png");
	bossHpWakuSprite = Sprite::Create(spriteCommon, 4);
	bossHpWakuSprite->SetPosition({ bossHpWakuSpritePos });
	bossHpWakuSprite->SetSize({ bossHpWakuSpriteSize });
	bossHpWakuSprite->TransVertexBuffer();

	//�{�X��hp�o�[
	spriteCommon->LoadTexture(5, L"Resource/hpBar.png");
	bossHpBarSprite = Sprite::Create(spriteCommon, 5, { 0,0 });
	bossHpBarSprite->SetPosition({ bossHpBarSpritePos });

	//�v���C���[��hp
	spriteCommon->LoadTexture(6, L"Resource/heart.png");
	playerHpSprite = Sprite::Create(spriteCommon, 6);
	playerHpSprite->SetPosition({ playerHpSpritePos });
	playerHpSprite->SetSize({ playerHpSpriteSize });
	playerHpSprite->SetTexsize({ playerHpSpriteTexsize });
	playerHpSprite->TransVertexBuffer();

	//movie�@skip�p
	spriteCommon->LoadTexture(11, L"Resource/kSkip.png");
	kSkipSprite = Sprite::Create(spriteCommon, 11);
	kSkipSprite->SetPosition({ SkipSpritePos });
	kSkipSprite->SetSize({ SkipSpriteSize });
	kSkipSprite->TransVertexBuffer();

	//�f�o�b�O�e�L�X�g
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
	
	
//���ʏ���
	//�Ə�
	reticleSprite->Update();
	reticleSprite->SetPosition({ mouseX,mouseY,0 });

	playerHpSprite->Update();//player��hp
	kSkipSprite->Update();//���[�r�[�X�L�b�v
	//�f�o�b�O�e�L�X�g
	debugtext_minute->Print(moji, debugTextPos.x, debugTextPos.y, debugTextScale);
	debugtext_minute2->Print(moji2, debugTextPos2.x, debugTextPos2.y, debugTextScale);

	
//�f�o�b�O�p�R�}���h
	//�v���C���[�̂����ւ�
	if (input->isKeyTrigger(DIK_P))
	{
		player->OnCollision();
	}
	//�����蔻�肯��
	if (input->isKeyTrigger(DIK_C) && mutekiFlagDeb == true)
	{
		mutekiFlagDeb = false;
	}
	//�����蔻��߂�
	else if (input->isKeyTrigger(DIK_C))
	{
		mutekiFlagDeb = true;
	}
	//�{�X��hp ������
	int bossHpAttack = 35;
	if (input->isKeyTrigger(DIK_Q))
	{
		for (int i = 0; i < bossHpAttack; i++)
		{
			boss->GetEnemy()->OnBossCollision();
		}		
	}
	//�N���A�V�[��
	if (input->isKeyTrigger(DIK_N))
	{
		pointsLast = true;
		scene_ = Scene::Clear;
		sceneDraw_ = SceneDraw::ClearDraw;
	}
	//�Q�[���I�[�o�[�V�[��
	if (input->isKeyTrigger(DIK_M))
	{
		playerDieFlag = true;
		scene_ = Scene::GameOver;
		sceneDraw_ = SceneDraw::GameOverDraw;
	}

	//�r���X�V����
	billM->Update();

	//�X�J�C�h�[��
	skydome->SetPosition({ skydomePos });
	skydome->Update();

	//�ݒu���̍X�V
	wallFloor->Update();
	road->Update();
	kanbanObj->Update();
	kanbanPlaneObj->Update();
	deadObj->Update();
	targetObj->Update();
	
	//2d���e�B�N���X�v���C�g�̍��W
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//�f�o�b�O�p
	sprintf_s(moji, "%f", boss->GetPos().y);
	sprintf_s(moji2, "%f", boss->GetPos().z);

	//�J�����̒����_�Z�b�g
	cameraObj->SetTargetS(startPlayer->GetPosition());
	cameraObj->SetPlayerDieFlag(player->GetHp0());
	cameraObj->SetTarget(player->GetWorldPosition());
	cameraObj->SetBoss(boss->GetPos(), boss->GetBossDead());
	cameraObj->UpdateCamera();

	//�J����
	camera->SetEye({ cameraObj->GetEye() });
	camera->SetTarget({ cameraObj->GetTarget() });
	camera->SetUp({ cameraObj->GetUp() });
	camera->UpdateCamera();

	//���e�B�N��
	player->SetHwnd(hwnd);
	player->SetViewPort(viewPort);
	player->SetCameraMatViewProjection(cameraObj->GetMatViewProjection());
	
	//�v���C���[�̃G���W�����o
	Particle->CreateParticlePlayer(particleCountEngine, particleLifeEngine, { startPlayer->position.x,startPlayer->position.y,startPlayer->position.z - particleEngineAdd },
		particleVelocityEngine, particleAccelEngine, particleStartScaleEngine, particleEndScaleEngine,
		particleStartColorEngine, particleEndColorEngine);

	Particle->CreateParticlePlayer(particleCountEngine, particleLifeEngine, { player->GetWorldPosition().x,player->GetWorldPosition().y,player->GetWorldPosition().z - particleEngineAdd },
		particleVelocityEngine, particleAccelEngine, particleStartScaleEngine, particleEndScaleEngine,
		particleStartColorEngine, particleEndColorEngine);

	player->Update();
	startPlayer->Update();
	Particle->Update();
	Particle2->Update();
	particleEnemy->Update();
	lightGroup->Update();
	
	//���s�����@�@�@
	lightGroup->SetAmbientColor({ lightColor });//���C�g�J���[
	lightGroup->SetDirLightDir(0, lightPos);//�ꏊ
	lightGroup->SetDirLightActive(0, true);//�L����
	//�ۉe
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowDir(0, lightCircleShadowDir);
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(player->GetWorldPosition()));
	lightGroup->SetCircleShadowAtten(0, lightCircleShadowAtten);
	lightGroup->SetCircleShadowFactorAngle(0, lightCircleShadowFactorAngle);

}

void GameScene::Draw()
{	
	/// ������3D�I�u�W�F�N�g�̕`�揈��
	Object3d::PreDraw(dxcommon->GetCmdlist());

	skydome->Draw();//�X�J�C�h�[��
	wallFloor->Draw();//��
	road->Draw();//��

	billM->Draw();

	startPlayer->Draw();
	Object3d::PostDraw();
	// 3D�I�u�N�W�F�N�g�̕`�您���
	(this->*spFuncTableDraw[phaseDrawNumber])();
	/// ������3D�I�u�W�F�N�g�̕`�揈��
	Object3d::PreDraw(dxcommon->GetCmdlist());
	
	Particle->Draw(dxcommon->GetCmdlist());
	Particle2->Draw(dxcommon->GetCmdlist());
	particleEnemy->Draw(dxcommon->GetCmdlist());
	Object3d::PostDraw();
// 3D�I�u�N�W�F�N�g�̕`�您���


	
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
		oneWay2->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay2->Update();
	}
	//csv��enemy����
	UpdateEnemyPop();

	ParticleTitle();

}

void GameScene::Game()
{
	//sprite
		//�{�X��hp
	bossHpBarSprite->SetSize({ bossHpBar.x,bossHpBar.y });
	bossHpBarSprite->TransVertexBuffer();

	bossHpBarSprite->Update();//�{�X��hp�o�[
	bossHpWakuSprite->Update();//�{�X��hp�o�[�̘g

	//player��hp
	playerHpSprite->SetPosition({ playerHpSpritePos });
	playerHpSprite->SetSize({ hpBar.x,hpBar.y });
	playerHpSprite->SetTexsize({ hpBar.x,hpBar.y });
	playerHpSprite->TransVertexBuffer();

	//3d�ݒu��
		//�X�J�C�h�[��
		//�J�����̈ړ��ʂ�菭���x������
	skydomePos.z += cameraObj->GetEyeVec().z * skydomeVec;

	//�X�^�[�g���[�r�[
	if (cameraObj->GetStartMovieFlag() == false)
	{
		//player����ȂȂߑO�ɂƂ΂�
		startPlayer->position.z += startPlayerAddZ;
		startPlayer->position.y += startPlayerAddY;
	}

	//���[�r�[�X�L�b�v
	if (input->isKeyTrigger(DIK_K))
	{
		cameraObj->SetStartMovieSkip(false);
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;//���[�r�[�X�L�b�v�̃X�v���C�g
	}

	//�X�^�[�g���[�r�[��̎��@(��)�̏ꏊ
	if (startPlayer->GetPosition().z >= startPlayerAfterPos.z && boss->GetBossDead() == false)
	{
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;//���[�r�[�X�L�b�v�̃X�v���C�g
	}
	else
	{
		Particle->CreateParticlePlayer(particleCountEngine, particleLifeEngine, { startPlayer->position.x,startPlayer->position.y,startPlayer->position.z - particleEngineAdd },
			particleVelocityEngine, particleAccelEngine, particleStartScaleEngine, particleEndScaleEngine,
			particleStartColorEngine, particleEndColorEngine);
		ParticleTitle();
	}

	//csv��enemy����
	UpdateEnemyPop();

	//�`���[�g���A����������Ă��邩���J�����ɋ�����
	cameraObj->SetTutorialFlag(tutorialFlag);

	//���[�r�[���I�������̃`���[�g���A���p��obj�@�Ɓ@player�̈ʒu�ݒ�
	if (cameraObj->GetStartGameFlag() == TRUE && setObjectFlag == false)
	{
		targetObj->SetPosition({ InGametargetObjPos });
		player->SetPlayerPos({ playerPos });
		setObjectFlag = true;
	}
	//�e�̔��ː���obj�@�@(��ʉE�̊Ŕ�)(�A�j���[�V��������)
	if (cameraObj->GetStartGameFlag() == TRUE)
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
	//�e�̔��ː���obj�@�̍X�V����
	kanbanShotObj->Update();
	kanbanShot2Obj->Update();
	kanbanShot3Obj->Update();
	kanbanShot4Obj->Update();


	//���@
	player->SetStartFlag(cameraObj->GetStartGameFlag());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->SetPlayerHpBar(hpBar.x);

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//���@�̒e�̃p�[�e�B�N��
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet1 = bullet->GetWorldPosition();

		//�U�����p�[�e�B�N�����ł�
		if (input->isMouseKey())
		{
			attackParticleFlag = true;
		}
		if (attackParticleFlag == true)
		{
			Particle2->CreateParticle(particleCountBullet, particleLifeBullet, bullet1,
				particleVelocityBullet, particleAccelBullet, 
				particleStartScaleBullet, particleEndScaleBullet, 
				particleStartColorBullet, particleEndColorBullet);
		}
	}

	// �G
	//oneway�X�V����
	for (std::unique_ptr<EnemyOneWay>& oneWay : oneWays)
	{
		CheckAllCollision(oneWay->GetEnemy());
		oneWay->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay->SetPlayerPosition(player->GetWorldPosition());
		lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(oneWay->GetEnemy()->GetWorldPosition()));

		oneWay->Update();
	}

	for (std::unique_ptr<EnemyOneWay>& oneWay2 : oneWayMovies)
	{
		CheckAllCollision(oneWay2->GetEnemy());
		oneWay2->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		oneWay2->Update();
	}

	//circle�X�V����
	for (std::unique_ptr<EnemyCircle>& circle : circles)
	{
		CheckAllCollision(circle->GetEnemy());
		circle->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		circle->SetPlayerPosition(player->GetWorldPosition());
		lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(circle->GetEnemy()->GetWorldPosition()));

		circle->Update();
	}

	//�{�X
	boss->SetPlayerWorldPos(player->GetWorldPosition());
	boss->GetEnemy()->SetBossHpBar(bossHpBar.x, bossHpBarMax);
	boss->Update();

	//�{�X�̎�����܂��G

	 //�{�X�̎�����܂��U���̂���
	 //�{�X�ɒǏ]���邽��
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


	//�{�X���o���A�𒣂�����
	bossChildLUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRUB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildLDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDF->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());
	bossChildRDB->SetBarrierPhaseFlag(boss->GetBarrierPhaseFlag());

	//�{�X�n�����蔻��
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

	//�{�X�̃o���A������
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
	bossHpBar.x = boss->GetEnemy()->GetHpBarX();
	hpBar.x = player->GetHpBar();

	//�{�X��͂��܂� (�J�������~�܂�)
	if (cameraObj->GetEndFlag() == TRUE)
	{
		bossFlag = true;
	}

	//�{�X���|���ꂽ��
	if (boss->GetEnemy()->IsDead() == true)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);

		//�{�X�̉��o�����������
		if (boss->GetEnemy()->GetWorldPosition().y <= floorY)
		{
			//�V�[���`�F���W�܂��p
			dieTimer--;
			if (dieTimer <= 0)
			{
				player->SetEndMovieFlag(true);
				//�V�[���`�F���W
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
			particleEnemy->CreateParticle(particleCountBossDie, particleLifeBossDie, boss->GetPos(),
				particleVelocityBossDie, particleAccelBossDie,
				particleStartScaleBossDie, particleEndScaleBossDie,
				particleStartColorBossDie, particleEndColorBossDie);
		}
	}

	


	//�v���C���[��hp���O�ɂȂ�����
	if (player->GetHp0() == TRUE)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);
		//�p�[�e�B�N������
		Particle->CreateParticle(particleCountPlayerDie, particleLifePlayerDie, player->GetWorldPosition(), 
			particleVelocityPlayerDie, particleAccelPlayerDie,
			particleStartScalePlayerDie, particleEndScalePlayerDie,
			particleStartColorPlayerDie, particleEndColorPlayerDie);

		//���ꂽ���o�܂��p
		dieTimer--;

		if (dieTimer <= 0)
		{
			//�V�[���`�F���W
			playerDieFlag = true;
			scene_ = Scene::GameOver;
			sceneDraw_ = SceneDraw::GameOverDraw;
			dieTimer = dieTimerMax;
		}

	}
	//�Q�[�����I�������r��������
	if (pointsLast == true || playerDieFlag == true)
	{
		billM->SetBillDeadFlag(true);
	}

	//�����𖞂�����list����

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
	ParticleClear();
	clearObj->rotation.y++;
	clearObj->SetPosition({ clearSceneClearObjPos.x,clearSceneClearObjPos.y ,clearSceneClearObjPos.z + cameraObj->GetEye().z });
	clickCount++;
	clickObj->SetPosition({ ClearclickObjPos.x,ClearclickObjPos.y,ClearclickObjPos.z + cameraObj->GetEye().z });
	clearObj->Update();
	clickObj->Update();

}

void GameScene::GameOver()
{
	cameraObj->SetStopFlag(true);
	deadObj->rotation.y++;
	deadObj->SetPosition({ GameOverdeadObjPos.x,GameOverdeadObjPos.y,GameOverdeadObjPos.z + cameraObj->GetEye().z  });

	deadObj->Update();
	clickCount++; 
	clickObj->SetPosition({ GameOverclickObjPos.x,GameOverclickObjPos.y,GameOverclickObjPos.z + cameraObj->GetEye().z });
	clickObj->Update();
}

void GameScene::TitleDraw()
{
	/// ������3D�I�u�W�F�N�g�̕`�揈��
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
	// 3D�I�u�N�W�F�N�g�̕`�您���
	spriteCommon->PreDraw();
	reticleSprite->Draw();
}

void GameScene::GameDraw()
{
	/// ������3D�I�u�W�F�N�g�̕`�揈��
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

	if (cameraObj->GetEndFlag() == TRUE)
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
	// 3D�I�u�N�W�F�N�g�̕`�您���

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

	//�f�o�b�O�e�L�X�g
	//debugtext_minute->DrawAll();
	//debugtext_minute2->DrawAll();
}

void GameScene::ClearDraw()
{
	/// ������3D�I�u�W�F�N�g�̕`�揈��
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
}

void GameScene::GameOverDraw()
{
	/// ������3D�I�u�W�F�N�g�̕`�揈��
	Object3d::PreDraw(dxcommon->GetCmdlist());
	if (clickCount >= clickAliveCount)
	{
		clickObj->Draw();
		if (clickCount >= clickDeadCount)
		{
			clickCount = clickCountReset;
		}
	}
	deadObj->Draw();
	Object3d::PostDraw();
	// 3D�I�u�N�W�F�N�g�̕`�您���
}

//�����蔻��
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
			if (boss->GetEnemy()->IsDead() == false && mutekiFlag == false)
			{
				player->OnCollision();
				//�p�[�e�B�N������
				Particle->CreateParticle(particleCountPlayer, particleLifePlayer, player->GetWorldPosition(),
					particleVelocityPlayer, particleAccelPlayer,
					particleStartScalePlayer, particleEndScalePlayer,
					particleStartColorPlayer, particleEndColorPlayer);

				mutekiFlag = true;
			}

			bullet->OnCollision();
		}
	}

	//���e�ƓG�����蔻��
	pos1 = enemy->GetWorldPosition();

	//�����蔻��̒���
	float pos1Add = 4;
	float pos1AddZ = 4;//z���W�ɒǉ�����
	float pos1BossAdd = 12;//�{�X�̒���
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		//�G���G�ƒe�̓����蔻��
		if (pos2.z <= pos1.z + pos1Add + pos1AddZ && pos2.z >= pos1.z - pos1Add - pos1AddZ &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if (cameraObj->GetRaleIndex() <= 5)
			{
				enemy->OnCollision();

				bullet->OnCollision();


				//�p�[�e�B�N������
				particleEnemy->CreateParticle(particleCountEnemy, particleLifeEnemy, enemy->GetWorldPosition(),
					particleVelocityEnemy, particleAccelEnemy,
					particleStartScaleEnemy, particleEndScaleEnemy,
					particleStartColorEnemy, particleEndColorEnemy);
			}

		}
		//�{�X�ƒe�̓����蔻��
		if (pos2.z <= pos1.z + pos1BossAdd && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1BossAdd && pos2.x >= pos1.x - pos1BossAdd &&
			pos2.y <= pos1.y + pos1BossAdd && pos2.y >= pos1.y - pos1Add)
		{
			if (cameraObj->GetRaleIndex() >= 6)
			{
				enemy->OnBossCollision();
				if (boss->GetBarrierFlag() == false && boss->GetBossMovieFlag() == false)
				{

					particleEnemy->CreateParticle(particleCountBoss, particleLifeBoss, enemy->GetWorldPosition(),
						particleVelocityBoss, particleAccelBoss,
						particleStartScaleBoss, particleEndScaleBoss,
						particleStartColorBoss, particleEndColorBoss);
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
	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//�G�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bossChild->GetBullets();


	pos1 = boss->GetEnemy()->GetWorldPosition();
	//boss��bossmini �e�����蔻��
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
	//���L�����ƓG�e�����蔻��
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
				//�p�[�e�B�N������
				Particle->CreateParticle(particleCountPlayer, particleLifePlayer, player->GetWorldPosition(),
					particleVelocityPlayer, particleAccelPlayer,
					particleStartScalePlayer, particleEndScalePlayer,
					particleStartColorPlayer, particleEndColorPlayer);
				mutekiFlag = true;
			}

			bullet->OnCollision();
		}
	}

	//���e�ƓG�����蔻��
	pos1 = bossChild->GetWorldPosition();

	//�����蔻��̒���
	float pos1Add = 4;

	//�{�X�̎�����܂��G�̓����蔻��
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();

		if (pos2.z <= pos1.z + pos1Add && pos2.z >= pos1.z &&
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if (boss->GetBarrierPhaseFlag() == TRUE)
			{
				int particleCountBossChild = 30;
				int particleLifeBossChild = 32;
				XMFLOAT3 particleVelocityBossChild = { 1.3f, 1.3f, 1.3f };
				XMFLOAT3 particleAccelBossChild = { 0.01f, 0.01f, 0.01f };
				float particleStartScaleBossChild = 25.0f;
				float particleEndScaleBossChild = 2.0f;
				XMFLOAT3 particleStartColorBossChild = { 1.0f,1.0f,1.0f };
				XMFLOAT3 particleEndColorBossChild = { 1.0f,0.5f,0.0f };
				if (bossChild->IsDead() == true)
				{
					particleEnemy->CreateParticle(particleCountBossChild, particleLifeBossChild, bossChild->GetWorldPosition(),
						particleVelocityBossChild, particleAccelBossChild,
						particleStartScaleBossChild, particleEndScaleBossChild,
						particleStartColorBossChild, particleEndColorBossChild);
				}
				else
				{
					bossChild->OnBossMiniCollision();
					particleEnemy->CreateParticle(particleCountBossChild, particleLifeBossChild, bossChild->GetWorldPosition(),
						particleVelocityBossChild, particleAccelBossChild,
						particleStartScaleBossChild, particleEndScaleBossChild,
						particleStartColorBossChild, particleEndColorBossChild);
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

	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = targetObj->GetPosition();
	pos1.y = pos1.y + 10;

	float pos1Addx = 6;
	float pos1Addy = 6;
	float pos1Addz = 8;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{

		pos2 = bullet->GetWorldPosition();
		//�`���[�g���A���� �I �Ƀ_���[�W
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

				//�p�[�e�B�N������
				Particle->CreateParticle(particleCountTarget, particleLifeTarget, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , targetObj->GetPosition().z },
					particleVelocityTarget, particleAccelTarget,
					particleStartScaleTarget, particleEndScaleTarget,
					particleStartColorTarget, particleEndColorTarget);
			}
			else if (count == 0)
			{
				count++;
				Particle->CreateParticle(particleCountTarget, particleLifeTarget, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , targetObj->GetPosition().z },
					particleVelocityTarget, particleAccelTarget,
					particleStartScaleTarget, particleEndScaleTarget,
					particleStartColorTarget, particleEndColorTarget);
				targetHp = targetHpMax;
				targetObj->position.x = 50;

			}
			else if (count == 1)
			{
				count++;
				Particle->CreateParticle(particleCountTarget, particleLifeTarget, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , targetObj->GetPosition().z },
					particleVelocityTarget, particleAccelTarget,
					particleStartScaleTarget, particleEndScaleTarget,
					particleStartColorTarget, particleEndColorTarget);
				targetHp = targetHpMax;
				targetObj->position.x = -50;
				targetObj->rotation.y = 165;
			}
			else if (count == 2)
			{
				count++;
				Particle->CreateParticle(particleCountTarget, particleLifeTarget, { targetObj->GetPosition().x ,  targetObj->GetPosition().y + shotObjAddy , targetObj->GetPosition().z },
					particleVelocityTarget, particleAccelTarget,
					particleStartScaleTarget, particleEndScaleTarget,
					particleStartColorTarget, particleEndColorTarget);
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

	//���e���X�g
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

//csv�ǂݍ���
void GameScene::EnemyPopLoadData()
{
	//�t�@�C�����J��
	std::fstream file;
	file.open("Resource/enemy.csv");

	//�t�@�C���̓��e���R�s�[
	enemyPopCommands << file.rdbuf();
	//�t�@�C�������
	file.close();

}

void GameScene::UpdateEnemyPop()
{
	//�ҋ@����
	//�ҋ@���������܂Ł@return�@����if����艺�̏����������Ȃ�
	if (waitCommand == true)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			waitCommand = false;
		}
		return;
	}

	//�ҋ@����
	//�ҋ@���������܂Ł@return�@����if����艺�̏����������Ȃ�
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
			//0���U���@1���U�����Ȃ�
			int attack = std::atof(word.c_str());
			bool attackFlag = true;
			if (attack == 1)
			{
				attackFlag = false;
			}

			//����
			std::unique_ptr<EnemyOneWay> newOneWay = std::make_unique<EnemyOneWay>();
			newOneWay->Init(enemyModel, pos, LorR, attackFlag);

			//�o�^
			oneWays.push_back(std::move(newOneWay));//move �̓��j�[�N������n���邽��
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
			//0���U���@1���U�����Ȃ�
			int attack = std::atof(word.c_str());
			bool attackFlag = true;
			if (attack == 1)
			{
				attackFlag = false;
			}

			//����
			std::unique_ptr<EnemyOneWay> newOneWay2 = std::make_unique<EnemyOneWay>();
			newOneWay2->Init(enemyModel, pos, LorR, attackFlag, true);

			//�o�^
			oneWayMovies.push_back(std::move(newOneWay2));//move �̓��j�[�N������n���邽��
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
			//0���U���@1���U�����Ȃ�
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

			//����
			std::unique_ptr<EnemyCircle> newCircle = std::make_unique<EnemyCircle>();
			newCircle->Init(enemyRotateModel, pos, LorR, attackFlag);

			//�o�^
			circles.push_back(std::move(newCircle));//move �̓��j�[�N������n���邽��

		}
		else if (word.find("BOSS") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			boss = new Boss();
			boss->Init(bossModel, enemyBulletModel, barrierModel, { pos });
		}
		else if (word.find("CHILDLUF") == 0)
		{
			XMFLOAT3 pos = CommandPositionSet(line_stream, word);

			getline(line_stream, word, ',');
			int32_t number = (int32_t)std::atof(word.c_str());

			bossChildLUF = new BossChild();
			bossChildLUF->Init(bossModel, pos, number);
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
			break;//���[�v���甲���đҋ@�����ɍs��
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			waitCommand = true;
			waitTimer = waitTime;

			break;//���[�v���甲���đҋ@�����ɍs��
		}

	}
}

XMFLOAT3 GameScene::CommandPositionSet(std::istream& line_stream, std::string& word)
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

//�p�[�e�B�N��
void GameScene::ParticleClear()
{
	
	//���[�r�[���̃p�[�e�B�N��
	movieParticleTime++;
	//60frame ����@120frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
	if (movieParticleTime >= movieParticleTime60f && movieParticleTime <= movieParticleTime120f)
	{
		movieParticleXL = particleMinPosXTitle;
		movieParticleXR = particleMaxPosXTitle;
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleYClear,cameraObj->GetEye().z + particleAddZClear },
			particleVelocityClear,particleAccelClear, 
			particleStartScaleClear, particleEndScaleClear, 
			particleStartColorClear, particleEndColorClear);
		
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYClear,cameraObj->GetEye().z + particleAddZClear },
			particleVelocityClear,particleAccelClear, 
			particleStartScaleClear, particleEndScaleClear,
			particleStartColorClear, particleEndColorClear);
		//80frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
		if (movieParticleTime >= movieParticleTime80f)
		{
			Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYClear,cameraObj->GetEye().z + particleAddZClear },
				particleVelocityClear,particleAccelClear, 
				particleStartScaleClear, particleEndScaleClear, 
				particleStartColorClear, particleEndColorClear);
		}
	}
	//120frame ����@180frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
	else if (movieParticleTime >= movieParticleTime120f && movieParticleTime <= movieParticleTime180f)
	{
		movieParticleXL = particleMaxPosXTitle;
		movieParticleXR = particleMinPosXTitle;
		Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleYClear,cameraObj->GetEye().z + particleAddZClear },
			particleVelocityClear, particleAccelClear,
			particleStartScaleClear, particleEndScaleClear,
			particleStartColorClear, particleEndColorClear);
		//160frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
		if (movieParticleTime >= movieParticleTime160f)
		{
			Particle->CreateParticleClear(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYClear,cameraObj->GetEye().z + particleAddZClear },
				particleVelocityClear, particleAccelClear,
				particleStartScaleClear, particleEndScaleClear,
				particleStartColorClear, particleEndColorClear);
		}
	}
	else if (movieParticleTime >= movieParticleTime180f)
	{
		//180frame��0��
		movieParticleTime = 0;
	}
}

void GameScene::ParticleTitle()
{
	
	//���[�r�[���̃p�[�e�B�N��
	movieParticleTime++;
	//60frame ����@120frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
	if (movieParticleTime >= movieParticleTime60f && movieParticleTime <= movieParticleTime120f)
	{
		movieParticleXL = particleMinPosXTitle;
		movieParticleXR = particleMaxPosXTitle;
		Particle->CreateParticle(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleYTitle,particleZTitle },
			particleVelocityTitle,particleAccelTitle, 
			particleStartScaleTitle, particleEndScaleTitle, 
			particleStartColorTitle, particleEndColorTitle);
		Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYTitle2,particleZTitle },
			particleVelocityTitle, particleAccelTitle,
			particleStartScaleTitle, particleEndScaleTitle,
			particleStartColorTitle, particleEndColorTitle);
		//80frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
		if (movieParticleTime >= movieParticleTime80f)
		{
			Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYTitle2,particleZTitle },
				particleVelocityTitle, particleAccelTitle,
				particleStartScaleTitle, particleEndScaleTitle,
				particleStartColorTitle, particleEndColorTitle);
		}
	}
	//120frame ����@180frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
	else if (movieParticleTime >= movieParticleTime120f && movieParticleTime <= movieParticleTime180f)
	{
		movieParticleXL = particleMaxPosXTitle;
		movieParticleXR = particleMinPosXTitle;
		Particle->CreateParticle(particleCountTitle, particleLifeTitle, { -movieParticleXL, particleYTitle,particleZTitle },
			particleVelocityTitle, particleAccelTitle,
			particleStartScaleTitle, particleEndScaleTitle,
			particleStartColorTitle, particleEndColorTitle);
		//160frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
		if (movieParticleTime >= movieParticleTime160f)
		{
			Particle->CreateParticle(particleCountTitle, particleLifeTitle, { movieParticleXR, particleYTitle2,particleZTitle },
				particleVelocityTitle, particleAccelTitle,
				particleStartScaleTitle, particleEndScaleTitle,
				particleStartColorTitle, particleEndColorTitle);
		}
	}
	else if (movieParticleTime >= movieParticleTime180f)
	{
		//180frame��0��
		movieParticleTime = 0;
	}
}



