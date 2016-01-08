#include "Task.h"
#include <iostream>
#include <map>
#include <list>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

Task::Task(const char* name, unsigned int priority) :
	m_Name(name),
	m_Priority(priority),
	m_IsKill(false),
	m_IsSleep(false)
{
	TaskManager::Add(this);
}

Task::Task()
{
}

void Task::SetKill()
{
	m_IsKill = true;
}

void Task::SetPriority(unsigned int priority)
{
	m_Priority = priority;
	TaskManager::Sort();
}

void Task::Stop()
{
	m_IsSleep = true;
}

void Task::Start()
{
	m_IsSleep = false;
}

bool Task::Running() const
{
	return m_IsSleep;
}

bool Task::GetKill() const
{
	return m_IsKill;
}

unsigned int Task::GetPriority() const
{
	return m_Priority;
}

const char *Task::GetName() const
{
	return m_Name;
}

void Task::Update()
{
	std::cout << m_Name << "基底タスクUpdateが呼ばれました" << "\n";
}

void Task::DrawName() const
{
	std::cout << "Task_Name : " << m_Name << "\n";
}

Task::~Task()
{
}

bool less(const Task *data1, const Task *data2)
{
	return data1->GetPriority() < data2->GetPriority();
};

class TaskManager::TaskPimpl
{
public:
	std::map<const char*, bool> stopList;
	std::list<Task *> taskList;
};

TaskManager::TaskManager()
{
	m_pTaskPimpl = new TaskPimpl;
}

TaskManager::~TaskManager()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	auto it = pList->begin();
	auto itEnd = pList->end();
	for (; it != itEnd; )
	{
		delete *it;
		it = pList->erase(it);
	}
	pList->clear();
	delete m_pTaskPimpl;
}

inline TaskManager* TaskManager::GetInstance()
{
	static TaskManager taskManager;
	return &taskManager;
}

void TaskManager::Kill(const char* taskName)
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList)
	{
		if (i->m_Name == taskName) i->SetKill();
	}
}

void TaskManager::AllKill()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList) i->SetKill();
}

void TaskManager::PartClear(const char* taskName)
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	auto it = pList->begin();
	auto itEnd = pList->end();
	for (; it != itEnd;)
	{
		//名前が一致したらリストから外す
		if ((*it)->m_Name == taskName)
		{
			delete *it;
			it = pList->erase(it);
		}
		else
		{
			it++;
		}
	}
}

void TaskManager::AllClear()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	auto it = pList->begin();
	auto itEnd = pList->end();
	for (; it != itEnd;)
	{
		delete *it;
		it = pList->erase(it);
	}
	pList->clear();
}

void TaskManager::Update()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	auto it = pList->begin();
	auto itEnd = pList->end();
	for (; it != itEnd; )
	{
		//タスクを削除
		if ((*it)->m_IsKill == true)
		{
			delete *it;
			it = pList->erase(it);
			continue;
		}

		if ((*it)->m_IsSleep == true)
		{
			it++;
			continue;
		}

		(*it)->Update();
		it++;
	}
}

void TaskManager::DrawName()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList)
	{
		std::cout << i->m_Priority << " : ";
		i->DrawName();
	}
}

void TaskManager::Stop(const char* taskName)
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;

	for (auto& i : *pList)
	{
		if (i->m_Name == taskName)
		{
			i->Stop();
		}
	}
}

void TaskManager::AllStop()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList) i->Stop();
}

void TaskManager::Start(const char* taskName)
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList)
	{
		if (i->m_Name == taskName) i->Start();
	}
}

void TaskManager::AllStart()
{
	std::list<Task*> *pList = &GetInstance()->m_pTaskPimpl->taskList;
	for (auto& i : *pList) i->Start();
}

void TaskManager::Add(Task* taskPointer)
{
	GetInstance()->m_pTaskPimpl->taskList.push_back(taskPointer);
	GetInstance()->m_pTaskPimpl->taskList.sort(&less);
}

void TaskManager::Sort()
{
	GetInstance()->m_pTaskPimpl->taskList.sort(&less);
}