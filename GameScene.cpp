#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrƒ`ƒFƒbƒN
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

}

void GameScene::Update()
{
	POINT mousePosition;

	GetCursorPos(&mousePosition);
	ScreenToClient(hwnd, &mousePosition);
	mouseX = mousePosition.x;//k¬•ÏŠ·@long -> float
	mouseY = mousePosition.y;


	XMMATRIX matVPV = Matrix4::matrixMatrix(camera->matProjection, viewPort);
	XMMATRIX matInverse = XMMatrixInverse(nullptr, matVPV);

	XMVECTOR posNear = XMVECTOR{ (Win::window_width, Win::window_height, 0) };
	XMVECTOR posFar = XMVECTOR{ (Win::window_width, Win::window_height, 1) };

	posNear = Matrix4::transform(posNear, matInverse);
	posFar = Matrix4::transform(posFar, matInverse);

	XMVECTOR mouseDirection;
	mouseDirection.m128_f32[0] = posFar.m128_f32[0] - posNear.m128_f32[0];
	mouseDirection.m128_f32[1] = posFar.m128_f32[1] - posNear.m128_f32[1];
	mouseDirection.m128_f32[2] = posFar.m128_f32[2] - posNear.m128_f32[2];
	mouseDirection = XMVector3Normalize(mouseDirection);

	const float distanceTestObject = 1.0f;

	reticlePos.m128_f32[0] = player->GetReticleWorldPosition().x;
	reticlePos.m128_f32[1] = player->GetReticleWorldPosition().y;
	reticlePos.m128_f32[2] = player->GetReticleWorldPosition().z;

	reticlePos = (mouseDirection - posNear) * distanceTestObject;
	player->SetReticleWorldPos(reticlePos);

	CheckAllCollision(enemy);
	CheckAllCollision(enemyL);
	enemy->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemy->GetWorldPosition());
	enemy->Update();

	enemyL->SetPlayerPosition(player->GetWorldPosition());
	player->SetEnemyPosition(enemyL->GetWorldPosition());
	enemyL->Update();

	player->Update();

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

	//Ž©’eƒŠƒXƒg
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//“G’eƒŠƒXƒg
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

	pos1 = player->GetWorldPosition();
	//Ž©ƒLƒƒƒ‰‚Æ“G’e“–‚½‚è”»’è
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
	//Ž©’e‚Æ“G“–‚½‚è”»’è
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

	
	//Ž©’e‚Æ“G’e“–‚½‚è”»’è
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
