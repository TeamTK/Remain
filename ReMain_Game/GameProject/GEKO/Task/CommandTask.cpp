#include "CommandTask.h"
#include <iostream>

CommandTask::CommandTask(const char* name) :
	m_Name(name)
{
}

CommandTask::~CommandTask()
{
}

void CommandTask::Update()
{
	std::cout << "CommandTask‚ÌUpdate‚ªŒÄ‚Î‚ê‚Ä‚¢‚Ü‚·" << "\n";
}

void CommandTask::DebugOperation()
{
	std::cout << "CommandTask Operation : " << m_Name << "\n";
}

CommandObject::CommandObject() :
	m_pCommandTask(nullptr)
{
}

CommandObject::~CommandObject()
{
}

void CommandObject::Regist(CommandTask *pCommand)
{
	if (m_pCommandTask != nullptr) delete m_pCommandTask;
	m_pCommandTask = pCommand;
}

void CommandObject::Update()
{
	m_pCommandTask->Update();
}

void CommandObject::DebugOperation()
{
	m_pCommandTask->DebugOperation();
}