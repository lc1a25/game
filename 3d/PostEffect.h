#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite 
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DirectX::���ȗ�
    using XMMATRIX = DirectX::XMMATRIX;
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();

    //��ʃN���A�J���[
    static const float clearColor[4];

    // �f�o�C�X
    //static ID3D12Device* device;

    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    //�O���t�B�b�N�p�C�v���C��
    ComPtr<ID3D12PipelineState>  pipelineState;

    //
    ComPtr<ID3D12RootSignature> rootSignature;

    /// <summary>
    /// ������
    /// </summary>
    void Init(SpriteCommon* spriteCommon, UINT texnumber, XMFLOAT2 anchorpoint,
        bool isFlagX, bool isFlagY);

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreatePipeline();

};
