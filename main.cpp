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
#include <algorithm>

#include "Audio.h"

#include "Matrix4.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

using namespace DirectX;

GameScene* Reset(GameScene* gameScene, DirectXCommon* dxcommon, Input* input, Audio* audio,Win* win)
{
	if (gameScene != nullptr)
	{
		delete gameScene;
	}
	gameScene = new GameScene();
	gameScene->Init(dxcommon, input, audio,win);

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
	gameScene = Reset(gameScene,dxcommon, input, audio,win);
	
	// DirectX初期化処理　ここまで

	////描画初期化処理　ここから
	
	//描画初期化処理　ここまで



#pragma region 初期化

	int gameSceneNum = 0;
	
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
	
		gameScene->SetGameFlag(gameSceneNum);
		gameScene->SetHwnd(win->GetHwnd());
		gameScene->SetViewPort(dxcommon->GetViewPort());
		if (gameSceneNum == 0)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = 1;
			}
		}
		else if (gameSceneNum == 1)
		{
			gameScene->Update();

			if (gameScene->playerDieFlag == true)
			{
				gameSceneNum = 3;
			}
			if (gameScene->pointsLast == true)
			{
				gameSceneNum = 2;
			}

		}
		//ゲームクリア
		else if (gameSceneNum == 2)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameScene = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		//ゲームオーバー
		else if (gameSceneNum == 3)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameScene = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		
		
//描画
		
		dxcommon->BeginDraw();

		// ４．描画コマンドここから

		gameScene->Draw();
		
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
	delete gameScene;
	delete dxcommon;
	delete input;
	
	return 0;
}