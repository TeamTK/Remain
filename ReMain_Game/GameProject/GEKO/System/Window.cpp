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

	// ウインドウ サイズの変更処理
	case WM_SIZE:
		if (!Direct3D11::Get().GetID3D11Device() || wParam == SIZE_MINIMIZED)
			break;

		//ウインドウ更新
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
	// ウィンドウの定義
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

	//ウィンドウサイズ固定判断
	if (m_isFixing)
	{
		//ウィンドウサイズ固定
		m_hWnd = CreateWindow(Winname, Winname, WINDOW_FIXING,
							  0, 0, WinWidth + 18, WinHeight + 47, 0, 0, hInst, 0);
	}
	else
	{
		//ウィンドウサイズ変更可能
		m_hWnd = CreateWindow(Winname, Winname, WS_OVERLAPPEDWINDOW,
							  0, 0, WinWidth + 18, WinHeight + 47, 0, 0, hInst, 0);
	}

	if (!m_hWnd)
	{
		return E_FAIL;
	}

	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	RECT    rc1;        // デスクトップ領域
	RECT    rc2;        // ウインドウ領域
	INT     cx, cy;     // ウインドウ位置
	INT     sx, sy;     // ウインドウサイズ

	// サイズの取得
	GetMonitorRect(&rc1);									// デスクトップのサイズ
	GetWindowRect(m_hWnd, &rc2);                            // ウインドウのサイズ

	//画面中央を計算
	sx = (rc2.right - rc2.left);                            // ウインドウの横幅
	sy = (rc2.bottom - rc2.top);                            // ウインドウの高さ
	cx = (((rc1.right - rc1.left) - sx) / 2 + rc1.left);    // 横方向の中央座標軸
	cy = (((rc1.bottom - rc1.top) - sy) / 2 + rc1.top);     // 縦方向の中央座標軸

	RECT rc3;
	GetClientRect(m_hWnd, &rc3);
	int w = 0;
	int h = 0;
	w += sx - (rc3.right - rc3.left);
	h += sy - (rc3.bottom - rc3.top);

	// 画面中央に移動
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
	//FPS計算表示
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
	//フレームレート制御
	Fps::Update();
	Fps::Wait();

	if (m_msg.message != WM_QUIT)
	{
		// メッセージを取得し、WM_QUITかどうか
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);  //キーボード利用を可能にする
			DispatchMessage(&m_msg);   //制御をWindowsに戻す
		}
	}
	else
	{
		return false;
	}

	//裏画面を描画
	Direct3D11::Get().Present();

	//画面更新
	Direct3D11::Get().Clear(0.4f, 0.4f, 1.0f);

	return true;
}