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
	std::cout << m_Name << "���^�X�NUpdate���Ă΂�܂���" << "\n";
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
	//�擪�Ƀ^�X�N���Ȃ������炻�̂܂ܒǉ�
	if (GetInstance()->m_Begin == nullptr)
	{
		GetInstance()->m_Begin = taskPointer;
	}
	else
	{
		Task* task = GetInstance()->m_Begin;
		Task* prev = nullptr;

		//�^�X�N���X�g���I���܂�
		while (task != nullptr)
		{
			//�ǉ�����^�X�N�̗D�揇�ʂ���Ȃ�^�X�N���X�g�̐擪�ɒǉ�
			if (taskPointer->GetPriority() < task->GetPriority())
			{
				//�^�X�N���X�g�̍ŏ��Ȃ炻�̂܂ܒǉ�
				if (task == GetInstance()->m_Begin) GetInstance()->m_Begin = taskPointer;

				//�^�X�N���X�g�̓���ւ��A�ǉ�����^�X�N��O��
				taskPointer->SetNext(task);
				task->SetPrev(taskPointer);
				break;
			}

			//�ǉ�����^�X�N�̗D�揇�ʂ����Ȃ玟�̃^�X�N�ɐi��
			prev = task;
			task = task->GetNext();
		}

		//�ǉ�����^�X�N���Ō�܂ōs������^�X�N���Ō�ɒǉ�
		if (prev != nullptr)
		{
			//�Ō�̃^�X�N�ɒǉ�����
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
		//�w�肵���^�X�N������
		if (task->GetName() == taskName)
		{
			Task *next = task->GetNext(); //���̃^�X�N���i�[
			Task *prev = task->GetPrev(); //���̃^�X�N���i�[

			//���ɂ���^�X�N�ƑO�ɂ���^�X�N���i�[
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//���݂̃^�X�N���폜
			delete task;

			//���̃^�X�N�Ɉړ�
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

	//�^�X�N���X�g���I���܂Ń��[�v
	while (task != nullptr)
	{
		Task *next = task->GetNext(); //���̃^�X�N���i�[
		Task *prev = task->GetPrev(); //���̃^�X�N���i�[

		//���ɂ���^�X�N�ƑO�ɂ���^�X�N���i�[
		if (next != nullptr) next->SetPrev(prev);
		if (prev != nullptr) prev->SetNext(next);
		else GetInstance()->m_Begin = next;

		//���݂̃^�X�N���폜
		delete task;

		//���̃^�X�N�Ɉړ�
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
			Task *next = task->GetNext(); //���̃^�X�N���i�[
			Task *prev = task->GetPrev(); //���̃^�X�N���i�[

			//���ɂ���^�X�N�ƑO�ɂ���^�X�N���i�[
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//���݂̃^�X�N���폜
			delete task;

			//���̃^�X�N�Ɉړ�
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
			Task *next = task->GetNext(); //���̃^�X�N���i�[
			Task *prev = task->GetPrev(); //���̃^�X�N���i�[

			//���ɂ���^�X�N�ƑO�ɂ���^�X�N���i�[
			if (next != nullptr) next->SetPrev(prev);
			if (prev != nullptr) prev->SetNext(next);
			else GetInstance()->m_Begin = next;

			//���݂̃^�X�N���폜
			delete task;

			//���̃^�X�N�Ɉړ�
			task = next;
		}
		else
		{
			task->DrawName();
			task = task->GetNext();
		}
	}
}