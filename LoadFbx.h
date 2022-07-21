#pragma once

#include "fbxsdk.h"
#include "ModelFbx.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>

class LoadFbx
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static LoadFbx* GetInstance();

private:
	// privateなコンストラクタ（シングルトンパターン）
	LoadFbx() = default;
	// privateなデストラクタ（シングルトンパターン）
	~LoadFbx() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	LoadFbx(const LoadFbx& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const LoadFbx& obj) = delete;

public:
	/// <summary>
	///	初期化
	/// </summary>
	/// <param name = "device">D3D12device</param>
	void Init(ID3D12Device * device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

private:
	//D3D12device
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//FBXインポーター
	FbxImporter* fbxImporter = nullptr;


//fbxよみこみ
private://エイリアス
	//std:: を省略
	using string = std::string;

public://定数
	//モデル格納ルートパス
	static const string baseDirectory;

	/// <summary>
	///	ファイルからFBXモデルよみこみ
	/// </summary>
	/// <param name = "model name">モデル名</param>
	//void LoadModelFile(const string& modelName);
	ModelFbx *LoadModelFile(const string& modelName);

	/// <summary>
	///	再帰的にノード構成を解析
	/// </summary>
	/// <param name = "model">読み込み先モデルオブジェクト</param>
	/// <param name = "fbxNode">解析対象のノード</param>
	/// <param name = "parent">親ノード</param>
	void ParseNodeRecursive(ModelFbx* modelFbx, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	///	メッシュ読み取り
	/// </summary>
	/// <param name = "model">読み込み先モデルオブジェクト</param>
	/// <param name = "fbxNode">解析対象のノード</param>
	void ParseMesh(ModelFbx* modelFbx, FbxNode* fbxNode);

	//頂点座標読み取り
	void ParseMeshVertices(ModelFbx* modelFbx, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(ModelFbx* modelFbx, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(ModelFbx* modelFbx, FbxNode* fbxNode);
	//テクスチャ読み取り
	void LoadTexture(ModelFbx* modelFbx, const std::string &fullPath);

	/// <summary>
	/// Fbxの行列をXMMATRIXに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">もととなるFBX行列</param>
	static void ConvertMatrixFbx(XMMATRIX* dst, const FbxAMatrix& src);

	//ディレクトリを含んだファイルパスからファイル名を抽出
	std::string ExtractFileName(const std::string& path);

	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

	//スキニング情報の読み取り
	void ParseSkin(ModelFbx* modelFbx, FbxMesh* fbxMesh);

};