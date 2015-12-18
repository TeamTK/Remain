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

	void KeyManagement::Update()
	{
		//マウスの状態を取得
		if (FAILED(KeyManagement::Get().GetMouseDevice8()->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState)))
		{
			KeyManagement::Get().GetMouseDevice8()->Acquire();
		}

		//XInputの状態を取得
		XInputGetState(0, &m_XInputState[0]);
		XInputGetState(1, &m_XInputState[1]);
		XInputGetState(2, &m_XInputState[2]);
		XInputGetState(3, &m_XInputState[3]);
	}

	LPDIRECTINPUTDEVICE8& KeyManagement::GetKeyDevice8()
	{
		return m_KeyDevice;
	}

	LPDIRECTINPUTDEVICE8& KeyManagement::GetMouseDevice8()
	{
		return m_MouseDevice;
	}

	DIMOUSESTATE2* KeyManagement::GetMouseState()
	{
		return &m_MouseState;
	}

	XINPUT_STATE* KeyManagement::GetXInputState(int index)
	{
		return &m_XInputState[index];
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
		m_pDims = KeyManagement::Get().GetMouseState();
	}

	KeyMouse::~KeyMouse()
	{

	}

	EMouseWheel KeyMouse::GetWheelState()
	{
		//ホイール量の差で上に回した判定
		int difference = m_WheelNum;
		m_WheelNum += m_pDims->lZ;
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
		return Point((float)m_pDims->lX, (float)m_pDims->lY);
	}

	float KeyMouse::GetWheelAmount()
	{
		return (float)m_pDims->lZ;
	}

	bool KeyMouse::WheelPressed()
	{
		if (m_pDims->rgbButtons[2] & 0x80) return true;
		return false;
	}

	bool KeyMouse::WheelClicked()
	{
		if ((m_pDims->rgbButtons[2] & 0x80) && !mf_Wheel)
		{
			mf_Wheel = true;
			return true;
		}
		else
		{
			if (!(m_pDims->rgbButtons[2] & 0x80)) mf_Wheel = false;
		}

		return false;
	}

	bool KeyMouse::LPressed()
	{
		if (m_pDims->rgbButtons[0] & 0x80) return true;

		return false;
	}

	bool KeyMouse::LClicked()
	{
		if ((m_pDims->rgbButtons[0] & 0x80) && !mf_LeftMouse)
		{
			mf_LeftMouse = true;
			return true;
		}
		else
		{
			if (!(m_pDims->rgbButtons[0] & 0x80)) mf_LeftMouse = false;
		}

		return false;
	}

	bool KeyMouse::RPressed()
	{
		if (m_pDims->rgbButtons[1] & 0x80) return true;
		return false;
	}

	bool KeyMouse::RClicked()
	{
		if ((m_pDims->rgbButtons[1] & 0x80) && !mf_RightMouse)
		{
			mf_RightMouse = true;
			return true;
		}
		else
		{
			if (!(m_pDims->rgbButtons[1] & 0x80)) mf_RightMouse = false;
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

	KeyXInputPad::KeyXInputPad(int index) :
		m_keyState(0)
	{
		m_pXInputState = KeyManagement::Get().GetXInputState(index);
	}

	KeyXInputPad::~KeyXInputPad()
	{
		m_pXInputState = nullptr;
	}

	bool KeyXInputPad::UpPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) return true;
		return false;
	}

	bool KeyXInputPad::UpClicked()
	{
		bool isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		if (isPad && !(m_keyState & eUp))
		{
			m_keyState |= eUp;
			return true;
		}
		else if(!isPad) m_keyState &= ~eUp;

		return false;
	}

	bool KeyXInputPad::DownPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) return true;
		return false;
	}

	bool KeyXInputPad::DownClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		if (isPad && !(m_keyState & eDown))
		{
			m_keyState |= eDown;
			return true;
		}
		else if (!isPad) m_keyState &= ~eDown;

		return false;
	}

	bool KeyXInputPad::LeftPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) return true;
		return false;
	}

	bool KeyXInputPad::LeftClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		if (isPad && !(m_keyState & eLeft))
		{
			m_keyState |= eLeft;
			return true;
		}
		else if (!isPad) m_keyState &= ~eLeft;

		return false;
	}

	bool KeyXInputPad::RightPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) return true;
		return false;
	}

	bool KeyXInputPad::RightClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
		if (isPad && !(m_keyState & eRight))
		{
			m_keyState |= eRight;
			return true;
		}
		else if (!isPad) m_keyState &= ~eRight;

		return false;
	}

	bool KeyXInputPad::StartPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_START) return true;
		return false;
	}

	bool KeyXInputPad::StartClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_START;
		if (isPad && !(m_keyState & eStart))
		{
			m_keyState |= eStart;
			return true;
		}
		else if (!isPad) m_keyState &= ~eStart;

		return false;
	}

	bool KeyXInputPad::BackPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_BACK) return true;
		return false;
	}

	bool KeyXInputPad::BackClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		if (isPad && !(m_keyState & eBack))
		{
			m_keyState |= eBack;
			return true;
		}
		else if (!isPad) m_keyState &= ~eBack;

		return false;
	}

	bool KeyXInputPad::ThumbLeftPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) return true;
		return false;
	}

	bool KeyXInputPad::ThumbLeftClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
		if (isPad && !(m_keyState & eLeftThumb))
		{
			m_keyState |= eLeftThumb;
			return true;
		}
		else if (!isPad) m_keyState &= ~eLeftThumb;

		return false;
	}

	bool KeyXInputPad::ThumbRightPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) return true;
		return false;
	}

	bool KeyXInputPad::ThumbRightClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
		if (isPad && !(m_keyState & eRightThumb))
		{
			m_keyState |= eRightThumb;
			return true;
		}
		else if (!isPad) m_keyState &= ~eRightThumb;

		return false;
	}

	bool KeyXInputPad::ShoulderLeftPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) return true;
		return false;
	}

	bool KeyXInputPad::ShoulderLeftClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
		if (isPad && !(m_keyState & eLeftShoulder))
		{
			m_keyState |= eLeftShoulder;
			return true;
		}
		else if (!isPad) m_keyState &= ~eLeftShoulder;

		return false;
	}

	bool KeyXInputPad::ShoulderRightPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) return true;
		return false;
	}

	bool KeyXInputPad::ShoulderRightClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
		if (isPad && !(m_keyState & eRightShoulder))
		{
			m_keyState |= eRightShoulder;
			return true;
		}
		else if (!isPad) m_keyState &= ~eRightShoulder;

		return false;
	}

	bool KeyXInputPad::APressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_A) return true;
		return false;
	}

	bool KeyXInputPad::AClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_A;
		if (isPad && !(m_keyState & eA))
		{
			m_keyState |= eA;
			return true;
		}
		else if (!isPad) m_keyState &= ~eA;

		return false;
	}

	bool KeyXInputPad::BPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_B) return true;
		return false;
	}

	bool KeyXInputPad::BClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_B;
		if (isPad && !(m_keyState & eB))
		{
			m_keyState |= eB;
			return true;
		}
		else if (!isPad) m_keyState &= ~eB;

		return false;
	}

	bool KeyXInputPad::XPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_X) return true;
		return false;
	}

	bool KeyXInputPad::XClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_X;
		if (isPad && !(m_keyState & eX))
		{
			m_keyState |= eX;
			return true;
		}
		else if (!isPad) m_keyState &= ~eX;

		return false;
	}

	bool KeyXInputPad::YPressed()
	{
		if (m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_Y) return true;
		return false;
	}

	bool KeyXInputPad::YClicked()
	{
		unsigned int isPad = m_pXInputState->Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		if (isPad && !(m_keyState & eY))
		{
			m_keyState |= eY;
			return true;
		}
		else if (!isPad) m_keyState &= ~eY;

		return false;
	}

	int KeyXInputPad::TriggerLeft()
	{
		return (int)m_pXInputState->Gamepad.bLeftTrigger;
	}

	int KeyXInputPad::TriggerRight()
	{
		return (int)m_pXInputState->Gamepad.bRightTrigger;
	}

	int KeyXInputPad::ThumbLeftX()
	{
		return (int)m_pXInputState->Gamepad.sThumbLX;
	}

	int KeyXInputPad::ThumbLeftY()
	{
		return (int)m_pXInputState->Gamepad.sThumbLY;
	}

	int KeyXInputPad::ThumbRightX()
	{
		return (int)m_pXInputState->Gamepad.sThumbRX;
	}

	int KeyXInputPad::ThumbRightY()
	{
		return (int)m_pXInputState->Gamepad.sThumbRY;
	}

	void KeyXInputPad::Vibration(int index, int leftMotorSpeed, int rightMotorSpeed)
	{
		XINPUT_VIBRATION v;
		v.wLeftMotorSpeed = leftMotorSpeed;
		v.wRightMotorSpeed = rightMotorSpeed;
		DWORD a = XInputSetState(index, &v);
	}

	bool KeyXInputPad::GetIsConnection()
	{
		if (m_pXInputState->dwPacketNumber)
		{
			m_pXInputState->dwPacketNumber = 0;
			return true;
		}
		return false;
	}
};
