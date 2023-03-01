#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
//#include <DirectXMath.h>
#include <string>

#include "Model.h"
#include "Camera.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	
public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX viewProj;//ビュープロジェクション
		XMMATRIX world;//ワールド行列
		XMFLOAT3 cameraPos;//カメラ座標
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//アンビエント係数
		float pad1;//パディング
		XMFLOAT3 diffuse;//ディフューズ係数
		float pad2;//パディング
		XMFLOAT3 specular;//スペキュラー係数
		float alpha;//アルファ
	};

	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient;//アンビエント影響度
		XMFLOAT3 diffuse;//ディフューズ影響度
		XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
		std::string textureFilename;//テクスチャファイル名

		//XMVECTOR
		//コンストラクタ
		Material()
		{
			ambient = { 0.3f, 0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	static Material material;

	// 射影行列
	//static XMMATRIX matProjection;

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();


	
	
private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	//static VertexPosNormalUv vertices[vertexCount];
	static std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	//static unsigned short indices[planeCount * 3];
	static std::vector<unsigned short> indices;
	//カメラ
	static Camera* camera;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(bool flag = false);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return { matWorld.r[3].m128_f32[0], 
											matWorld.r[3].m128_f32[1] ,
											matWorld.r[3].m128_f32[2] }; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// モデルセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) { Object3d::camera = camera; }

	void SetCameraMatWorld(XMMATRIX matWorld) { matCameraWorld = matWorld; }

	void SetMatWorld(XMMATRIX worldTransform) { this->matWorld = worldTransform; }


	const XMFLOAT3& GetEye() { return camera->GetEye(); }
	const XMFLOAT3& GetTarget() { return camera->GetTarget(); }
	const XMFLOAT3& GetUp() { return camera->GetUp(); }

	const XMMATRIX& GetMatViewProjection() { return camera->GetMatViewProjection(); }

	

	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };

	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };

	// ローカルワールド変換行列
	XMMATRIX matWorld;

	// ビュー行列
	XMMATRIX matView;

	// 親オブジェクト
	Object3d* parent = nullptr;

private: // メンバ変数
	//ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	//ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };

	XMMATRIX matCameraWorld;
	//model
	Model* model = nullptr;
};

