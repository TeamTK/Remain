#ifndef _DEBUG_
#define _DEBUG_

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <iostream>

class Debug
{
public:
	/// <summary>
	/// �f�o�b�N�n�܂�R���\�[����ʗ����グ
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	static void Start();

	/// <summary>
	/// �f�o�b�N�I���R���\�[����ʏ���
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	static void End();

	/// <summary>
	/// �������R���\�[���ɕ\��
	/// �g�p�� : Debug::Log("test" + Change::NumToString(1.0f));
	/// </summary>
	/// <param name="str">
	/// string�^�̕���
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	static void Log(std::string str);

	/// <summary>
	/// ���������[�N���N�����VS++�̏o�̓E�C���h�E�ɕ\��
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	static void SearchMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	};

private:
	Debug();
	static Debug* const GetInstance();

private:
	int m_hConsole;
};

#endif