#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>

#pragma comment(lib, "winmm.lib")
 
#define WINDOW_FIXING WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX

//�E�C���h�E�̃N���C�A���g�̈�̃T�C�Y
struct WindowSize
{
	int width;
	int height;
};

class Window
{
public:
	static Window* Get();

	/// <summary>
	/// �E�B���h�E���b�Z�[�W����
	/// </summary>
	/// <param name="hWnd">
	/// �E�B���h�E�n���h��
	/// </param>
	/// <param name="uMsg">
	/// ���b�Z�[�W�n���h��
	/// </param>
	/// <param name="wParam">
	/// �s��
	/// </param>
	/// <param name="lParam">
	/// �s��
	/// </param>
	/// <returns>
	/// ����ɏI��
	/// </returns>
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// �E�B���h�E�̏�����
	/// </summary>
	/// <param name="Winname">
	/// �F
	/// </param>
	/// <param name="WinWidth">
	/// �F
	/// </param>
	/// <param name="WinHeight">
	/// �F
	/// </param>
	/// <returns>
	/// �������������̔��f��Ԃ�
	/// </returns>
	HRESULT	InitWindow(LPCSTR Winname, INT WinWidth, INT WinHeight);

	/// <summary>
	/// �E�B���h�E�n���h����n��
	/// </summary>
	/// <returns>
	/// �E�B���h�E�n���h��
	/// </returns>
	HWND GetWindowHandle() const;

	/// <summary>
	/// �E�B���h�E�̃N���C�A���g�̈�T�C�Y��n��
	/// </summary>
	/// <returns>
	/// �E�B���h�E�̃N���C�A���g�̈�T�C�Y
	/// </returns>
	const WindowSize *GetWindowSize() const;

	/// <summary>
	/// �E�B���h�E�̃N���C�A���g�̈�T�C�Y��n��(�ŏ��ɐݒ肵���T�C�Y)
	/// </summary>
	/// <returns>
	/// �E�B���h�E�̃N���C�A���g�̈�T�C�Y(�ŏ��ɐݒ肵���T�C�Y)
	/// </returns>
	const WindowSize *GetWindowOriginSize() const;

	void SetScreenColor(float red, float green, float blue);

	/// <summary>
	/// �E�B���h�E���Œ肷��
	/// </summary>
	/// <param name="isFixing">
	/// true : �Œ� false : �ύX�\
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void SetFixing();

	/// <summary>
	/// �E�B���h�E�̘g��\�����Ȃ�
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void NoDrawFrame();

	/// <summary>
	/// �t���[�����[�g��\��
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void DrawFps();

	/// <summary>
	/// �E�B���h�E���I������
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void WindowEnd() const;

	/// <summary>
	/// ���[�v����
	/// </summary>
	/// <returns>
	/// ���[�v���\�Ȃ�true
	/// </returns>
	bool Loop();

private:
	Window();

private:
	HWND m_hWnd;				//�E�C���h�E�̃n���h��
	MSG  m_msg;					//���b�Z�[�W�n���h��
	WindowSize m_WinSize;		//�E�C���h�E�̃T�C�Y���Əc
	WindowSize m_OriginWinSize; //�ŏ��ɐݒ肵�����ƍ���
	bool m_IsFixing;			//�E�B���h�E�T�C�Y���Œ蔻�f
	float m_Red;				//��ʃN���A�F�i�ԁj
	float m_Green;				//��ʃN���A�F�i�΁j
	float m_Blue;				//��ʃN���A�F�i�j
};

#endif 