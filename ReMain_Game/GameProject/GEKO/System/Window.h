#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>

#pragma comment(lib, "winmm.lib")
 
#define WINDOW_FIXING WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX

//ウインドウのクライアント領域のサイズ
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
	/// ウィンドウメッセージ処理
	/// </summary>
	/// <param name="hWnd">
	/// ウィンドウハンドル
	/// </param>
	/// <param name="uMsg">
	/// メッセージハンドル
	/// </param>
	/// <param name="wParam">
	/// 不明
	/// </param>
	/// <param name="lParam">
	/// 不明
	/// </param>
	/// <returns>
	/// 正常に終了
	/// </returns>
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// ウィンドウの初期化
	/// </summary>
	/// <param name="Winname">
	/// 青色
	/// </param>
	/// <param name="WinWidth">
	/// 青色
	/// </param>
	/// <param name="WinHeight">
	/// 青色
	/// </param>
	/// <returns>
	/// 初期化が成功の判断を返す
	/// </returns>
	HRESULT	InitWindow(LPCSTR Winname, INT WinWidth, INT WinHeight);

	/// <summary>
	/// ウィンドウハンドルを渡す
	/// </summary>
	/// <returns>
	/// ウィンドウハンドル
	/// </returns>
	HWND GetWindowHandle() const;

	/// <summary>
	/// ウィンドウのクライアント領域サイズを渡す
	/// </summary>
	/// <returns>
	/// ウィンドウのクライアント領域サイズ
	/// </returns>
	const WindowSize *GetWindowSize() const;

	/// <summary>
	/// ウィンドウのクライアント領域サイズを渡す(最初に設定したサイズ)
	/// </summary>
	/// <returns>
	/// ウィンドウのクライアント領域サイズ(最初に設定したサイズ)
	/// </returns>
	const WindowSize *GetWindowOriginSize() const;

	void SetScreenColor(float red, float green, float blue);

	/// <summary>
	/// ウィンドウを固定する
	/// </summary>
	/// <param name="isFixing">
	/// true : 固定 false : 変更可能
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void SetFixing();

	/// <summary>
	/// ウィンドウの枠を表示しない
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void NoDrawFrame();

	/// <summary>
	/// フレームレートを表示
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void DrawFps();

	/// <summary>
	/// ウィンドウを終了する
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void WindowEnd() const;

	/// <summary>
	/// ループ処理
	/// </summary>
	/// <returns>
	/// ループが可能ならtrue
	/// </returns>
	bool Loop();

private:
	Window();

private:
	HWND m_hWnd;				//ウインドウのハンドル
	MSG  m_msg;					//メッセージハンドル
	WindowSize m_WinSize;		//ウインドウのサイズ横と縦
	WindowSize m_OriginWinSize; //最初に設定した幅と高さ
	bool m_IsFixing;			//ウィンドウサイズを固定判断
	float m_Red;				//画面クリア色（赤）
	float m_Green;				//画面クリア色（緑）
	float m_Blue;				//画面クリア色（青）
};

#endif 