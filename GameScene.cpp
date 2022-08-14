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
	mouseX = mousePosition.x;//縮小変換　long -> float
	mouseY = mousePosition.y;
	
	//カメラのプロジェクション行列
	XMMATRIX matProjection = camera->GetMatViewProjection();
	//XMMATRIX matView = camera->GetMatView();
	//ビュープロジェクションビューポート行列
	//XMMATRIX matVP = Matrix4::matrixMatrix(matView , matProjection);
	XMMATRIX matVPV = Matrix4::matrixMatrix(matProjection, viewPort);
	//逆行列を計算
	XMMATRIX matInverse = Matrix4::matrixInverse(matVPV);
	//スクリーン座標
	XMVECTOR posNear = XMVECTOR{ mouseX, mouseY, 0 };
	XMVECTOR posFar = XMVECTOR{ mouseX, mouseY, 1 };

	//スクリーンからワールド座標に
	posNear = Matrix4::transform(posNear, matInverse);
	posFar = Matrix4::transform(posFar, matInverse);

	//マウスレイの方向
	XMVECTOR mouseDirection;
	mouseDirection.m128_f32[0] = posFar.m128_f32[0] - posNear.m128_f32[0];
	mouseDirection.m128_f32[1] = posFar.m128_f32[1] - posNear.m128_f32[1];
	mouseDirection.m128_f32[2] = posFar.m128_f32[2] - posNear.m128_f32[2];
	mouseDirection = XMVector3Normalize(mouseDirection);

	//3dのレティクルの座標をうけとる
	reticlePos.m128_f32[0] = player->GetReticleWorldPosition().x;
	reticlePos.m128_f32[1] = player->GetReticleWorldPosition().y;
	reticlePos.m128_f32[2] = player->GetReticleWorldPosition().z;
	//カメラから3dレティクルの距離
	const float distanceObject = 50.0f;
	//near から far に　distanceObject 分進んだ距離
	reticlePos = (mouseDirection - posNear) * distanceObject;

	//3dのレティクルをマウスがさしているところに行かせる
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
		if (length <= size + 12)
		{
			enemy->OnCollision();

			bullet->OnCollision();
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
				enemy->OnCollision();

				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
}
