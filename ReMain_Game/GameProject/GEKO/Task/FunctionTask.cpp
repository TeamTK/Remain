#include "FunctionTask.h"
#include <iostream>

FunctionTask::FunctionTask()
{

}

FunctionTask::~FunctionTask()
{
	m_funcMap.clear();
}

bool FunctionTask::Running(const char *name)
{
	return m_funcMap[name].m_IsRunning;
}

void FunctionTask::Regist(const char *name, std::function<void()> func)
{
	m_funcMap[name] = FuncTaskInfo(func);
}

void FunctionTask::Stop(const char *name)
{
	m_funcMap[name].m_IsRunning = false;
}

void FunctionTask::AllStop()
{
	for (auto& i : m_funcMap) i.second.m_IsRunning = false;
}

void FunctionTask::Start(const char *name)
{
	m_funcMap[name].m_IsRunning = true;
}

void FunctionTask::AllStart()
{
	for (auto& i : m_funcMap) i.second.m_IsRunning = true;
}

void FunctionTask::Update()
{
	for (auto& i : m_funcMap)
	{
		if (!i.second.m_IsRunning) continue;
		i.second.func();
	}
}

void FunctionTask::RunningDraw()
{
	for (auto& i : m_funcMap)
	{
		if (!i.second.m_IsRunning) continue;
		std::cout << i.first << "\n";
	}
}

void FunctionTask::RegistDraw()
{
	for (auto& i : m_funcMap) std::cout << i.first << "\n";
}