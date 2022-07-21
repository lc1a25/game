#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite 
{
private: // エイリアス
// Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DirectX::を省略
    using XMMATRIX = DirectX::XMMATRIX;
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    //画面クリアカラー
    static const float clearColor[4];

    // デバイス
    //static ID3D12Device* device;

    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //グラフィックパイプライン
    ComPtr<ID3D12PipelineState>  pipelineState;

    //
    ComPtr<ID3D12RootSignature> rootSignature;

    /// <summary>
    /// 初期化
    /// </summary>
    void Init(SpriteCommon* spriteCommon, UINT texnumber, XMFLOAT2 anchorpoint,
        bool isFlagX, bool isFlagY);

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatePipeline();

};

