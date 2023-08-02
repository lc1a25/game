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
#include "Light.h"

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
	//Light::StaticInitialize(dxcommon->GetDev());
	// ���C�g�ÓI������
	LightGroup::StaticInitialize(dxcommon->GetDev());
	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	//�Q�[���V�[��������
	gameScene = Reset(gameScene,dxcommon, input, audio,win);
	
	// DirectX�����������@�����܂�




#pragma region ������

	int gameSceneNum = 0;//������V�[��
	int gameSceneTitleNum = 0;//�^�C�g���V�[��
	int gameSceneInGameNum = 1;//�C���Q�[���V�[��
	int gameSceneClearNum = 2;//�N���A�V�[��
	int gameSceneGameOverNum = 3;//�Q�[���I�[�o�[�V�[��
	
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
	
		gameScene->SetGameFlag(gameSceneNum);//�V�[�����ǂ��ɂ��邩
		gameScene->SetHwnd(win->GetHwnd());//hwnd������ ���e�B�N���p
		gameScene->SetViewPort(dxcommon->GetViewPort());//�r���[�|�[�g������@���e�B�N���p

		if (gameSceneNum == gameSceneTitleNum)//�^�C�g��
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneInGameNum;//�C���Q�[����
			}
		}
		else if (gameSceneNum == gameSceneInGameNum)//�C���Q�[��
		{
			gameScene->Update();

			if (gameScene->playerDieFlag == true)//�v���C���[�����񂾂�
			{
				gameSceneNum = gameSceneGameOverNum;//�Q�[���I�[�o�[
			}
			if (gameScene->pointsLast == true)
			{
				gameSceneNum = gameSceneClearNum;//�Q�[���N���A
			}

		}
		//�Q�[���N���A
		else if (gameSceneNum == gameSceneClearNum)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneTitleNum;//�^�C�g���ɖ߂�
				gameScene = Reset(gameScene, dxcommon, input, audio,win);
			}
		}
		//�Q�[���I�[�o�[
		else if (gameSceneNum == gameSceneGameOverNum)
		{
			gameScene->Update();
			if (input->isMouseKey())
			{
				gameSceneNum = gameSceneTitleNum;//�^�C�g���ɖ߂�
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