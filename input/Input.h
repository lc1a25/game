#pragma once

#define DIRECTINPUT_VERSION 0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "Win.h"
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define ALL_KEYS 256



//using namespace Microsoft::WRL;

class Input
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Win* win = nullptr;

public:
	static BYTE keys[ALL_KEYS];
	static BYTE oldkeys[ALL_KEYS];
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;

	ComPtr<IDirectInputDevice8> Devmouse;

	static DIMOUSESTATE2 mouseState;
	static DIMOUSESTATE2 beforeMouse;


bool isKey(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex];
	}

	bool isKeyTrigger(int keyIndex) {
		if (keyIndex < 0 || keyIndex > 255)return false;
		return keys[keyIndex] && !oldkeys[keyIndex];
	}

	bool isMouseKey() {
		if (mouseState.rgbButtons[0])
		{
			return true;
		}
		return false;
	}



	void Initialize(Win*win);


	//�X�V����
	void Update();

	void Finalize();

	
	
};

