#pragma once
#include <d3d12.h>
#include <wrl.h>
class PipelineSet
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�

public:
		ComPtr<ID3D12PipelineState> pipelinestate;//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12RootSignature> rootsignature;//���[�g�V�O�l�`��
	
};
