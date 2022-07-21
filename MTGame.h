#pragma once
#include <Windows.h>
#include "Win.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Audio.h"
#include "fbxsdk.h"
#include "LoadFbx.h"

#include "Object3d.h"
#include "Object3dFbx.h"
#include "Camera.h"

#include "PostEffect.h"

#include "Player.h"
#include "Enemy.h"

#include "CollisionBase.h"
#include "Collision.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Input.h"
#include <wrl.h> // 7-1
#include <d3dx12.h>//7-3
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
class MTGame
{
private:
	Win* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxcommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugtext_minute = nullptr;
	DebugText* debugtext_minute2 = nullptr;
	Camera* camera;
	Sprite* sprite = Sprite::Create(spriteCommon, 0);
	SpriteCommon* spriteCommon = new SpriteCommon;

	PostEffect* postEffect = nullptr;
	ModelFbx* modelFbx = nullptr;
	Object3dFbx* object1 = nullptr;

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Object3d* skydome = skydome->Create();
	//当たり判定　球
	Sphere sphere;
	//当たり判定　三角形
	Triangle triangle_ene2;
	char moji[64];
	char moji2[64];
	char moji3[64];

	bool bulletFlag = false;

	int secound_x = 150;
	int secound_y = 0;
	int secound2_x = 100;
	int secound2_y = 0;

	int timer = 60;
	int secound = 0;
	int secound2 = 0;
	int minute = 0;

	int gameScene = 1;

	bool endRequest = false;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsEndRequest() { return endRequest; }
};
