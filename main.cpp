#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <math.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include<fstream>

#include "Object3d.h"

#include "Win.h"
#include "Input.h"
#include "DirectXCommon.h"
#include <algorithm>

#include "Audio.h"

#include "Matrix4.h"
#include "GameScene.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

using namespace DirectX;

GameScene* Reset(GameScene* gameScene, DirectXCommon* dxcommon, Input* input, Audio* audio,Win* win)
{
	if (gameScene != nullptr)
	{
		delete gameScene;
	}
	gameScene = new GameScene();
	gameScene->Init(dxcommon, input, audio,win);

	return gameScene;
}
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

	Object3d::StaticInitialize(dxcommon->GetDev(), win->window_width, win->window_height);

	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	//�Q�[���V�[��������
	gameScene = Reset(gameScene,dxcommon, input, audio,win);
	
	// DirectX�����������@�����܂�

	////�`�揉���������@��������
	
	//�`�揉���������@�����܂�



#pragma region ������

	int gameSceneNum = 0;
	
#pragma endregion

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
	
		gameScene->SetGameFlag(gameSceneNum);
		gameScene->SetHwnd(win->GetHwnd());
		gameScene->SetViewPort(dxcommon->GetViewPort());
		if (gameSceneNum == 0)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = 1;
			}
		}
		else if (gameSceneNum == 1)
		{
			gameScene->Update();

			if (gameScene->playerDieFlag == true)
			{
				gameSceneNum = 3;
			}
			if (gameScene->pointsLast == true)
			{
				gameSceneNum = 2;
			}

		}
		//�Q�[���N���A
		else if (gameSceneNum == 2)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameScene = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		//�Q�[���I�[�o�[
		else if (gameSceneNum == 3)
		{
			if (input->isKeyTrigger(DIK_SPACE))
			{
				gameScene = 0;
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		
		
//�`��
		
		dxcommon->BeginDraw();

		// �S�D�`��R�}���h��������

		gameScene->Draw();
		
		// �S�D�`��R�}���h�����܂�
		
		// �T�D���\�[�X�o���A��߂�

		dxcommon->EndDraw();
		
	}
	// �E�B���h�E�N���X��o�^����
	win->WinFinalize();

//���
	//Audio���
	audio->Finalize();
	delete audio;
	delete gameScene;
	delete dxcommon;
	delete input;
	
	return 0;
}