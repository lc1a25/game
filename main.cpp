#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
//#include <DirectXMath.h>
#include <vector>
#include <string>
#include <math.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Input.h"
#include <wrl.h> // 7-1
#include <d3dx12.h>//7-3
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#include "Object3d.h"

#include "Win.h"
#include "DirectXCommon.h"
#include "CollisionBase.h"
#include "Collision.h"

#include "Sprite.h"
#include "SpriteCommon.h"
#include <algorithm>

#include "DebugText.h"

#include "Audio.h"

#include "fbxsdk.h"
#include "LoadFbx.h"

#include "Object3dFbx.h"
#include "Camera.h"

#include "PostEffect.h"

#include "Player.h"
#include "Enemy.h"

#include "Matrix4.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

using namespace DirectX;
// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


// WindowsAPI����������
	
	
	Win* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxcommon = nullptr;
	Audio* audio = nullptr;
	GameScene* gameScene = nullptr;

	win = new Win();
	win->WinCreate();


	dxcommon = new DirectXCommon();
	dxcommon->Init(win);

	SpriteCommon* spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	DebugText* debugtext_minute = nullptr;
	debugtext_minute = new DebugText();

	Object3d::StaticInitialize(dxcommon->GetDev(), win->window_width, win->window_height);

	Object3dFbx::SetDevice(dxcommon->GetDev());

	Camera* camera;
	camera = new Camera();
	camera->Init();

	Object3dFbx::SetCamera(camera);

	Object3dFbx::CreateGraphicsPipeline();

	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	FbxManager* fbxManager = FbxManager::Create();

	LoadFbx::GetInstance()->Init(dxcommon->GetDev());

	PostEffect* postEffect = nullptr;

	// �Q�[���V�[���̏�����
	gameScene = new GameScene();
	gameScene->Init(dxcommon, input, audio);

	// DirectX�����������@�����܂�

	//�`�揉���������@��������
	spriteCommon->LoadTexture(0, L"Resource/target.png");

	Sprite *sprite = Sprite::Create(spriteCommon,0);

	const int debugTextTexNumber3 = 4;

	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");

	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	DebugText* debugtext_minute2 = nullptr;
	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 5;

	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);

	//�|�X�g�G�t�F�N�g
	//spriteCommon->LoadTexture(100, L"Resource/mimikkyu.jpg");

	postEffect = new PostEffect();
	postEffect->Init(spriteCommon,100, { 0.5f,0.5f }, false, false);

#pragma region model
	
	ModelFbx* modelFbx = nullptr;
	Object3dFbx* object1 = nullptr;

	modelFbx = LoadFbx::GetInstance()->LoadModelFile("boneTest");
	object1 = new Object3dFbx;
	object1->Init();
	object1->SetModelFbx(modelFbx);

	Model* skydome_model = Model::LoadFromOBJ("skydome");

	Object3d* skydome = skydome->Create();

	skydome->SetModel(skydome_model);
	skydome->SetPosition({ 0,0,0 });


#pragma endregion
	//�`�揉���������@�����܂�



#pragma region �V�F�[�_�[�̐F,������
	char moji[64];
	
	float secound_x = 150;
	float secound_y = 0;

	sprite->SetPosition({ 0.0f,0.0f,0.0f });
	sprite->SetSize({ 100,100 });
	//sprite->SetTexsize({100.0f,100.0f });

	sprite->TransVertexBuffer();

	XMFLOAT3 eye = camera->GetEye();
	
#pragma endregion

	object1->PlayAnimation();
	while (true)  // �Q�[�����[�v
	{
		//// ���b�Z�[�W������H
		

		//// ?�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���

		if (win->ProcessMessage()) {
			break;
		}

		// DirectX���t���[�������@��������

		//�L�[�{�[�h���̎擾�J�n
		//�S�L�[�̓��͏�Ԃ��擾����
		input->Update();

		sprite->Update();

		gameScene->SetHwnd(win->GetHwnd());
		gameScene->SetViewPort(dxcommon->GetViewPort());
		
		sprite->SetPosition({ gameScene->mouseX,gameScene->mouseY,0 });
	
		
		
		debugtext_minute->Print(gameScene->moji, secound_x, secound_y, 1.0f);
			XMVECTOR moveZ = XMVectorSet(0, 0, 1.0f, 0);//z speed

			XMVECTOR moveX2 = XMVectorSet(1.0f, 0, 0, 0);//debug

		
			XMFLOAT3 xCamera = { 1,0,0 };
		
			XMFLOAT3 yCamera = { 0,1,0 };
			
			XMFLOAT3 zCamera = { 0,0,1 };
			


			if (input->isKey(DIK_W))
			{
				camera->CameraMoveVector(yCamera,true);//�J�����𓮂���
			}
			if (input->isKey(DIK_D))
			{
				camera->CameraMoveVector(xCamera,true);//�J�����𓮂���
			}
			if (input->isKey(DIK_A))
			{
				camera->CameraMoveVector(xCamera,false);//�J�����𓮂���
			}
			if (input->isKey(DIK_S))
			{
				camera->CameraMoveVector(yCamera,false);//�J�����𓮂���
			}
			if (input->isKey(DIK_Q))
			{
				camera->CameraMoveVector(zCamera,false);
			}
			if (input->isKey(DIK_E))
			{
				camera->CameraMoveVector(zCamera,true);
			}

			if (input->isKey(DIK_UP))
			{
				camera->CameraRot(0.0f, 0.1f);
			}
		
		gameScene->Update();

		skydome->Update();
		camera->UpdateCamera();

		object1->Update();
		
		
		//�|�X�g�G�t�F�N�g

		//postEffect->PreDrawScene(dxcommon->GetCmdlist());
		
		//object1->Draw(dxcommon->GetCmdlist());
		//postEffect->PostDrawScene(dxcommon->GetCmdlist());
		
		dxcommon->BeginDraw();

		// �S�D�`��R�}���h��������

		//object1->Draw(dxcommon->GetCmdlist());
		//postEffect->Draw(dxcommon->GetCmdlist());
		
		////obj�`��
		gameScene->Draw();
		Object3d::PreDraw(dxcommon->GetCmdlist());
		skydome->Draw();
		Object3d::PostDraw();

		////�X�v���C�g���ʃR�}���h
		// �X�v���C�g
		spriteCommon->PreDraw();
		sprite->Draw();
		debugtext_minute->DrawAll();
		//debugtext_minute->DrawAll();
		//debugtext_minute2->DrawAll();
		// �S�D�`��R�}���h�����܂�
		
		// �T�D���\�[�X�o���A��߂�

		dxcommon->EndDraw();
		
	}
	// �E�B���h�E�N���X��o�^����
	win->WinFinalize();

	delete modelFbx;
	delete object1;

	//Audio���
	audio->Finalize();
	delete audio;

	//�f�o�b�O�e�L�X�g���
	delete postEffect;

	debugtext_minute->Finalize();
	delete debugtext_minute;

	delete sprite;
	delete spriteCommon;
	LoadFbx::GetInstance()->Finalize();
	delete dxcommon;
	delete input;
	
	return 0;
}