#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "ParallelLight.h"
#include "ShadowCircle.h"

/// <summary>
/// ライト
/// </summary>
class LightGroup
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	static const int DirLightNum = 3;
	static const int ShadowCircleNum = 1;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		// 環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源用
		ParallelLight::ConstBufferData dirLights[DirLightNum];
		ShadowCircle::ConstBufferData shadowCircles[ShadowCircleNum];
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static LightGroup* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="color">ライト色</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// 丸影の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetCircleShadowActive(int index, bool active);

	/// <summary>
	/// 丸影のキャスター座標をセット
	/// </summary>
	/// <param name="index">番号</param>
	/// <param name="lightpos">キャスター座標</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);

	/// <summary>
	/// 丸影の方向をセット
	/// </summary>
	/// <param name="index">番号</param>
	/// <param name="lightdir">方向</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// 丸影のキャスターとライトの距離をセット
	/// </summary>
	/// <param name="index">番号</param>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// 丸影の距離減衰係数をセット
	/// </summary>
	/// <param name="index">番号</param>
	/// <param name="lightatten">距離減衰係数</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// 丸影の減衰角度をセット
	/// </summary>
	/// <param name="index">番号</param>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度</param>
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);


private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	// 環境光の色
	XMFLOAT3 ambientColor = { 1,1,1 };

	// 平行光源の配列
	ParallelLight dirLights[DirLightNum];
	ShadowCircle shadowCircles[ShadowCircleNum];

	// ダーティフラグ
	bool dirty = false;
};

