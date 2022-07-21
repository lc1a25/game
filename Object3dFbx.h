#pragma once

#include "ModelFbx.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "LoadFbx.h"

class Object3dFbx
{
protected://エイリアス
//
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: 省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { Object3dFbx::device = device; }
	static void SetCamera(Camera* camera) { Object3dFbx::camera = camera; }

	///<summary>
	///グラフィックパイプラインの生成
	///</summary>
	static void CreateGraphicsPipeline();

private://静的メンバ変数
	//device
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	
public://サブクラス
	//定数バッファ用データ構造体(座標変換行列)
	struct ConstBufferDataTranceform
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};
public://メンバ関数
	///<summary>
	///初期化
	///</summary>
	void Init();
	///<summary>
	///更新
	///</summary>
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	///<summary>
	///モデルのセット
	///</summary>
	/// <param name = "modelFbx">モデル</param>
	void SetModelFbx(ModelFbx* modelFbx) { this->modelFbx = modelFbx; }

public://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//x,y,z 軸回りのローカル回転軸
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	///ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	ModelFbx* modelFbx = nullptr;

public://定数
	//ボーンの最大値
	static const int MAX_BONES = 32;// fbx hlsli と　一緒にする

	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;

public: //animation
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();
};
