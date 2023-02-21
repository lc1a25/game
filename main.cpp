#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <math.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include<fstream>

#include "Object3d.h"

#include "Win.h"
#include "Input.h"
#include "DirectXCommon.h"

#include "Sprite.h"
#include "SpriteCommon.h"
#include <algorithm>

#include "DebugText.h"

#include "Audio.h"

#include "Matrix4.h"
#include "Light.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

using namespace DirectX;

GameScene* Reset(GameScene* gameScene, DirectXCommon* dxcommon, Input* input, Audio* audio)
{
	if (gameScene != nullptr)
	{
		delete gameScene;
	}
	gameScene = new GameScene();
	gameScene->Init(dxcommon, input, audio);

	return gameScene;
}
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

	Object3d::StaticInitialize(dxcommon->GetDev(), win->window_width, win->window_height);

	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	//ゲームシーン初期化
	gameScene = Reset(gameScene,dxcommon, input, audio);
	
	// DirectX初期化処理　ここまで

	//描画初期化処理　ここから
	//スプライト初期化
	SpriteCommon* spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	//レティクル
	spriteCommon->LoadTexture(0, L"Resource/target.png");
	Sprite *reticleSprite = Sprite::Create(spriteCommon,0);

	//説明のテキスト
	spriteCommon->LoadTexture(1, L"Resource/setu.png");
	Sprite* explanSprite = Sprite::Create(spriteCommon, 1);
	
	//タイトル
	spriteCommon->LoadTexture(2, L"Resource/title.png");
	Sprite* titleSprite = Sprite::Create(spriteCommon, 2);

	//エンド
	spriteCommon->LoadTexture(3, L"Resource/end.png");
	Sprite* endSprite = Sprite::Create(spriteCommon, 3);

	//ボスのhpバーの枠
	spriteCommon->LoadTexture(4, L"Resource/hpWaku.png");
	Sprite* bossHpWakuSprite = Sprite::Create(spriteCommon, 4);

	//ボスのhpバー
	spriteCommon->LoadTexture(5, L"Resource/hpBar.png");
	Sprite* bossHpBarSprite = Sprite::Create(spriteCommon, 5,{0,0});

	//プレイヤーのhp
	spriteCommon->LoadTexture(6, L"Resource/heart.png");
	Sprite *playerHpSprite = Sprite::Create(spriteCommon, 6);

	//ゲームオーバー
	spriteCommon->LoadTexture(7, L"Resource/gameover.png");
	Sprite *gameOverSprite = Sprite::Create(spriteCommon, 7);

	//チュートリアル攻撃方法
	spriteCommon->LoadTexture(8, L"Resource/tyutorial.png");
	Sprite* tyutoRial = Sprite::Create(spriteCommon, 8);
	
	//チュートリアル移動方法
	spriteCommon->LoadTexture(9, L"Resource/tyutorialMove.png");
	Sprite* tyutoRialMove = Sprite::Create(spriteCommon, 9);

	//シーン遷移用
	spriteCommon->LoadTexture(10, L"Resource/backBlack.png");
	Sprite* backBlack = Sprite::Create(spriteCommon, 10);

	//デバッグテキスト
	DebugText* debugtext_minute = nullptr;
	debugtext_minute = new DebugText();

	const int debugTextTexNumber3 = 20;
	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");
	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	DebugText* debugtext_minute2 = nullptr;
	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 21;
	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);

	//描画初期化処理　ここまで



#pragma region 初期化
	
	float secound_x = 0;
	float secound_y = 0;

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

	tyutoRial->SetPosition({ 620.0f,120.0f,0.0f });
	tyutoRial->SetSize({ 750,108 });
	tyutoRial->TransVertexBuffer();

	tyutoRialMove->SetPosition({ 620.0f,600.0f,0.0f });
	tyutoRialMove->SetSize({ 750,190 });
	tyutoRialMove->TransVertexBuffer();


	float backBlackX = -640.0f;
	backBlack->SetPosition({ backBlackX,360.0f,0.0f });
	backBlack->SetSize({ 1280,720 });
	

	float bossHpX = 733.0f;
	float hp = 288;

	bossHpBarSprite->SetPosition({ 273.0f,34.0f,0.0f });

	int gameflag = 0;
	
#pragma endregion

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
		bossHpX = gameScene->GetbossHpBar();
		hp = gameScene->GetHpBar();
		
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

		tyutoRial->Update();
		tyutoRialMove->Update();

		backBlack->SetPosition({ backBlackX,360.0f,0.0f });
		backBlack->Update();
		backBlack->TransVertexBuffer();
		//照準
		reticleSprite->Update();
		reticleSprite->SetPosition({ gameScene->mouseX,gameScene->mouseY,0 });
		if (gameflag == 0)
		{
			gameScene->SetHwnd(win->GetHwnd());
			gameScene->SetViewPort(dxcommon->GetViewPort());

			gameScene->Update();
			if (input->isMouseKey())
			{
				gameflag = 1;
			}
		}
		if (gameflag == 1)
		{
			gameScene->SetHwnd(win->GetHwnd());
			gameScene->SetViewPort(dxcommon->GetViewPort());

			gameScene->Update();

			playerHpSprite->Update();
			//デバッグテキスト
			debugtext_minute->Print(gameScene->moji, secound_x, secound_y, 1.0f);
			debugtext_minute2->Print(gameScene->moji2, secound_x, secound_y + 100, 1.0f);
			if (gameScene->sceneChange == true)
			{
				backBlackX+= 100;
			}
			if (gameScene->playerDieFlag == true)
			{
				gameflag = 3;
			}
			if (gameScene->pointsLast == true)
			{
				gameflag = 2;
			}

		}
		//ゲームクリア
		else if (gameflag == 2)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameflag = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio);
				bossHpX = 733.0f;
				hp = 288;
			}
		}
		//ゲームオーバー
		else if (gameflag == 3)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameflag = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio);
				
				
				bossHpX = 733.0f;
				hp = 288;
			}
		}
		
		
//描画
		
		dxcommon->BeginDraw();

		// ４．描画コマンドここから
		

		gameScene->Draw();
		////スプライト共通コマンド
		// スプライト
		spriteCommon->PreDraw();
		if (gameflag == 0)
		{

			titleSprite->Draw();
			reticleSprite->Draw();
		}
		else if (gameflag == 1)
		{
			reticleSprite->Draw();
			//explanSprite->Draw();
			backBlack->Draw();
			if (gameScene->tutorialFlag == true)
			{
				tyutoRial->Draw();
				tyutoRialMove->Draw();
			}
			else
			{
				playerHpSprite->Draw();
			}
			if (gameScene->bossFlag == true)
			{
				bossHpBarSprite->Draw();
				bossHpWakuSprite->Draw();
			}
			

			/*debugtext_minute->DrawAll();
			debugtext_minute2->DrawAll();*/
			
		}
		else if (gameflag == 2)
		{
			endSprite->Draw();
		}
		else if (gameflag == 3)
		{
			gameOverSprite->Draw();

		}

		// ４．描画コマンドここまで
		
		// ５．リソースバリアを戻す

		dxcommon->EndDraw();
		
	}
	// ウィンドウクラスを登録解除
	win->WinFinalize();

//解放

	//Audio解放
	audio->Finalize();
	delete audio;

	//デバッグテキスト解放
	debugtext_minute->Finalize();
	delete debugtext_minute;
	delete debugtext_minute2;

	//スプライト解放
	delete reticleSprite;
	delete explanSprite;
	delete titleSprite;
	delete backBlack;
	delete tyutoRial;
	delete tyutoRialMove;
	delete gameOverSprite;
	delete bossHpBarSprite;
	delete playerHpSprite;
	delete endSprite;
	delete spriteCommon;


	delete dxcommon;
	delete input;
	
	return 0;
}

