#pragma once
#include "Matrix4.h"
#include "Object3d.h"

#pragma comment (lib, "winmm.lib")

class CameraObj
{
private:
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: 省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	Object3d* cameraObj = cameraObj->Create();

public:


	void Init(DirectX::XMVECTOR worldPos,XMFLOAT3 rotation);

	void UpdateCamera();

	XMVECTOR splinePosition(const std::vector<DirectX::XMVECTOR>& points, size_t startIndex, float t);

	//getter
	const XMMATRIX& GetMatViewProjection() { return matViewProjection; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatView() { return matView; }
	const XMMATRIX& GetMatWorld() { return cameraObj->matWorld; }


	const XMFLOAT3& GetEye() { return XMFLOAT3{ eye.m128_f32[0], eye.m128_f32[1], eye.m128_f32[2] }; }
	const XMFLOAT3& GetTarget() { return XMFLOAT3{ target.m128_f32[0],target.m128_f32[1] ,target.m128_f32[2] }; }
	const XMFLOAT3& GetUp() { return up; }

	const XMFLOAT3& GetEyeVec() { return eyeVec; }

	const float& GetpositionX() { return cameraObj->position.z; }


	const INT& GetRaleIndex() { return  startIndex; }

	const BOOL& GetEndFlag() { return pointsLast; }

	const BOOL& GetStartMovieFlag() { return startMovie; }

	const BOOL& GetStartGameFlag() { return startGame; }

	const XMVECTOR& GetStartPos() { return start; }

	XMMATRIX GetWorldTransform()
	{
		return worldTransform;
	}

	void SetPlayerDieFlag(bool playerDieFlag) { this->playerDieFlag = playerDieFlag; }

	void SetTutorialFlag(bool tutorialFlag) { pointsStart = tutorialFlag; }

	void SetTarget(XMFLOAT3 target) { playerPos = target; }

	void SetTargetS(XMFLOAT3 target) { startPlayerObj = target; }

	void SetBoss(XMFLOAT3 target,bool bossDieFlag) 
	{ 
		bossPos = target;
		this->bossDieFlag = bossDieFlag;
	}

	void SetEndMovieFlag(bool endMovieFlag) { this->endMovieFlag = endMovieFlag; }
		
	/// <summary>
	/// ムービースキップ用
	/// </summary>
	/// <param name="startMovie">falseならスキップ</param>
	void SetStartMovieSkip(bool startMovie) { this->startMovie = startMovie; }

	void SetStopFlag(bool stopFlag) { this->stopFlag = stopFlag; }

private:
	XMFLOAT3 playerPos;
	XMFLOAT3 startPlayerObj;
	XMFLOAT3 bossPos;

	bool playerDieFlag = false;
	bool bossDieFlag = false;
	bool endMovieFlag = false;
	bool stopFlag = false;

	float distance = 20.0f;
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	// 視点座標
	XMVECTOR eye = { 0,0,-50 };

	XMFLOAT3 eyeBill = { 0,0,0 };

	XMFLOAT3 eyeView = { 0,0,0 };
	// 注視点座標
	XMVECTOR target = {0,0,0};

	XMFLOAT3 targetBill = { 0,0,0 };

	XMFLOAT3 targetView = { 0,0,0 };
	// 上方向ベクトル
	 XMFLOAT3 up = {0,1,0};
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	XMMATRIX matViewProjection;

	XMMATRIX worldTransform ;

	//ワールド座標
	XMVECTOR worldPos;

	// X,Y,Z軸回りの回転角
	XMFLOAT3 rotation = { 0,0,0 };

	DWORD time = 0;

	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//ゲーム内
	XMVECTOR start{ 0.0f, 0.0f, 0.0f };
	XMVECTOR p2{ 0.0f, 0.0f, 100.0f };
	XMVECTOR p3{ 0.0f, 0.0f, +200.0f };
	XMVECTOR p4{ 0.0f, 0.0f, +400.0f };
	XMVECTOR p5{ 0.0f, 0.0f, +600.0f };
	XMVECTOR p6{ 0.0f, 0.0f, +800.0f };
	XMVECTOR p7{ 0.0f, 0.0f, +800.0f };

	XMVECTOR end{ 0.0f, 0.0f, 800.0f };

	//スタートムービー
	XMVECTOR startT{ -10.0f, 10.0f, -500.0f };
	XMVECTOR p2T{ -30.0f, 0.0f, -550.0f };
	XMVECTOR p3T{ 0.0f, 0.0f, -600.0f };

	XMVECTOR titlePos{ 0,20,-600 };//タイトル

	XMVECTOR targetEnd{};
	float maxTime = 10.0f;
	float timeRate = 0.0f;
	
	XMFLOAT3 eyeVecTemp = { 0.0f,0.0f,0.0f };
	XMFLOAT3 eyeVec = { 0.0f,0.0f,0.0f };

	bool pointsLast = false;
	bool pointsStart = true;
	bool startMovie = false;
	bool startGame = false;
	bool titleFlag = true;

	std::vector<XMVECTOR>points{ start,start,p2,p3,p4,p5,end,end,end,end };

	std::vector<XMVECTOR>tyutoPoints{ startT,startT,p2T,p3T,p3T };

	int startIndex = 0;
	int targetIndex = 2;
	int startTIndex = 0;

};
