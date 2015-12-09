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
	/// デバック始まりコンソール画面立ち上げ
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	static void Start();

	/// <summary>
	/// デバック終了コンソール画面消去
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	static void End();

	/// <summary>
	/// 文字をコンソールに表示
	/// 使用例 : Debug::Log("test" + Change::NumToString(1.0f));
	/// </summary>
	/// <param name="str">
	/// string型の文字
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	static void Log(std::string str);

	/// <summary>
	/// メモリリークが起きるとVS++の出力ウインドウに表示
	/// </summary>
	/// <returns>
	/// なし
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