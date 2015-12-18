#include "Task.h"
#include <iostream>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

Task::Task(const char* name, unsigned int priority) :
	m_Next(nullptr), 
	m_Prev(nullptr),
	m_Name(name),
	m_Priority(priority),
	m_IsKill(false)
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

bool Task::GetKill()
{
	return m_IsKill;
}

const char *Task::GetName()
{
	return m_Name;
}

void Task::Update()
{
	std::cout << m_Name << "基底タスクUpdateが呼ばれました" << "\n";
}

void Task::DrawName()
{
	std::cout << "Task_Name : " << m_Name << "\n";
}

Task::~Task()
{
}

void Task::SetPriority(unsigned int priority)
{
	m_Priority = priority;
}

unsigned int Task::GetPriority()
{
	return m_Priority;
}

void Task::SetPrev(Task *prevPointer)
{
	m_Prev = prevPointer;
}

void Task::SetNext(Task *nextPointer)
{
	m_Next = nextPointer;
}

Task* Task::GetPrev()
{
	return m_Prev;
}

Task* Task::GetNext()
{
	return m_Next;
}

TaskManager* TaskManager::m_Instance = nullptr;

TaskManager::TaskManager() :
	m_Begin(nullptr)
{
}

TaskManager::~TaskManager()
{
}

inline TaskManager* TaskManager::GetInstance()
{
	if (m_Instance == nullptr) m_Instance = new TaskManager();
	return m_Instance;
}

void TaskManager::ClearInstance()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
	}
	m_Instance = nullptr;
}

void TaskManager::Add(Task* taskPointer)
{
	//先頭にタスクがなかったらそのまま追加
	if (GetInstance()->m_Begin == nullptr)
	{
		GetInstance()->m_Begin = taskPointer;
	}
	else
	{
		Task* task = GetInstance()->m_Begin;
		Task* prev = nullptr;

		//タスクリストが終わるまで
		while (task != nullptr)
		{
			//追加するタスクの優先順位が上ならタスクリストの先頭に追加
			if (taskPointer->GetPriority() < task->GetPriority())
			{
				//タスクリストの最初ならそのまま追加
				if (task == GetInstance()->m_Begin) GetInstance()->m_Begin = taskPointer;

				//タスクリストの入れ替え、追加するタスクを前へ
				taskPointer->SetNext(task);
				task->SetPrev(taskPointer);
				break;
			}

			//追加するタスクの優先順位が下なら次のタスクに進む
			prev = task;
			task = task->GetNext();
		}

		//追加するタスクが最後まで行ったらタスクを最後に追加
		if (prev != nullptr)
		{
			//最後のタスクに追加処理
			prev->SetNext(taskPointer);
			taskPointer->SetPrev(prev);
		}
	}
}

void TaskManager::PartClear(const char* taskName)
{
	Task *task = GetInstance()->m_Begin;

	while (task != nullptr)
	{
		//指定したタスクを消去
		if (task->GetName() == taskName)
		{
			Task *next = task->GetNext(); //次のタスクを格納
			Task *prev = task->GetPrev(); //後ろのタスクを格納

			//後ろにあるタスクと前にあるタスクを格納
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//現在のタスクを削除
			delete task;

			//次のタスクに移動
			task = next;
		}
		else
		{
			task = task->GetNext();
		}
	}
}

void TaskManager::AllClear()
{
	Task *task = GetInstance()->m_Begin;

	//タスクリストが終わるまでループ
	while (task != nullptr)
	{
		Task *next = task->GetNext(); //次のタスクを格納
		Task *prev = task->GetPrev(); //後ろのタスクを格納

		//後ろにあるタスクと前にあるタスクを格納
		if (next != nullptr) next->SetPrev(prev);
		if (prev != nullptr) prev->SetNext(next);
		else GetInstance()->m_Begin = next;

		//現在のタスクを削除
		delete task;

		//次のタスクに移動
		task = next;
	}
}

void TaskManager::Update()
{
	Task *task = GetInstance()->m_Begin;
	
	while (task != nullptr)
	{
		if (task->GetKill())
		{
			Task *next = task->GetNext(); //次のタスクを格納
			Task *prev = task->GetPrev(); //後ろのタスクを格納

			//後ろにあるタスクと前にあるタスクを格納
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//現在のタスクを削除
			delete task;

			//次のタスクに移動
			task = next;
		}
		else
		{
			task->Update();
			task = task->GetNext();
		}
	}
}

void TaskManager::DrawName()
{
	Task *task = GetInstance()->m_Begin;

	while (task != nullptr)
	{
		if (task->GetKill())
		{
			Task *next = task->GetNext(); //次のタスクを格納
			Task *prev = task->GetPrev(); //後ろのタスクを格納

			//後ろにあるタスクと前にあるタスクを格納
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//現在のタスクを削除
			delete task;

			//次のタスクに移動
			task = next;
		}
		else
		{
			task->DrawName();
			task = task->GetNext();
		}
	}
}