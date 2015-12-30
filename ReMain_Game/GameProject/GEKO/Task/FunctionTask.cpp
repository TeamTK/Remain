#include "FunctionTask.h"
#include <iostream>

FunctionTask::FunctionTask()
{

}

FunctionTask::~FunctionTask()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();
	for (; it != itEnd; )
	{
		it = m_funcMap.erase(it);
	}
	m_funcMap.clear();
}

void FunctionTask::Regist(const char *name, std::function<void()> func)
{
	m_funcMap[name] = FuncTaskInfo(func);
}

void FunctionTask::Sleep(const char *name)
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();
	for (; it != itEnd; it++)
	{
		if (it->first == name)
		{
			it->second.m_IsSleep = true;
			break;
		}
	}
}

void FunctionTask::AllSleep()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();
	for (; it != itEnd; it++)
	{
		it->second.m_IsSleep = true;
	}
}

void FunctionTask::Awake(const char *name)
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();
	for (; it != itEnd; it++)
	{
		if (it->first == name)
		{
			it->second.m_IsSleep = false;
			break;
		}
	}
}

void FunctionTask::AllAwake()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();
	for (; it != itEnd; it++)
	{
		it->second.m_IsSleep = false;
	}
}

void FunctionTask::Update()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();

	for (; it != itEnd; it++)
	{
		if (it->second.m_IsSleep) continue;
		it->second.func();
	}
}

void FunctionTask::OperationDraw()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();

	for (; it != itEnd; it++)
	{
		if (it->second.m_IsSleep) continue;
		std::cout << it->first << "\n";
	}
}

void FunctionTask::RegistDraw()
{
	auto it = m_funcMap.begin();
	auto itEnd = m_funcMap.end();

	for (; it != itEnd; it++)
	{
		std::cout << it->first << "\n";
	}
}