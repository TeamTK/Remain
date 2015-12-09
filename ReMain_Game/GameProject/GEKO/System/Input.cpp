#include "Input.h"
#include "Window.h"
#include "Direct3D11.h"
#include <stdio.h>

namespace Input
{
	KeyManagement& KeyManagement::Get()
	{
		static KeyManagement keymanagement;
		return keymanagement;
	}

	void KeyManagement::Init()
	{
		// 「DirectInput」オブジェクトの作成
		if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
			DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_Dinput, NULL)))
		{
			//MessageBox(0, L"DirectInputMouse初期化失敗", NULL, MB_OK);
		}
		// 「DirectInputデバイス」オブジェクトの作成
		if (FAILED(m_Dinput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL)))
		{
			//MessageBox(0, L"DirectInputMouse初期化失敗", NULL, MB_OK);
		}
		// デバイスをマウスに設定
		if (FAILED(m_MouseDevice->SetDataFormat(&c_dfDIMouse2)))
		{
			//MessageBox(0, L"DirectInputMouse初期化失敗", NULL, MB_OK);
		}
		// 協調レベルの設定
		if (FAILED(m_MouseDevice->SetCooperativeLevel(Window::Get()->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			//MessageBox(0, L"DirectInputMouse初期化失敗", NULL, MB_OK);
		}

		m_MouseDevice->Acquire();

		// 「DirectInput」オブジェクトの作成
		if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
			DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_Dinput, NULL)))
		{
			//MessageBox(0, L"DirectInput初期化失敗", NULL, MB_OK);
		}
		// 「DirectInputデバイス」オブジェクトの作成
		if (FAILED(m_Dinput->CreateDevice(GUID_SysKeyboard, &m_KeyDevice, NULL)))
		{
			//MessageBox(0, L"DirectInput初期化失敗", NULL, MB_OK);
		}
		// デバイスをキーボードに設定
		if (FAILED(m_KeyDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			//MessageBox(0, L"DirectInput初期化失敗", NULL, MB_OK);
		}
		// 協調レベルの設定
		if (FAILED(m_KeyDevice->SetCooperativeLevel(Window::Get()->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			//MessageBox(0, L"DirectInput初期化失敗", NULL, MB_OK);
		}

		// デバイスを「取得」する
		m_KeyDevice->Acquire();
	}

	void KeyManagement::End()
	{
		SAFE_RELEASE(m_MouseDevice);
		SAFE_RELEASE(m_KeyDevice);
		SAFE_RELEASE(m_Dinput);
		OutputDebugString(L"KeyManagementが正常に終了しました\n");
	}

	LPDIRECTINPUTDEVICE8& KeyManagement::GetKeyDevice8()
	{
		return m_KeyDevice;
	}

	LPDIRECTINPUTDEVICE8& KeyManagement::GetMouseDevice8()
	{
		return m_MouseDevice;
	}

	Key::Key(int key) :
		m_key(key),
		m_fkey(false)
	{
	}

	Key::~Key()
	{
	}

	bool Key::Pressed()
	{
		HRESULT hr = KeyManagement::Get().GetKeyDevice8()->Acquire();
		if ((hr == DI_OK) || (hr == S_FALSE))
		{
			BYTE diks[256];
			KeyManagement::Get().GetKeyDevice8()->GetDeviceState(sizeof(diks), &diks);

			if (diks[m_key] & 0x80) return true;
		}
		return false;
	}

	bool Key::Clicked()
	{
		HRESULT hr = KeyManagement::Get().GetKeyDevice8()->Acquire();
		if ((hr == DI_OK) || (hr == S_FALSE))
		{
			BYTE diks[256];
			KeyManagement::Get().GetKeyDevice8()->GetDeviceState(sizeof(diks), &diks);

			if ((diks[m_key] & 0x80) && !m_fkey)
			{
				m_fkey = true;
				return m_fkey;
			}
			else
			{
				if (!(diks[m_key] & 0x80)) m_fkey = false;
			}
		}
		return false;
	}

	KeyMouse::KeyMouse() :
		m_WheelNum(0),
		mf_Wheel(false),
		mf_LeftMouse(false),
		mf_RightMouse(false)
	{

	}

	KeyMouse::~KeyMouse()
	{

	}

	EMouseWheel KeyMouse::GetWheelState()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return E_WHEEL_NO;
		}
		//ホイール量の差で上に回した判定
		int difference = m_WheelNum;
		m_WheelNum += dims.lZ;
		if (m_WheelNum > difference) return E_WHEEL_UP;
		if (m_WheelNum < difference) return E_WHEEL_DOWN;					 

		return E_WHEEL_NO;
	}

	Point KeyMouse::GetPos()
	{
		static POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(Window::Get()->GetWindowHandle(), &ptMouse);

		return Point((float)ptMouse.x, (float)ptMouse.y);
	}

	Point KeyMouse::GetRelativeValue()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return Point();
		}

		return Point((float)dims.lX, (float)dims.lY);
	}

	int KeyMouse::GetWheelAmount()
	{
		static int WheelAmount;
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return WheelAmount;
		}

		WheelAmount += dims.lZ;
		return (int)(WheelAmount * 0.008);
	}

	bool KeyMouse::WheelPressed()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if (dims.rgbButtons[2] & 0x80) return true;

		return false;
	}

	bool KeyMouse::WheelClicked()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if ((dims.rgbButtons[2] & 0x80) && !mf_Wheel)
		{
			mf_Wheel = true;
			return true;
		}
		else
		{
			if (!(dims.rgbButtons[2] & 0x80)) mf_Wheel = false;
		}

		return false;
	}

	bool KeyMouse::LPressed()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if (dims.rgbButtons[0] & 0x80) return true;

		return false;
	}

	bool KeyMouse::LClicked()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if ((dims.rgbButtons[0] & 0x80) && !mf_LeftMouse)
		{
			mf_LeftMouse = true;
			return true;
		}
		else
		{
			if (!(dims.rgbButtons[0] & 0x80)) mf_LeftMouse = false;
		}

		return false;
	}

	bool KeyMouse::RPressed()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if (dims.rgbButtons[1] & 0x80) return true;

		return false;
	}

	bool KeyMouse::RClicked()
	{
		DIMOUSESTATE2 dims = { 0 };
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
			return false;
		}

		if ((dims.rgbButtons[1] & 0x80) && !mf_RightMouse)
		{
			mf_RightMouse = true;
			return true;
		}
		else
		{
			if (!(dims.rgbButtons[1] & 0x80)) mf_RightMouse = false;
		}

		return false;
	}

	bool KeyMouse::DoubleClick(int time)
	{
		// ダブルクリック間隔の設定
		//SetDoubleClickTime(time);

		// ダブルクリック間隔の取得
		// ms = GetDoubleClickTime();

		//if (ms >= time)
		//{
		//	return true;
		//}
		return false;
	}
};
