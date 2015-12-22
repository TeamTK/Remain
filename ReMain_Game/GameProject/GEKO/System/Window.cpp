#include "Window.h"
#include "Direct3D11.h"
#include "Fps.h"
#include <stdio.h>

#pragma warning(disable : 4302)

#define GetMonitorRect(rc)  SystemParametersInfo(SPI_GETWORKAREA,0,rc,0)

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Window::Get()->WndProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() :
	m_hWnd(NULL),
	m_isFixing(false)
{
}

Window* Window::Get()
{
	static Window window;
	return &window;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			//PostQuitMessage(0);
			DestroyWindow(m_hWnd);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// �E�C���h�E �T�C�Y�̕ύX����
	case WM_SIZE:
		if (!Direct3D11::Get().GetID3D11Device() || wParam == SIZE_MINIMIZED)
			break;

		//�E�C���h�E�X�V
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		m_WinSize.sWidth = rc.right;
		m_WinSize.sHeight = rc.bottom;

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT Window::InitWindow(LPCSTR Winname, INT WinWidth, INT WinHeight)
{
	m_WinSize.sWidth = WinWidth;
	m_WinSize.sHeight = WinHeight;
	m_OriginWinSize.sWidth = WinWidth;
	m_OriginWinSize.sHeight = WinHeight;

	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	HINSTANCE hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	// �E�B���h�E�̒�`
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE("..\\myicon.ico"));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = Winname;
	wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE("..\\smyicon.ico"));
	RegisterClassEx(&wc);

	//�E�B���h�E�T�C�Y�Œ蔻�f
	if (m_isFixing)
	{
		//�E�B���h�E�T�C�Y�Œ�
		m_hWnd = CreateWindow(Winname, Winname, WINDOW_FIXING,
							  0, 0, WinWidth + 18, WinHeight + 47, 0, 0, hInst, 0);
	}
	else
	{
		//�E�B���h�E�T�C�Y�ύX�\
		m_hWnd = CreateWindow(Winname, Winname, WS_OVERLAPPEDWINDOW,
							  0, 0, WinWidth + 18, WinHeight + 47, 0, 0, hInst, 0);
	}

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	//�E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	RECT    rc1;        // �f�X�N�g�b�v�̈�
	RECT    rc2;        // �E�C���h�E�̈�
	INT     cx, cy;     // �E�C���h�E�ʒu
	INT     sx, sy;     // �E�C���h�E�T�C�Y

	// �T�C�Y�̎擾
	GetMonitorRect(&rc1);									// �f�X�N�g�b�v�̃T�C�Y
	GetWindowRect(m_hWnd, &rc2);                            // �E�C���h�E�̃T�C�Y

	//��ʒ������v�Z
	sx = (rc2.right - rc2.left);                            // �E�C���h�E�̉���
	sy = (rc2.bottom - rc2.top);                            // �E�C���h�E�̍���
	cx = (((rc1.right - rc1.left) - sx) / 2 + rc1.left);    // �������̒������W��
	cy = (((rc1.bottom - rc1.top) - sy) / 2 + rc1.top);     // �c�����̒������W��

	RECT rc3;
	GetClientRect(m_hWnd, &rc3);
	int w = 0;
	int h = 0;
	w += sx - (rc3.right - rc3.left);
	h += sy - (rc3.bottom - rc3.top);

	// ��ʒ����Ɉړ�
	SetWindowPos(m_hWnd, NULL, cx, cy, w, h, (SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER));

	return S_OK;
}

HWND Window::GetWindowHandle() const
{
	return m_hWnd;
}

WindowSize *Window::GetWindowSize()
{
	return &m_WinSize;
}

WindowSize *Window::GetWindowOriginSize()
{
	return &m_OriginWinSize;
}

void Window::SetFixing()
{
	m_isFixing = true;
}

void Window::NoDrawFrame()
{
	SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
}

void Window::DrawFps()
{
	//FPS�v�Z�\��
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf_s(str, "fps=%d", frame);
	if (timeGetTime() - time>1000)
	{
		time = timeGetTime();
		frame = 0;
		SetWindowTextA(m_hWnd, str);
	}
}

void Window::WindowEnd() const
{
	DestroyWindow(m_hWnd);
}

bool Window::Loop()
{
	//�t���[�����[�g����
	Fps::Update();
	Fps::Wait();

	if (m_msg.message != WM_QUIT)
	{
		// ���b�Z�[�W���擾���AWM_QUIT���ǂ���
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);  //�L�[�{�[�h���p���\�ɂ���
			DispatchMessage(&m_msg);   //�����Windows�ɖ߂�
		}
	}
	else
	{
		return false;
	}

	//����ʂ�`��
	Direct3D11::Get().Present();

	//��ʍX�V
	Direct3D11::Get().Clear(0.4f, 0.4f, 1.0f);

	return true;
}