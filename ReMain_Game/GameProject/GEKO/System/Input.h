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
		/// �C���X�^���X�𐶐����ĕԂ�
		/// </summary>
		/// <returns>
		/// KeyManagement�̃C���X�^���X
		/// </returns>
		static KeyManagement& Get();

		/// <summary>
		/// DirectInput�̃L�[�{�[�h�ƃ}�E�X������
		/// </summary>
		void Init();

		/// <summary>
		/// DrectInput�̃L�[�{�[�h�ƃ}�E�X�I������
		/// </summary>
		void End();

		/// <summary>
		/// �L�[�{�[�h�̃h���C�o�[��Ԃ�
		/// </summary>
		/// <returns>
		/// �L�[�{�[�h�̃h���C�o�[
		/// </returns>
		LPDIRECTINPUTDEVICE8& GetKeyDevice8();

		/// <summary>
		/// �}�E�X�̃h���C�o�[��Ԃ�
		/// </summary>
		/// <returns>
		/// �}�E�X�̃h���C�o�[
		/// </returns>
		LPDIRECTINPUTDEVICE8& GetMouseDevice8();

	private:
		KeyManagement(){};

	private:
		LPDIRECTINPUT8		 m_Dinput;		//DirectInput
		LPDIRECTINPUTDEVICE8 m_KeyDevice;	//�L�[�{�[�h�h���C�o�[
		LPDIRECTINPUTDEVICE8 m_MouseDevice;	//�}�E�X�h���C�o�[
	};

	class Key
	{
	public:
		/// <summary>
		/// �L�[�̎��ʂƏ�����
		/// </summary>
		/// <param name="key">
		/// �L�[�̎��ʔԍ�
		/// </param>
		Key(int key);
		~Key();

		/// <summary>
		/// �L�[�{�[�h�̉��������o
		/// </summary>
		/// <returns>
		/// true:������ false:������
		/// </returns>
		bool Pressed();

		/// <summary>
		/// �L�[�{�[�h����񂾂������������o
		/// </summary>
		/// <returns>
		/// true:�O�͉����ĂȂ����݉������@false:�O�����������݉����Ă��Ȃ�
		/// </returns>
		bool Clicked();

	private:
		int	 m_key;	 //�L�[�̎��ʔԍ�
		bool m_fkey; //�L�[�Ǘ�
	};

	//�}�E�X�z�C�[���̏��
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
		/// �}�E�X�z�C�[���̏�Ԃ�Ԃ�
		/// </summary>
		/// <returns>
		/// �񂵂Ă��Ȃ��FE_WHEEL_NO ��ɉ񂵂��FE_WHEEL_UP ���ɉ񂵂��FE_WHEEL_DOWN
		/// </returns>
		EMouseWheel GetWheelState();

		/// <summary>
		/// �}�E�X�̃X�N���[�����W��Ԃ�
		/// </summary>
		/// <returns>
		/// �X�N���[�����W��X��Y
		/// </returns>
		Point GetPos();

		/// <summary>
		/// �}�E�X�̑��Βl��Ԃ�
		/// </summary>
		/// <returns>
		/// ���Βl��X��Y
		/// </returns>
		Point GetRelativeValue();

		/// <summary>
		/// ���݂̃z�C�[���ʂ�Ԃ�
		/// </summary>
		/// <returns>
		/// ���݂̃z�C�[����
		/// </returns>
		int GetWheelAmount();

		/// <summary>
		/// �z�C�[���������������Ԃ�
		/// </summary>
		/// <returns>
		/// true�F������ false�F�����Ă��Ȃ�
		/// </returns>
		bool WheelPressed();

		/// <summary>
		/// �z�C�[������񂾂���������
		/// </summary>
		/// <returns>
		/// true:�O�͉����ĂȂ����݉������@false:�O�����������݉����Ă��Ȃ�
		/// </returns>
		bool WheelClicked();

		/// <summary>
		/// ���N���b�N����
		/// </summary>
		/// <returns>
		/// true�F������ false�F�����Ă��Ȃ�
		/// </returns>
    	bool LPressed();

		/// <summary>
		/// ���N���b�N��񔻒�
		/// </summary>
		/// <returns>
		/// true:�O�͉����ĂȂ����݉������@false:�O�����������݉����Ă��Ȃ�
		/// </returns>
		bool LClicked();

		/// <summary>
		/// �E�N���b�N����
		/// </summary>
		/// <returns>
		/// true�F������ false�F�����Ă��Ȃ�
		/// </returns>
		bool RPressed();

		/// <summary>
		/// �E�N���b�N��񔻒�
		/// </summary>
		/// <returns>
		/// true:�O�͉����ĂȂ����݉������@false:�O�����������݉����Ă��Ȃ�
		/// </returns>
		bool RClicked();

		bool DoubleClick(int time);

	private:
		int  m_WheelNum;	//�z�C�[����
		bool mf_Wheel;      //�z�C�[���Ǘ�
		bool mf_LeftMouse;	//�}�E�X���N���b�N�Ǘ�
		bool mf_RightMouse; //�}�E�X�E�N���b�N�Ǘ�
		POINT m_Relative;   //�}�E�X�̑��Βl
	};

	//�L�[�{�[�h
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

	//�}�E�X
	static KeyMouse Mouse;
}

#endif