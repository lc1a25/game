#include "Input.h"
#define DIRECTINPUT_VERSION 0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "Win.h"
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

BYTE Input::keys[ALL_KEYS] = { 0 };
BYTE Input::oldkeys[ALL_KEYS] = { 0 };

void Input::Initialize(Win*win)
{
	this->win = win;
	HRESULT result;
	//DirectInput�I�u�W�F�N�g�̐���

	result = DirectInput8Create(
		win->GetWindowHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	//�L�[�{�[�h�f�o�C�X�̐���

	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//���̓f�[�^�̌`���Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	result = dinput->CreateDevice(GUID_SysMouse, &Devmouse, NULL);
	result = Devmouse->SetDataFormat(&c_dfDIMouse2);

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	/*//��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����@//�f�o�C�X�����̃A�v�������Ő�L���Ȃ�//Windows�L�[�𖳌��ɂ���*/




}

void Input::Update()
{
	HRESULT result;
	for (int i = 0; i < ALL_KEYS; ++i) {
		oldkeys[i] = keys[i];
	}
	result = devkeyboard->Acquire();
	result = devkeyboard->GetDeviceState(sizeof(keys), keys);

	result = Devmouse->Acquire();

	beforeMouse = mouseState;

	result = Devmouse->GetDeviceState(sizeof(mouseState), &mouseState);
}




