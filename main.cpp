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
#include "Light.h"

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
	//Light::StaticInitialize(dxcommon->GetDev());
	// ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->GetDev());
	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	//ゲームシーン初期化
	gameScene = Reset(gameScene,dxcommon, input, audio,win);
	
	// DirectX初期化処理　ここまで




#pragma region 初期化

	int gameSceneNum = 0;//今いるシーン
	int gameSceneTitleNum = 0;//タイトルシーン
	int gameSceneInGameNum = 1;//インゲームシーン
	int gameSceneClearNum = 2;//クリアシーン
	int gameSceneGameOverNum = 3;//ゲームオーバーシーン
	
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
	
		gameScene->SetGameFlag(gameSceneNum);//シーンがどこにいるか
		gameScene->SetHwnd(win->GetHwnd());//hwndおくる レティクル用
		gameScene->SetViewPort(dxcommon->GetViewPort());//ビューポートおくる　レティクル用

		if (gameSceneNum == gameSceneTitleNum)//タイトル
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneInGameNum;//インゲームへ
			}
		}
		else if (gameSceneNum == gameSceneInGameNum)//インゲーム
		{
			gameScene->Update();

			if (gameScene->playerDieFlag == true)//プレイヤーが死んだら
			{
				gameSceneNum = gameSceneGameOverNum;//ゲームオーバー
			}
			if (gameScene->pointsLast == true)
			{
				gameSceneNum = gameSceneClearNum;//ゲームクリア
			}

		}
		//ゲームクリア
		else if (gameSceneNum == gameSceneClearNum)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneTitleNum;//タイトルに戻る
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		//ゲームオーバー
		else if (gameSceneNum == gameSceneGameOverNum)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneTitleNum;//タイトルに戻る
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