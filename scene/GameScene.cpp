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

	//���f���ǂݍ���
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
	
	player = new Player();
	player->Init(playerModel, bulletModel);

	startPlayer->SetModel(playerModel);
	startPlayer->scale = { 1,1,1 };
	startPlayer->SetPosition({0,0,-550});//500,0,-50

	enemy = new Enemy();
	enemy->Init(enemyModel, { 30.0f, -300.0f, -200.0f }, enemyModel);//30,0,100
	
	enemyL = new Enemy();
	enemyL->Init(enemyModel, { 50.0f, -50.0f, -200.0f }, enemyModel);

	enemyCircle = new EnemyCircle();
	enemyCircle->Init(enemyRotateModel, { 0.0f, -10.0f, -150.0f }, false,false);//-50.0f, 50.0f, 400.0f

	enemyCircle2 = new EnemyCircle();
	enemyCircle2->Init(enemyRotateModel, { 30.0f, -10.0f, -200.0f }, true,false);

	enemyOneWay = new EnemyOneWay();
	enemyOneWay->Init(enemyModel, { 0.0f,0.0f,-300.0f }, false);

	enemyOneWay2 = new EnemyOneWay();
	enemyOneWay2->Init(enemyModel, { 30.0f,-300.0f,-200.0f }, true);//30.0f,-100.0f,650.0f

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
	
}

void GameScene::Update()
{
	if (input->isKeyTrigger(DIK_P))
	{
		player->OnCollision();
	}
	if (input->isKeyTrigger(DIK_C) && mutekiFlag == true)
	{
		mutekiFlag = false;
	}
	else if (input->isKeyTrigger(DIK_C))
	{
		mutekiFlag = true;
	}
	if (input->isKeyTrigger(DIK_Q))
		if (cameraObj->GetStartMovieFlag() == false)
		{
			for (int i = 0; i < 50; i++)
			{
				boss->GetEnemy()->OnBossCollision();
			}
			startPlayer->position.z += 4;
			startPlayer->position.y += 0.3;
		}

	//���[�r�[�X�L�b�v
	if (input->isKeyTrigger(DIK_K))
	{
		cameraObj->SetStartMovieSkip(false);
		startPlayer->SetPosition({ 0,-200,610 });
	}

	//�X�^�[�g���[�r�[
	if (cameraObj->GetStartMovieFlag() == false)
	{
		startPlayer->position.z += 4;
		startPlayer->position.y += 0.3;
	}
	
	//�X�^�[�g���[�r�[��̎��@(��)�̏ꏊ
	if (startPlayer->GetPosition().z >= 600 && boss->GetBossDead() == false)
	{
		startPlayer->SetPosition({0,-200,610});
		player->SetkeyInput(true);
	}

	//bill������
	bills.remove_if([](std::unique_ptr<Bill>& bill)
	{
			return bill->billDead();
	});
	
	//�r���X�V����
	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->SetCameraZ(cameraObj->GetEye().z);
		bill->Update();
	}

	//csv��enemy����
	UpdateEnemyPop();

	//�`���[�g���A��
	if (mojiHp <= 0)
	{
		tutorialFlag = false;
		sceneChange = true;	
	}

	cameraObj->SetTutorialFlag(tutorialFlag);

	//�X�J�C�h�[��
	 //�J�����̈ړ��ʂ�菭���x������
	skydomeZ += cameraObj->GetEyeVec().z * 0.8;
	skydome->SetPosition({ 0,-220,skydomeZ });
	skydome->Update();

	//�ݒu���̍X�V
	wallFloor->Update();
	road->Update();
	
	shotHibiObj->Update();
	kanbanObj->Update();
	kanbanPlaneObj->Update();

	if (cameraObj->GetStartGameFlag() == true)
	{
		shotObj->SetPosition({ 0,0,90 });
	}
	shotObj->Update();
	//�e�̔��ː���
	if (cameraObj->GetStartGameFlag() == true)
	{
		kanbanTime++;
	}
	if (kanbanTime >= 31)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPos });
		kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot4Obj->SetPosition({ kanbanShotPosDown });
	}
	if (kanbanTime >= 61)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot3Obj->SetPosition({ kanbanShotPos });
		kanbanShot4Obj->SetPosition({ kanbanShotPosDown });
	}
	if (kanbanTime >= 91)
	{
		kanbanShotObj->SetPosition({ kanbanShotPosDown });
		kanbanShot2Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot3Obj->SetPosition({ kanbanShotPosDown });
		kanbanShot4Obj->SetPosition({ kanbanShotPos });
	}
	if (kanbanTime >= 121)
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
	

	//2d���e�B�N���X�v���C�g�̍��W
	mouseX = player->GetMouseX();
	mouseY = player->GetMouseY();

	//�f�o�b�O�p
	sprintf_s(moji, "%f", player->GetWorldPosition().z);

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
	player->SetEnemyFlag(enemyL->IsDead());
	player->SetCameraObj(cameraObj->GetWorldTransform());
	player->SetCameraPos(cameraObj->GetEye());
	player->SetCameraEyeVec(cameraObj->GetEyeVec());
	player->SetPlayerHpBar(hpBar);
	player->Update();

	enemy->SetCameraZ(cameraObj->GetEyeVec().z);
	enemyOneWay->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);
	enemyCircle->GetEnemy()->SetCameraZ(cameraObj->GetEyeVec().z);

	// �G
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

	//�{�X
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

	//�����蔻��
	if (mutekiFlag == false)
	{
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
		CheckPillarCollision();
	}

	//hp
	bossHpBar = boss->GetEnemy()->GetHpBarX();
	hpBar = player->GetHpBar();

	//�����
	if (cameraObj->GetEndFlag() == true)
	{
		bossFlag = true;
	}
	//�{�X���|���ꂽ��
	if (boss->GetEnemy()->IsDead() == true)
	{
		player->SetkeyInput(false);
		//�{�X�̉��o�����������
		if (boss->GetEnemy()->GetWorldPosition().y <= floorY)
		{
			//�V�[���`�F���W�܂��p
			dieTimer--;
			if (dieTimer <= 0)
			{
				player->SetEndFlag(true);
				//�V�[���`�F���W
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

	

	//�v���C���[��hp���O�ɂȂ�����
	if (player->GetHp0() == true)
	{
		
		//�p�[�e�B�N������
		PlayerCreateParticle(player->GetWorldPosition());
		
		//boss�̂��ꂽ���o�܂��p
		dieTimer--;

		if (dieTimer <= 0)
		{
			//�V�[���`�F���W
			playerDieFlag = true;

			dieTimer = 120;
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
}

void GameScene::Draw()
{
	/// ������3D�I�u�W�F�N�g�̕`�揈��
	Object3d::PreDraw(dxcommon->GetCmdlist());
	
	//�X�J�C�h�[��
	skydome->Draw();

	wallFloor->Draw();
	road->Draw();

	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Draw();
	}
	if (mojiHp >= 10)
	{
		shotObj->Draw();
	}
	else if (mojiHp >= 0)
	{
		shotHibiObj->Draw();
	}

	//pillar->Draw();
	//pillar2->Draw();
	//pillar3->Draw();
	//pillar4->Draw();
	//pillar5->Draw();

	kanbanObj->Draw();

	kanbanPlaneObj->Draw();
	kanbanShotObj->Draw();
	kanbanShot2Obj->Draw();
	kanbanShot3Obj->Draw();
	kanbanShot4Obj->Draw();

	startPlayer->Draw();

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
// 3D�I�u�N�W�F�N�g�̕`�您���
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
			if (boss->GetEnemy()->IsDead() == false)
			{
				player->OnCollision();
				//�p�[�e�B�N������
				PlayerCreateParticle(player->GetWorldPosition());
			}

			bullet->OnCollision();
		}
	}

	pos1 = enemy->GetWorldPosition();
	//���e�ƓG�����蔻��
	

	//�����蔻��̒���
	float pos1Add = 4;

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();
		
		if (pos1.z >= cameraObj->GetEye().z && pos2.z >= pos1.z && 
			pos2.x <= pos1.x + pos1Add && pos2.x >= pos1.x - pos1Add &&
			pos2.y <= pos1.y + pos1Add && pos2.y >= pos1.y - pos1Add)
		{
			if (cameraObj->GetRaleIndex() <= 5)
			{
				enemy->OnCollision();

				bullet->OnCollision();
				//�p�[�e�B�N������
				EnemyCreateParticle(enemy->GetWorldPosition());
			}
			else
			{
				enemy->OnBossCollision();
				BossCreateParticle(enemy->GetWorldPosition());
				if (boss->GetEnemy()->IsDead() == true)
				{
					BossCreateParticle(enemy->GetWorldPosition());
				}
				bullet->OnCollision();
			}
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

void GameScene::CheckBossANDChildCollision(Enemy* bossChild)
{
	XMFLOAT3 pos1, pos2;
	//�G�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bossChild->GetBullets();

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
			if (boss->GetEnemy()->IsDead() == false)
			{
				player->OnCollision();

			}

			bullet->OnCollision();
		}
	}
	
}

void GameScene::CheckPillarCollision()
{
	XMFLOAT3 pos1, pos2;

	//���e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

	pos1 = shotObj->GetPosition();
	pos1.y = pos1.y + 10;

	float pos1Addx = 32;
	float pos1Addy = 16;
	float shotObjAddy = 10;
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		pos2 = bullet->GetWorldPosition();

		if (pos2.z >= pos1.z && //z
			pos2.x <= pos1.x + pos1Addx && pos2.x >= pos1.x - pos1Addx && //x
			pos2.y <= pos1.y + pos1Addy && pos2.y >= pos1.y - pos1Addy && //y
			mojiHp >= 0 //alive
			)
		{
			bullet->OnCollision();
			mojiHp--;
			//�p�[�e�B�N������
			MojiCreateParticle({ shotObj->GetPosition().x ,  shotObj->GetPosition().y + shotObjAddy , shotObj->GetPosition().z });
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

			enemyCircle = new EnemyCircle();
			enemyCircle->Init(enemyRotateModel, pos, LorR, attackFlag);
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

void GameScene::PlayerCreateParticle(XMFLOAT3 position)
{
	//�p�[�e�B�N��
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
	//�p�[�e�B�N��
	for (int i = 0; i < 30; i++)
	{

		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};


		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

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
	//�p�[�e�B�N��
	for (int i = 0; i < 30; i++)
	{

		const float rnd_pos = 10.0f;
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
		const float rnd_acc = 0.011f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 15.0f, 0.0f, { 1,1,1 }, { 1,0.5,0 });
	}
}

void GameScene::MojiCreateParticle(XMFLOAT3 position)
{
	//�p�[�e�B�N��
	for (int i = 0; i < 30; i++)
	{

		const float rnd_pos = 10.0f;
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
		const float rnd_acc = 0.011f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//Add(5, pos, vel, acc);
		Particle->Add(32, pos, vel, acc, 15.0f, 0.0f, { 1,0,0 }, { 0.5,0.3,0.17 });
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
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 80,-32,startZ + i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -80,-32,startZ + i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 200,-32,0 - i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -200,-32,0 - i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { 80,-32,0 - i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}

	for (float i = 0; i < iMax2; i += iAdd)
	{
		//bill����
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(pillarModel, { -80,-32,0 - i }, { 4,13,4 });

		//bill�o�^
		bills.push_back(std::move(newBill));//move �̓��j�[�N������n���邽��
	}


}


