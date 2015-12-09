#include "Debug.h"
#include <Windows.h>
#include <io.h>
#include <Fcntl.h>
#include <string>

Debug::Debug()
{
}

Debug* const Debug::GetInstance()
{
	static Debug debug;
	return &debug;
}

void Debug::Start()
{
	//ˆê‰žŽc‚µ‚Ä‚¨‚­
	/*
	AllocConsole();
	GetInstance()->m_hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(GetInstance()->m_hConsole, "w");
	setvbuf(stdout, NULL, _IONBF, 0);
	*/

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
}

void Debug::End()
{
	//ˆê‰žŽc‚µ‚Ä‚¨‚­
	//_close(GetInstance()->m_hConsole);

	FreeConsole();
}

void Debug::Log(std::string str)
{
	std::cout << str << "\n";
}