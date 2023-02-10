#pragma once

#include "PipelineSet.h"
#include <DirectXTex.h>

/// <summary>
/// �X�v���C�g����
/// </summary>
class SpriteCommon
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�


	using XMMATRIX = DirectX::XMMATRIX;
	

public:
	//�e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512;

private://�����o�ϐ��ɂ� _ ����
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineSet_;

	//�ˉe�s��
	XMMATRIX matProjection_{};

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap_;

	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	ComPtr<ID3D12Resource> texBuff_[spriteSRVCount];

	//�f�o�C�X(�؂�Ă���)
	ID3D12Device* dev_ = nullptr;

	//�R�}���h���X�g(�؂�Ă���)
	ID3D12GraphicsCommandList* cmdList_;

private:
	void CreatePipeline2d();

public://�����o�֐�


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="window_width">�E�B���h�E�悱</param>
	/// <param name="window_height">�E�B���h�E����</param>
	void Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList,int window_width, int window_height);
	
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �摜�ǂݍ���
	/// </summary>
	void LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// �e�N�X�`���擾
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>�e�N�X�`�����\�[�X</returns>
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* Device_() { return dev_; }

	const XMMATRIX& GetMatProjection() { return matProjection_; }

	ID3D12GraphicsCommandList* GetcmdList() { return cmdList_; }

	/// <summary>
	/// ���[�g�f�X�N���v�^�e�[�u���̐ݒ�
	/// </summary>
	/// <param name="rootParametorIndex">���[�g�p�����[�^�ԍ�</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void SetGraphicsRootDescriptorTable(UINT rootParametorIndex,UINT texNumber);

};


