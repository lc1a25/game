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
	enemy->Init(enemyModel,{30,0,100},true);

	enemyL = new Enemy();
	enemyL->Init(enemyModel, { -30,0,100 }, false);
	//camera->cameraObj->Init();
	camera = new Camera();
	camera->Init();

	cameraObj = new CameraObj();
	cameraObj->Init({0,0,0},{0,0,0});

	Object3d::SetCamera(camera);
	
}

void GameScene::Update()
{
	sprintf_s(moji, "%f", cameraObj->GetpositionX());
	//camera->eye = cameraObj->eye;


	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(hwnd, &mousePosition);
	mouseX = mousePosition.x;//�k���ϊ��@long -> float
	mouseY = mousePosition.y;
	
	//�J�����̃v���W�F�N�V�����s��
	XMMATRIX matProjection = camera->GetMatViewProjection();
	//XMMATRIX matView = camera->GetMatView();
	//�r���[�v���W�F�N�V�����r���[�|�[�g�s��
	//XMMATRIX matVP = Matrix4::matrixMatrix(matView , matProjection);
	XMMATRIX matVPV = Matrix4::matrixMatrix(matProjection, viewPort);
	//�t�s����v�Z
	XMMATRIX matInverse = Matrix4::matrixInverse(matVPV);
	//�X�N���[�����W
	XMVECTOR posNear = XMVECTOR{ mouseX, mouseY, 0 };
	XMVECTOR posFar = XMVECTOR{ mouseX, mouseY, 1 };

	//�X�N���[�����烏�[���h���W��
	posNear = Matrix4::transform(posNear, matInverse);
	posFar = Matrix4::transform(posFar, matInverse);

	//�}�E�X���C�̕���
	XMVECTOR mouseDirection;
	mouseDirection.m128_f32[0] = posFar.m128_f32[0] - posNear.m128_f32[0];
	mouseDirection.m128_f32[1] = posFar.m128_f32[1] - posNear.m128_f32[1];
	mouseDirection.m128_f32[2] = posFar.m128_f32[2] - posNear.m128_f32[2];
	mouseDirection = XMVector3Normalize(mouseDirection);

	//3d�̃��e�B�N���̍��W�������Ƃ�
	reticlePos.m128_f32[0] = player->GetReticleWorldPosition().x;
	reticlePos.m128_f32[1] = player->GetReticleWorldPosition().y;
	reticlePos.m128_f32[2] = player->GetReticleWorldPosition().z;
	//�J��������3d���e�B�N���̋���
	const float distanceObject = 50.0f;
	//near ���� far �Ɂ@distanceObject ���i�񂾋���
	reticlePos = (mouseDirection - posNear) * distanceObject;

	//3d�̃��e�B�N�����}�E�X�������Ă���Ƃ���ɍs������
	player->SetReticleWorldPos(reticlePos);

	camera->UpdateCamera();
	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);
	enemy->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemy->GetWorldPosition());
	enemy->Update();

	enemyL->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemyL->GetWorldPosition());
	enemyL->Update();

	player->Update();

	//camera->SetEye({0,0,150});
	cameraObj->UpdateCamera();
}

void GameScene::Draw()
{
	Object3d::PreDraw(dxcommon->GetCmdlist());
	player->Draw();
	enemy->Draw();
	enemyL->Draw();
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
		if (length <= size + 12)
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
				enemy->OnCollision();

				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
}
