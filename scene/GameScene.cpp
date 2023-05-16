#include "GameScene.h"
#include "time.h"

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

	bossHpBar = 733;
	bossHpBarMax = 733;
	dieTimer = 120;

	pointsLast = false;

	bossFlag = false;

	hpBar = 288;
	hpBarMax = 288;

	//�v���C���[���|���ꂽ�p
	playerDieFlag = false;

	//�I�u�W�F�N�g�̏�����
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

	titleObj->SetModel(shotObjModel);
	titleObj->scale = { 1.5,1.5,1.5 };
	titleObj->SetPosition({ 0,25,-560 });

	player = new Player();
	player->Init(playerModel, bulletModel);

	startPlayer->SetModel(playerModel);
	startPlayer->scale = { 2,2,2 };
	startPlayer->SetPosition({0,0,-550});

	boss = new Boss();
	boss->Init(bossModel, enemyBulletModel, barrierModel,{ 0,0,-200.0f });

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

	//��
	wallFloor->SetModel(wallFlatModel);
	wallFloor->scale = { 65,7,170 };
	wallFloor->SetPosition({ 0,-65,50 });

	//���̓��H
	road->SetModel(roadModel);
	road->scale = { 10,7,600 };
	road->SetPosition({ 0,-55,650 });

	Object3d::SetCamera(camera);

	//�G�̏���csv���璊�o
	EnemyPopLoadData();

	tutorialFlag = true;

	// 3D�I�u�W�F�N�g����
	Particle = ParticleManager::Create(dxcommon->GetDev(), camera);

	//�r������
	BillCreate();

	//�`�揉���������@��������
	//�X�v���C�g������
	spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	//���e�B�N��
	spriteCommon->LoadTexture(0, L"Resource/target.png");
	reticleSprite = Sprite::Create(spriteCommon, 0);
	reticleSprite->SetPosition({ 0.0f,0.0f,0.0f });
	reticleSprite->SetSize({ 100,100 });
	reticleSprite->TransVertexBuffer();

	//�^�C�g��
	spriteCommon->LoadTexture(2, L"Resource/title.png");
	titleSprite = Sprite::Create(spriteCommon, 2);
	titleSprite->SetPosition({ 640.0f,360.0f,0.0f });
	titleSprite->SetSize({ 1280,720 });
	titleSprite->TransVertexBuffer();

	//�G���h
	spriteCommon->LoadTexture(3, L"Resource/end.png");
	endSprite = Sprite::Create(spriteCommon, 3);
	endSprite->SetPosition({ 640.0f,360.0f,0.0f });
	endSprite->SetSize({ 1280,720 });
	endSprite->TransVertexBuffer();

	//�{�X��hp�o�[�̘g
	spriteCommon->LoadTexture(4, L"Resource/hpWaku.png");
	bossHpWakuSprite = Sprite::Create(spriteCommon, 4);
	bossHpWakuSprite->SetPosition({ 640.0f,50.0f,0.0f });
	bossHpWakuSprite->SetSize({ 769,38 });
	bossHpWakuSprite->TransVertexBuffer();

	//�{�X��hp�o�[
	spriteCommon->LoadTexture(5, L"Resource/hpBar.png");
	bossHpBarSprite = Sprite::Create(spriteCommon, 5, { 0,0 });
	bossHpBarSprite->SetPosition({ 273.0f,34.0f,0.0f });

	//�v���C���[��hp
	spriteCommon->LoadTexture(6, L"Resource/heart.png");
	playerHpSprite = Sprite::Create(spriteCommon, 6);
	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ 288,96 });
	playerHpSprite->SetTexsize({ 288,96 });
	playerHpSprite->TransVertexBuffer();

	//�Q�[���I�[�o�[
	spriteCommon->LoadTexture(7, L"Resource/gameover.png");
	gameOverSprite = Sprite::Create(spriteCommon, 7);
	gameOverSprite->SetPosition({ 640.0f,360.0f,0.0f });
	gameOverSprite->SetSize({ 1280,720 });
	gameOverSprite->TransVertexBuffer();

	//movie�@skip�p
	spriteCommon->LoadTexture(11, L"Resource/kSkip.png");
	kSkipSprite = Sprite::Create(spriteCommon, 11);
	kSkipSprite->SetPosition({ 1200.0f,20.0f,0.0f });
	kSkipSprite->SetSize({ 140,32 });
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
	
//�X�v���C�g
	//�{�X��hp
	bossHpBarSprite->SetSize({ bossHpBar,32 });
	bossHpBarSprite->TransVertexBuffer();

	//player��hp
	playerHpSprite->SetPosition({ 1112.0f,640.0f,0.0f });
	playerHpSprite->SetSize({ hpBar,96 });
	playerHpSprite->SetTexsize({ hpBar,96 });
	playerHpSprite->TransVertexBuffer();

	titleSprite->Update();//�^�C�g��
	endSprite->Update();//�N���A
	gameOverSprite->Update();//�Q�[���I�[�o�[
	bossHpBarSprite->Update();//�{�X��hp�o�[
	bossHpWakuSprite->Update();//�{�X��hp�o�[�̘g

	//�Ə�
	reticleSprite->Update();
	reticleSprite->SetPosition({ mouseX,mouseY,0 });

	playerHpSprite->Update();//player��hp
	kSkipSprite->Update();//���[�r�[�X�L�b�v
	//�f�o�b�O�e�L�X�g
	debugtext_minute->Print(moji, 0, 0, 1.0f);
	debugtext_minute2->Print(moji2, 0, 100, 1.0f);

	
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

//�Q�[����
	//�����𖞂�����list����
	bills.remove_if([](std::unique_ptr<Bill>& bill)
		{
			return bill->billDead();
		});
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

	//�r���X�V����
	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->SetCameraZ(cameraObj->GetEye().z);
		bill->Update();
	}

	//�X�J�C�h�[��
	 //�J�����̈ړ��ʂ�菭���x������
	skydomeZ += cameraObj->GetEyeVec().z * skydomeVec;
	skydome->SetPosition({ 0,-220,skydomeZ });
	skydome->Update();

	//�ݒu���̍X�V
	wallFloor->Update();
	road->Update();
	shotHibiObj->Update();
	kanbanObj->Update();
	kanbanPlaneObj->Update();
	shotObj->Update();

	//�^�C�g������X�^�[�g���[�r�[���o��
	if (cameraObj->GetStartMovieFlag() == false && gameStartFlag == false)
	{
		if (input->isMouseKey())
		{
			cameraObj->SetStartMovieSkip(true);
			gameStartFlag = true;
		}
	}
	//�X�^�[�g���[�r�[
	if (gameStartFlag == true && cameraObj->GetStartMovieFlag() == false)
	{
		
		startPlayer->position.z += startPlayerAddZ;
		startPlayer->position.y += startPlayerAddY;
	}

	//���[�r�[�X�L�b�v
	if (gameStartFlag == true && input->isKeyTrigger(DIK_K))
	{
		cameraObj->SetStartMovieSkip(false);
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;
	}
	
	//�X�^�[�g���[�r�[��̎��@(��)�̏ꏊ
	if (startPlayer->GetPosition().z >= startPlayerAfterPos.z && boss->GetBossDead() == false)
	{
		startPlayer->SetPosition(startPlayerAfterPos);
		movieSkipFlag = true;
	}
	else
	{
		//���[�r�[���̃p�[�e�B�N��
		movieParticleTime++;
		 //60frame ����@120frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
		if (movieParticleTime >= 60 && movieParticleTime <= 120)
		{
			movieParticleXL = 40;
			movieParticleXR = 80;
			Particle->CreateParticle(30, 64, { -movieParticleXL, 0,-320 }, { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			Particle->CreateParticle(30, 64, { movieParticleXR, 20,-320 }, { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			//80frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
			if(movieParticleTime >= 80)
			{ 
				Particle->CreateParticle(30, 64, { movieParticleXR, 20,-320 }, { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			}
		}
		 //120frame ����@180frame�@�̂������ɏo���p�[�e�B�N�� (�r�����U������Ă��銴��)
		else if(movieParticleTime >= 120 && movieParticleTime <= 180)
		{
			movieParticleXL = 80;
			movieParticleXR = 40;
			Particle->CreateParticle(30, 64, { -movieParticleXL, 0,-320 }, { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			//160frame ����@�ł�p�[�e�B�N�� (�����Ɠ����ꏊ�ɏo�Ă�ƕς�����)
			if (movieParticleTime >= 160)
			{
				Particle->CreateParticle(30, 64, { movieParticleXR, 20,-320 }, { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
			}
		}
		else if (movieParticleTime >= 180)
		{
			//180frame��0��
			movieParticleTime = 0;
		}
	}

	//csv��enemy����
	UpdateEnemyPop();

	//�`���[�g���A��
	if (mojiHp <= -1)
	{
		tutorialFlag = false;	
	}

	//�`���[�g���A��������������Ƃ��J�����ɋ�����
	cameraObj->SetTutorialFlag(tutorialFlag);

	
	//���[�r�[���I�������̃`���[�g���A���p��obj�@�Ɓ@player�̈ʒu�ݒ�
	if (cameraObj->GetStartGameFlag() == true && setObjectFlag == false)
	{
		shotObj->SetPosition({ 0,0,90 });
		player->SetPlayerPos({ 0,0,0 });
		setObjectFlag = true;
	}

	//�e�̔��ː���obj�@�@(��ʉE�̊Ŕ�)(�A�j���[�V��������)
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
	//�e�̔��ː���obj�@�̍X�V����
	kanbanShotObj->Update();
	kanbanShot2Obj->Update();
	kanbanShot3Obj->Update();
	kanbanShot4Obj->Update();
	
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

	//���@
	player->SetStartFlag(cameraObj->GetStartGameFlag());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->SetPlayerHpBar(hpBar);
	player->Update();

	// �G
	//oneway�X�V����
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
	
	//circle�X�V����
	for (std::unique_ptr<EnemyCircle>& circle : circles)
	{
		CheckAllCollision(circle->GetEnemy());
		circle->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
		circle->SetPlayerPosition(player->GetWorldPosition());
		circle->Update();
	}

	//�{�X
	boss->SetPlayerWorldPos(player->GetWorldPosition());
	boss->GetEnemy()->SetBossHpBar(bossHpBar,bossHpBarMax);
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
	CheckMojiCollision();

	//���@�̒e�̃p�[�e�B�N��
	//���e���X�g
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
			Particle->CreateParticle(30, 8, bullet1, { 0.5,0.5,0.5 }, { 0.03,0.03,0.03 }, 5.0f, 1.0f, { 1,1,1 }, { 1,0,0 });
		}
	}
	
	//�{�X�̃o���A������
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

    //�{�X��͂��܂� (�J�������~�܂�)
	if (cameraObj->GetEndFlag() == true)
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

	

	//�v���C���[��hp���O�ɂȂ�����
	if (player->GetHp0() == true)
	{
		player->SetEndFlag(true);
		player->SetkeyInput(false);
		//�p�[�e�B�N������
		Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
			{ 1,1,1 }, { 1,0,0 });
		
		//boss�̂��ꂽ���o�܂��p
		dieTimer--;

		if (dieTimer <= 0)
		{
			//�V�[���`�F���W
			playerDieFlag = true;

			dieTimer = dieTimerMax;
		}
		
	}
	//�Q�[�����I�������r��������
	if (pointsLast == true || playerDieFlag == true)
	{
		for (std::unique_ptr<Bill>& bill : bills)
		{
			bill->SetBillDeadFlag(true);
		}
	}

	startPlayer->Update();
	Particle->Update();
	titleObj->Update();
}

void GameScene::Draw()
{	
	/// ������3D�I�u�W�F�N�g�̕`�揈��
	Object3d::PreDraw(dxcommon->GetCmdlist());

	skydome->Draw();//�X�J�C�h�[��
	wallFloor->Draw();//��
	road->Draw();//��

	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Draw();
	}
	//���������ł��U�����ꂽ��Ђт������������ɕύX����
	if (mojiHp >= mojiChangeHp)
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
	if (cameraObj->GetStartMovieFlag() == false && gameStartFlag == false)
	{
		titleObj->Draw();
	}
		
	
	
	startPlayer->Draw();

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
// 3D�I�u�N�W�F�N�g�̕`�您���

	spriteCommon->PreDraw();
	if (gameScene == gameSceneTitle)//�^�C�g��
	{

		//titleSprite->Draw();
		reticleSprite->Draw();
	
		//debugtext_minute->DrawAll();
		//debugtext_minute2->DrawAll();
	}
	else if (gameScene == gameSceneInGame)//�Q�[����
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

		//�f�o�b�O�e�L�X�g
		//debugtext_minute->DrawAll();
		//debugtext_minute2->DrawAll();

	}
	else if (gameScene == gameSceneClear)//�N���A�V�[��
	{
		endSprite->Draw();
	}
	else if (gameScene == gameSceneGameOver)//�Q�[���I�[�o�[�V�[��
	{
		gameOverSprite->Draw();
	}

	
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
				Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
					{ 1,1,1 }, { 1,0,0 });

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
				Particle->CreateParticle(30, 64, enemy->GetWorldPosition(), { 0.5,0.5,0.5 }, { 0.02,0.02,0.02 }, 15.0f, 1.0f,
					{ 1,1,1 }, { 1,0.5,0 });
			}
			
		}
		//�{�X�ƒe�̓����蔻��
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
				Particle->CreateParticle(5, 32, player->GetWorldPosition(), { 0.8,0.8,0.8 }, { 0,-0.05,0 }, 10.0f, 2.0f,
					{ 1,1,1 }, { 1,0,0 });
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

void GameScene::CheckMojiCollision()
{
	if (mutekiFlagDeb == true)
	{
		return;
	}
	XMFLOAT3 pos1, pos2;

	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = shotObj->GetPosition();
	pos1.y = pos1.y + 10;

	float pos1Addx = 32;
	float pos1Addy = 16;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		//�`���[�g���A���̕����Ƀ_���[�W
		if (pos2.z >= pos1.z && //z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			mojiHp >= 1 //alive
			)
		{
			bullet->OnCollision();
			mojiHp--;
			//�p�[�e�B�N������
			Particle->CreateParticle(30, 32, bullet->GetWorldPosition(), { 2.8f, 2.8f, 2.8f }, { 0.02,0.02,0.02 }, 12.0f, 2.0f,
				{ 1,0,0 }, { 0.5,0.3,0.17 });
		}
		//�`���[�g���A���̕�����������Ƃ��p
		else if (pos2.z >= pos1.z && //z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			mojiHp >= 0 //alive
			)
		{
			mojiHp=-1;
			Particle->CreateParticle(30, 32, { shotObj->GetPosition().x ,  shotObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z }
				, { 2.8f, 2.8f, 2.8f }, { 0,-0.08,0 }, 12.0f, 2.0f,
				{ 0,1,0 }, { 0.5,0.3,0.17 });
		}
	}
}

void GameScene::CheckBillCollision(Bill* bill)
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
			newOneWay2->Init(enemyModel, pos, LorR, attackFlag,true);

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



FLOAT GameScene::BillScaleY(int randam)
{
	if (randam == 1)
	{
		billScaleY = 13;
	}
	else if (randam == 2)
	{
		billScaleY = 7;
	}
	else if (randam == 0)
	{
		billScaleY = 10;
	}
	return billScaleY;
}

XMFLOAT3 GameScene::BillRot(int randam)
{
	if (randam == 1)
	{
		billRotation.x = 90;
		billRotation.y = 45;
		billRotation.z = 20;
	}
	else if (randam == 2)
	{
		billRotation.x = -20;
		billRotation.y = -45;
		billRotation.z = -90;
	}
	else if (randam == 0)
	{
		billRotation.x = 90;
		billRotation.y = 0;
		billRotation.z = 45;
	}
	return billRotation;
	
}

void GameScene::BillCreate()
{
	const float iMax = 600;
	const float iAdd = 40;
	const float startZ = 130;
	const float iMaxS = 1000;
	const float iMaxE = 400;
srand((1));
	for (float i = 0; i < iMax; i += iAdd)
	{
		

		randBill = rand() % 3;
	/*	randBillRot = rand() % 15;
		if (randBillRot <= 0)
		{
			billRotation.z = 90;
		}*/
		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { 40,-32,startZ + i }, { 4,billScaleY,4 }, billRotation);

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { -40,-32,startZ + i }, { 4,billScaleY,4 });

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}

	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { 80,-32,startZ + i }, { 4,billScaleY,4 });

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { -80,-32,startZ + i }, { 4,billScaleY,4 });

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { 120,-32,startZ + i }, { 4,billScaleY,4 });

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill����
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(billModel, { -120,-32,startZ + i }, { 4,billScaleY,4 });

			//bill�o�^
			bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		
	}


	for (float i = 0; i < iMaxS; i += iAdd)
	{


		randBill = rand() % 3;
		/*	randBillRot = rand() % 15;
			if (randBillRot <= 0)
			{
				billRotation.z = 90;
			}*/
		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { 40,-32,0 - i }, { 4,billScaleY,4 }, billRotation);

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { -40,-32,0 - i }, { 4,billScaleY,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}

	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { 80,-32,0 - i }, { 4,billScaleY,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { -80,-32,0 - i }, { 4,billScaleY,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { 120,-32,0 - i }, { 4,billScaleY,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { -120,-32,0 - i }, { 4,billScaleY,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��

	}
	

	//�{�X��̃r���@�����_���ɓ|���ĉ�]���Ĕz�u
	for (float i = 0; i < iMaxE; i += iAdd)
	{

		randBill = rand() % 3;
		randBillRot = rand() % 3;

		BillScaleY(randBill);
		billRotation = BillRot(randBillRot);
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(billModel, { -200 + i,-32,990 }, { 4,billScaleY,4 },billRotation);
		
		//bill����
		std::unique_ptr<Bill> newBill2 = std::make_unique<Bill>();
		newBill2->Init(billModel, { 200 - i,-32,990 }, { 4,billScaleY,4 }, billRotation);

		//bill����
		std::unique_ptr<Bill> newBill3 = std::make_unique<Bill>();
		newBill3->Init(billModel, { 200 + i,-32,1110 }, { 4,billScaleY,4 }, billRotation);

		//bill����
		std::unique_ptr<Bill> newBill4 = std::make_unique<Bill>();
		newBill4->Init(billModel, { 200 - i,-32,1110 }, { 4,billScaleY,4 }, billRotation);

		//bill����
		std::unique_ptr<Bill> newBill5 = std::make_unique<Bill>();
		newBill5->Init(billModel, { 200 + i,-32,1230 }, { 4,billScaleY,4 }, billRotation);

		//bill����
		std::unique_ptr<Bill> newBill6 = std::make_unique<Bill>();
		newBill6->Init(billModel, { 200 - i,-32,1230 }, { 4,billScaleY,4 }, billRotation);


		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
		//bill�o�^
		bills.push_back(std::move(newBill2));//move �̓��j�[�N������n���邽��

		//bill�o�^
		bills.push_back(std::move(newBill3));//move �̓��j�[�N������n���邽��
		//bill�o�^
		bills.push_back(std::move(newBill4));//move �̓��j�[�N������n���邽��
		//bill�o�^
		bills.push_back(std::move(newBill5));//move �̓��j�[�N������n���邽��
		//bill�o�^
		bills.push_back(std::move(newBill6));//move �̓��j�[�N������n���邽��
	}



}


