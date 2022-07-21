#include "PostEffect.h"
#include "Win.h"

#include <d3dcompiler.h>

#include <d3dx12.h>

#pragma comment (lib,"d3dcompiler.lib")
using namespace DirectX;

//�ÓI�����o�ϐ��̎���
const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };//R G B A �΂��ۂ���

PostEffect::PostEffect() :
	Sprite()
{
}

void PostEffect::Init(SpriteCommon* spriteCommon, UINT texnumber, XMFLOAT2 anchorpoint,
	bool isFlagX, bool isFlagY)
{
	spriteCommon_ = spriteCommon;
	CreatePipeline();
	HRESULT result;

	
	//���N���X�Ƃ��Ă̏�����
	//Sprite::Init(spriteCommon, texnumber, anchorpoint, isFlagX, isFlagY);

	//���_�o�b�t�@�̐���
	result = spriteCommon_->Device_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv)*vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	assert(SUCCEEDED(result));

	//���_�f�[�^
	VertexPosUv vertices[vertNum] =
	{	{{-0.5f	,-0.5f	,0.0f},{0.0f,1.0f} },
		{{-0.5f	,+0.5f	,0.0f},{0.0f,0.0f} },
		{{+0.5f ,-0.5f	,0.0f},{1.0f,1.0f} },
		{{+0.5f ,+0.5f	,0.0f},{1.0f,0.0f} }, };

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	//�萔�o�b�t�@�̐���
	result = spriteCommon->Device_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));



	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(DXGI_FORMAT_R8G8B8A8_UNORM,
		Win::window_width,
		(UINT)Win::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = spriteCommon->Device_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM,clearColor),
		IID_PPV_ARGS(&texBuff));
		assert(SUCCEEDED(result));

	//�e�N�X�`���ԃN���A
		//��f��(1280 * 720 921600pixel)
		const UINT pixelCount = Win::window_width * Win::window_height;
		//�摜1�����̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * Win::window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = sizeof(UINT) * Win::window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;

		//SRV�p�@�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvDescHeapDesc.NumDescriptors = 1;
		//SRV�p�@�f�X�N���v�^�q�[�v����
		result = spriteCommon->Device_()->CreateDescriptorHeap(&srvDescHeapDesc,
			IID_PPV_ARGS(&descHeapSRV));
		assert(SUCCEEDED(result));

		//SRV�ݒ�
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = 1;

		//�f�X�N���v�^�q�[�v��SRV�쐬
		spriteCommon->Device_()->CreateShaderResourceView(texBuff.Get(),//�r���[���֘A�t����o�b�t�@��
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart());

		//RTV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 1;
		//RTV�p�f�X�N���v�^�q�[�v�𐶐�
		result = spriteCommon->Device_()->CreateDescriptorHeap(&rtvDescHeapDesc,
			IID_PPV_ARGS(&descHeapRTV));
		assert(SUCCEEDED(result));

		//�f�X�N���v�^�q�[�v��RTV�쐬
		spriteCommon->Device_()->CreateRenderTargetView(texBuff.Get(),
			nullptr,
			descHeapRTV->GetCPUDescriptorHandleForHeapStart());

		//�[�x�o�b�t�@���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC depthResDesc =
			CD3DX12_RESOURCE_DESC::Tex2D(
				DXGI_FORMAT_D32_FLOAT,
				Win::window_width,
				Win::window_height,
				1, 0,
				1, 0,
				D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		//�[�x�o�b�t�@�̐���
		result = spriteCommon->Device_()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
			IID_PPV_ARGS(&depthBuff));
		assert(SUCCEEDED(result));

		//DSV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DescHeapDesc.NumDescriptors = 1;
		//DSV�p�f�X�N���v�^�q�[�v�쐬
		result = spriteCommon->Device_()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
		assert(SUCCEEDED(result));

		//�f�X�N���v�^�q�[�v��DSV�쐬
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		spriteCommon->Device_()->CreateDepthStencilView(depthBuff.Get(),
			&dsvDesc,
			descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	////���[���h�s��̍X�V
	//matWorld_ = XMMatrixIdentity();

	////Z����]
	//matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));

	////���s�ړ�
	//matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	//�萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = color_;
	constMap->mat = XMMatrixIdentity();

	constBuff_->Unmap(0, nullptr);
	if (isInvisible_ == true)
	{
		return;
	}

	//ID3D12GraphicsCommandList* cmdList = spriteCommon_->GetcmdList();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//�f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//���[�g�p�����[�^1�ԂɃV�F�[�_�[���\�[�X�r���[���Z�b�g
	//spriteCommon_->SetGraphicsRootDescriptorTable(1, texnumber_);
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());



	//�|���S���̕`��
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A�̕ύX(�V�F�[�_�[���\�[�X->�`��\)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		Win::window_width, Win::window_height));
	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Win::window_width,
		Win::window_height));

	//�S��ʃN���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A(�`��\->�V�F�[�_�[���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void PostEffect::CreatePipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;// ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;//ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;//ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resource/shader/PostEffectTestVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resource/shader/PostEffectTestPS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1�s�ŏ������ق������₷��)


		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1�s�ŏ������ق������₷��)
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	//���_�V�F�[�_�@�s�N�Z���V�F�[�_
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�W���I�Ȑݒ�(�|���S�����h��Ԃ� �[�x�N���b�s���O��L����)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O�Ȃ�

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�

	//�W���I�Ȑݒ�(�������݋���)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//��U�W���l���Z�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��
	//gpipeline.DepthStencilState.DepthEnable = false;//�[�x


		//�u�����h
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//���ʐݒ�(BLEND)
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	 //���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	 //�\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;	 //�f�X�g�̒l��100%�g��

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���_���C�A�E�g�ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`��
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;
	//descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);//b0 ���W�X�^
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0 ���W�X�^


	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0);//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0�@���W�X�^)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = spriteCommon_->Device_()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	gpipeline.pRootSignature = rootSignature.Get();
	assert(SUCCEEDED(result));

	//ComPtr<ID3D12PipelineState> pipelinestate;// ID3D12PipelineState* pipelinestate = nullptr;
	result = spriteCommon_->Device_()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}
