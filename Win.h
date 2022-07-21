#pragma once
#include <Windows.h>

class Win
{
public:
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

	WNDCLASSEX windowClass{}; // �E�B���h�E�N���X�̐ݒ�

	HWND hwnd = nullptr;	// �E�B���h�E�n���h��
	
	MSG msg{};  // ���b�Z�[�W


	//�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetWindowHandle() { return windowClass.hInstance; }// �E�B���h�E�n���h��

	LPCWSTR GetWindowProc() { return windowClass.lpszClassName; }// �E�B���h�E�v���V�[�W����ݒ�

	MSG GetMsg() { return msg; } // ���b�Z�[�W

	LPMSG GetLPMsg() { return &msg; } // ���b�Z�[�W

	void WinCreate();//WindowAPI������

	void WinFinalize();

	bool ProcessMessage();

};

