#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "spriteCommon.h"

using namespace DirectX;

/// <summary>
/// �X�v���C�g1����\��
/// </summary>
class Sprite
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�


	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

protected:

	//���_��
	static const int vertNum = 4;

	SpriteCommon* spriteCommon_ = nullptr;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff_;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;

	//XYZ������̉�]��
	XMFLOAT3 rotation_ = { 0.0f,0.0f, 0.0f };

		//���W
	XMFLOAT3 position_ = { 0,0,0 };

	//���[���h�s��
	XMMATRIX matWorld_;

	//�F
	XMFLOAT4 color_ = { 1,1,1,1 };

	//�e�N�X�`���ԍ�
	UINT texnumber_ = 0;

	//�傫��
	XMFLOAT2 size_ = { 100, 100 };

	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };

	//���E���]
	bool isFlagX_ = false;

	//�㉺���]
	bool isFlagY_ = false;

	//�e�N�X�`��������W
	XMFLOAT2 texLeftTop_ = { 0,0 };

	//�e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 texSize_ = { 100,100 };

	//��\��
	bool isInvisible_ = false;

public:

	struct VertexPosUv
	{
		XMFLOAT3 pos;//x y z�@���W
		XMFLOAT2 uv;//u v ���W
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;//�F
		XMMATRIX mat;//3D�ϊ��s��
	};

	//�R���X�g���N�^
	//Sprite();

	/// <summary>
	/// �X�v���C�g��������
	/// </summary>
	/// <param name="spriteCommon">�X�v���C�g����</param>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlagX">x���]���邩</param>
	/// <param name="isFlagY">y���]���邩</param>
	static Sprite* Create(SpriteCommon* spriteCommon, UINT texnumber, XMFLOAT2 anchorpoint = { 0.5f,0.5f },
		bool isFlagX = false, bool isFlagY = false);


	/// <summary>
	/// ������
	/// </summary>
	void Init(SpriteCommon* spriteCommon, UINT texnumber,XMFLOAT2 anchorpoint , 
				bool isFlagX , bool isFlagY );

	void TransVertexBuffer();


	void Update();

	void Draw();

	void SetPosition(const XMFLOAT3& position) { position_ = position;}

	void SetSize(const XMFLOAT2& size) { size_ = size; }

	void SetTexsize(const XMFLOAT2& texsize) { texSize_ = texsize; }

	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { texLeftTop_ = texLeftTop; }


};
