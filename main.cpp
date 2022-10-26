#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
//#include <DirectXMath.h>
#include <vector>
#include <string>
#include <math.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Input.h"
#include <wrl.h> // 7-1
#include <d3dx12.h>//7-3
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#include "Object3d.h"

#include "Win.h"
#include "DirectXCommon.h"
#include "CollisionBase.h"
#include "Collision.h"

#include "Sprite.h"
#include "SpriteCommon.h"
#include <algorithm>

#include "DebugText.h"

#include "Audio.h"

#include "fbxsdk.h"
#include "LoadFbx.h"

#include "Object3dFbx.h"
#include "Camera.h"

#include "PostEffect.h"

#include "Player.h"
#include "Enemy.h"

#include "Matrix4.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

using namespace DirectX;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


// WindowsAPI初期化処理
	
	
	Win* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxcommon = nullptr;
	Audio* audio = nullptr;
	GameScene* gameScene = nullptr;

	win = new Win();
	win->WinCreate();


	dxcommon = new DirectXCommon();
	dxcommon->Init(win);

	SpriteCommon* spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	DebugText* debugtext_minute = nullptr;
	debugtext_minute = new DebugText();

	Object3d::StaticInitialize(dxcommon->GetDev(), win->window_width, win->window_height);

	Object3dFbx::SetDevice(dxcommon->GetDev());

	Camera* camera;
	camera = new Camera();
	camera->Init();

	Object3dFbx::SetCamera(camera);

	Object3dFbx::CreateGraphicsPipeline();

	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	FbxManager* fbxManager = FbxManager::Create();

	LoadFbx::GetInstance()->Init(dxcommon->GetDev());

	PostEffect* postEffect = nullptr;

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Init(dxcommon, input, audio);

	// DirectX初期化処理　ここまで

	//描画初期化処理　ここから
	spriteCommon->LoadTexture(0, L"Resource/target.png");

	Sprite *sprite = Sprite::Create(spriteCommon,0);

	spriteCommon->LoadTexture(1, L"Resource/setu.png");

	Sprite* sprite2 = Sprite::Create(spriteCommon, 1);

	spriteCommon->LoadTexture(2, L"Resource/title.png");

	Sprite* titleSprite = Sprite::Create(spriteCommon, 2);

	spriteCommon->LoadTexture(3, L"Resource/end.png");

	Sprite* endSprite = Sprite::Create(spriteCommon, 3);

	const int debugTextTexNumber3 = 4;

	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");

	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	DebugText* debugtext_minute2 = nullptr;
	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 5;

	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);

	//ポストエフェクト
	//spriteCommon->LoadTexture(100, L"Resource/mimikkyu.jpg");

	postEffect = new PostEffect();
	postEffect->Init(spriteCommon,100, { 0.5f,0.5f }, false, false);

#pragma region model
	
	ModelFbx* modelFbx = nullptr;
	Object3dFbx* object1 = nullptr;

	modelFbx = LoadFbx::GetInstance()->LoadModelFile("boneTest");
	object1 = new Object3dFbx;
	object1->Init();
	object1->SetModelFbx(modelFbx);

	Model * model1 = Model::LoadFromOBJ("bullet");
	Object3d* box = box->Create();
	box->SetModel(model1);
	box->scale = { 1,1,1, };
	box->SetPosition({ -100.0f, 0.0f, +200.0f });

	Object3d* box2 = box2->Create();
	box2->SetModel(model1);
	box2->scale = { 1,1,1, };
	box2->SetPosition({ 50.0f, 100.0f, +400.0f });


	Object3d* box3 = box3->Create();
	box3->SetModel(model1);
	box3->scale = { 1,1,1, };
	box3->SetPosition({ 50.0f, -100.0f, +600.0f });


	Object3d* box4 = box4->Create();
	box4->SetModel(model1);
	box4->scale = { 1,1,1, };
	box4->SetPosition({ 10.0f, -30.0f, +800.0f });
	
	Model* skydome_model = Model::LoadFromOBJ("skydome");

	Object3d* skydome = skydome->Create();

	skydome->SetModel(skydome_model);
	skydome->scale = { 11,11,11 };
	skydome->SetPosition({ 0,0,0 });

	Model* goalmodel = Model::LoadFromOBJ("go");

	Object3d* goal = goal->Create();

	goal->SetModel(goalmodel);
	goal->scale = { 1,1,1 };
	goal->SetPosition({ 0,0,900 });


#pragma endregion
	//描画初期化処理　ここまで



#pragma region シェーダーの色,初期化
	char moji[64];
	
	float secound_x = 0;
	float secound_y = 0;

	sprite->SetPosition({ 0.0f,0.0f,0.0f });
	sprite->SetSize({ 100,100 });
	sprite->TransVertexBuffer();

	sprite2->SetPosition({ 640.0f,360.0f,0.0f });
	sprite2->SetSize({ 1280,720 });
	sprite2->TransVertexBuffer();

	titleSprite->SetPosition({ 640.0f,360.0f,0.0f });
	titleSprite->SetSize({ 1280,720 });
	titleSprite->TransVertexBuffer();

	endSprite->SetPosition({ 640.0f,360.0f,0.0f });
	endSprite->SetSize({ 1280,720 });
	endSprite->TransVertexBuffer();

	XMFLOAT3 eye = camera->GetEye();

	int gameflag = 0;
	
#pragma endregion

	object1->PlayAnimation();
	while (true)  // ゲームループ
	{
		//// メッセージがある？
		

		//// ?ボタンで終了メッセージが来たらゲームループを抜ける

		if (win->ProcessMessage()) {
			break;
		}

		// DirectX毎フレーム処理　ここから

		//キーボード情報の取得開始
		//全キーの入力状態を取得する
		input->Update();

		
		sprite2->Update();
		titleSprite->Update();
		endSprite->Update();
		if (gameflag == 0)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameflag = 1;
			}
		}
		if (gameflag == 1)
		{
			gameScene->SetHwnd(win->GetHwnd());
			gameScene->SetViewPort(dxcommon->GetViewPort());

			gameScene->Update();

			//spline 曲線　通り道
			box->Update();
			box2->Update();
			box3->Update();
			box4->Update();
			goal->Update();

			//スカイドーム
			skydome->Update();

			object1->Update();
			//照準
			sprite->Update();
			sprite->SetPosition({ gameScene->mouseX,gameScene->mouseY,0 });
			//デバッグテキスト
			debugtext_minute->Print(gameScene->moji, secound_x, secound_y, 1.0f);
			debugtext_minute2->Print(gameScene->moji2, secound_x, secound_y + 100, 1.0f);

			if (gameScene->pointsLast == true)
			{
				gameflag = 2;
			}
		}
		else if (gameflag == 2)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameflag = 0;
				gameScene->Init(dxcommon, input, audio);
			}
		}
		
		
		camera->UpdateCamera();
		
		
//描画
		//ポストエフェクト

		//postEffect->PreDrawScene(dxcommon->GetCmdlist());
		
		//object1->Draw(dxcommon->GetCmdlist());
		//postEffect->PostDrawScene(dxcommon->GetCmdlist());
		
		dxcommon->BeginDraw();

		// ４．描画コマンドここから

		//object1->Draw(dxcommon->GetCmdlist());
		//postEffect->Draw(dxcommon->GetCmdlist());
		
		////obj描画
		gameScene->Draw();
		Object3d::PreDraw(dxcommon->GetCmdlist());

		box->Draw();
		box2->Draw();
		box3->Draw();
		box4->Draw();
		skydome->Draw();
		//goal->Draw();
		Object3d::PostDraw();

		////スプライト共通コマンド
		// スプライト
		spriteCommon->PreDraw();
		if (gameflag == 0)
		{
			titleSprite->Draw();
		}
		else if (gameflag == 1)
		{
			sprite->Draw();
			sprite2->Draw();

			/*debugtext_minute->DrawAll();
			debugtext_minute2->DrawAll();*/
			
		}
		else if (gameflag == 2)
		{
			endSprite->Draw();
		}

		// ４．描画コマンドここまで
		
		// ５．リソースバリアを戻す

		dxcommon->EndDraw();
		
	}
	// ウィンドウクラスを登録解除
	win->WinFinalize();

//解放
	delete modelFbx;
	delete object1;

	//Audio解放
	audio->Finalize();
	delete audio;

	//デバッグテキスト解放
	delete postEffect;

	debugtext_minute->Finalize();
	delete debugtext_minute;

	delete sprite;
	delete spriteCommon;
	LoadFbx::GetInstance()->Finalize();
	delete dxcommon;
	delete input;
	
	return 0;
}