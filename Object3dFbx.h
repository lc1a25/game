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
protected://�G�C���A�X
//
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX:: �ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�ÓI�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device) { Object3dFbx::device = device; }
	static void SetCamera(Camera* camera) { Object3dFbx::camera = camera; }

	///<summary>
	///�O���t�B�b�N�p�C�v���C���̐���
	///</summary>
	static void CreateGraphicsPipeline();

private://�ÓI�����o�ϐ�
	//device
	static ID3D12Device* device;
	//�J����
	static Camera* camera;

	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��)
	struct ConstBufferDataTranceform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};
public://�����o�֐�
	///<summary>
	///������
	///</summary>
	void Init();
	///<summary>
	///�X�V
	///</summary>
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	///<summary>
	///���f���̃Z�b�g
	///</summary>
	/// <param name = "modelFbx">���f��</param>
	void SetModelFbx(ModelFbx* modelFbx) { this->modelFbx = modelFbx; }

public://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//x,y,z �����̃��[�J����]��
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	///���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	ModelFbx* modelFbx = nullptr;

public://�萔
	//�{�[���̍ő�l
	static const int MAX_BONES = 32;// fbx hlsli �Ɓ@�ꏏ�ɂ���

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

public: //animation
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();
};
