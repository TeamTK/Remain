#ifndef _INPUT_H_
#define _INPUT_H_

#define	DIRECTINPUT_VERSION	0x0800

#include "..\Include\\dinput.h"
#pragma comment(lib,"GEKO\\Lib\\dxguid.lib")
#pragma comment(lib,"GEKO\\Lib\\dinput8.lib")

struct Point
{
	float x;
	float y;
	Point() :
		x(0.0f), y(0.0f)
	{
	}
	Point(float x, float y) :
		x(x), y(y)
	{
	}
};

namespace Input
{
	class KeyManagement
	{
	public:
		/// <summary>
		/// インスタンスを生成して返す
		/// </summary>
		/// <returns>
		/// KeyManagementのインスタンス
		/// </returns>
		static KeyManagement& Get();

		/// <summary>
		/// DirectInputのキーボードとマウス初期化
		/// </summary>
		void Init();

		/// <summary>
		/// DrectInputのキーボードとマウス終了処理
		/// </summary>
		void End();

		/// <summary>
		/// キーボードのドライバーを返す
		/// </summary>
		/// <returns>
		/// キーボードのドライバー
		/// </returns>
		LPDIRECTINPUTDEVICE8& GetKeyDevice8();

		/// <summary>
		/// マウスのドライバーを返す
		/// </summary>
		/// <returns>
		/// マウスのドライバー
		/// </returns>
		LPDIRECTINPUTDEVICE8& GetMouseDevice8();

	private:
		KeyManagement(){};

	private:
		LPDIRECTINPUT8		 m_Dinput;		//DirectInput
		LPDIRECTINPUTDEVICE8 m_KeyDevice;	//キーボードドライバー
		LPDIRECTINPUTDEVICE8 m_MouseDevice;	//マウスドライバー
	};

	class Key
	{
	public:
		/// <summary>
		/// キーの識別と初期化
		/// </summary>
		/// <param name="key">
		/// キーの識別番号
		/// </param>
		Key(int key);
		~Key();

		/// <summary>
		/// キーボードの押すを検出
		/// </summary>
		/// <returns>
		/// true:押した false:離した
		/// </returns>
		bool Pressed();

		/// <summary>
		/// キーボードを一回だけ押したを検出
		/// </summary>
		/// <returns>
		/// true:前は押してなく現在押した　false:前押したが現在押していない
		/// </returns>
		bool Clicked();

	private:
		int	 m_key;	 //キーの識別番号
		bool m_fkey; //キー管理
	};

	//マウスホイールの状態
	enum EMouseWheel
	{
		E_WHEEL_NO,
		E_WHEEL_UP,
		E_WHEEL_DOWN,
	};

	class KeyMouse
	{
	public:
		KeyMouse();
		~KeyMouse();

		/// <summary>
		/// マウスホイールの状態を返す
		/// </summary>
		/// <returns>
		/// 回していない：E_WHEEL_NO 上に回した：E_WHEEL_UP 下に回した：E_WHEEL_DOWN
		/// </returns>
		EMouseWheel GetWheelState();

		/// <summary>
		/// マウスのスクリーン座標を返す
		/// </summary>
		/// <returns>
		/// スクリーン座標のXとY
		/// </returns>
		Point GetPos();

		/// <summary>
		/// マウスの相対値を返す
		/// </summary>
		/// <returns>
		/// 相対値のXとY
		/// </returns>
		Point GetRelativeValue();

		/// <summary>
		/// 現在のホイール量を返す
		/// </summary>
		/// <returns>
		/// 現在のホイール量
		/// </returns>
		int GetWheelAmount();

		/// <summary>
		/// ホイールを押した判定を返す
		/// </summary>
		/// <returns>
		/// true：押した false：押していない
		/// </returns>
		bool WheelPressed();

		/// <summary>
		/// ホイールを一回だけ押す判定
		/// </summary>
		/// <returns>
		/// true:前は押してなく現在押した　false:前押したが現在押していない
		/// </returns>
		bool WheelClicked();

		/// <summary>
		/// 左クリック判定
		/// </summary>
		/// <returns>
		/// true：押した false：押していない
		/// </returns>
    	bool LPressed();

		/// <summary>
		/// 左クリック一回判定
		/// </summary>
		/// <returns>
		/// true:前は押してなく現在押した　false:前押したが現在押していない
		/// </returns>
		bool LClicked();

		/// <summary>
		/// 右クリック判定
		/// </summary>
		/// <returns>
		/// true：押した false：押していない
		/// </returns>
		bool RPressed();

		/// <summary>
		/// 右クリック一回判定
		/// </summary>
		/// <returns>
		/// true:前は押してなく現在押した　false:前押したが現在押していない
		/// </returns>
		bool RClicked();

		bool DoubleClick(int time);

	private:
		int  m_WheelNum;	//ホイール量
		bool mf_Wheel;      //ホイール管理
		bool mf_LeftMouse;	//マウス左クリック管理
		bool mf_RightMouse; //マウス右クリック管理
		POINT m_Relative;   //マウスの相対値
	};

	//キーボード
	static Key KeyA(DIK_A);
	static Key KeyB(DIK_B);
	static Key KeyC(DIK_C);
	static Key KeyD(DIK_D);
	static Key KeyE(DIK_E);
	static Key KeyF(DIK_F);
	static Key KeyG(DIK_G);
	static Key KeyH(DIK_H);
	static Key KeyI(DIK_I);
	static Key KeyJ(DIK_J);
	static Key KeyK(DIK_K);
	static Key KeyL(DIK_L);
	static Key KeyM(DIK_M);
	static Key KeyN(DIK_N);
	static Key KeyO(DIK_O);
	static Key KeyP(DIK_P);
	static Key KeyQ(DIK_Q);
	static Key KeyR(DIK_R);
	static Key KeyS(DIK_S);
	static Key KeyT(DIK_T);
	static Key KeyU(DIK_U);
	static Key KeyV(DIK_V);
	static Key KeyW(DIK_W);
	static Key KeyX(DIK_X);
	static Key KeyY(DIK_Y);
	static Key KeyZ(DIK_Z);
	static Key Key0(DIK_0);
	static Key Key1(DIK_1);
	static Key Key2(DIK_2);
	static Key Key3(DIK_3);
	static Key Key4(DIK_4);
	static Key Key5(DIK_5);
	static Key Key6(DIK_6);
	static Key Key7(DIK_7);
	static Key Key8(DIK_8);
	static Key Key9(DIK_9);
	static Key KeyEscape(DIK_ESCAPE);
	static Key KeySpace(DIK_SPACE);
	static Key KeyBackSpace(DIK_BACKSPACE);
	static Key KeyUp(DIK_UP);
	static Key KeyDown(DIK_DOWN);
	static Key KeyRight(DIK_RIGHT);
	static Key KeyLeft(DIK_LEFT);
	static Key KeyLShift(DIK_LSHIFT);
	static Key KeyRShift(DIK_RSHIFT);
	static Key KeyLControl(DIK_LCONTROL);
	static Key KeyRControl(DIK_RCONTROL);
	static Key KeyTab(DIK_TAB);

	//マウス
	static KeyMouse Mouse;
}

#endif