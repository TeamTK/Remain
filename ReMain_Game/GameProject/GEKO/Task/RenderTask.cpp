#include "RenderTask.h"
#include "assert.h"
#include <Windows.h>
#include <iostream>
#include <list>

RenderTask::RenderTask() :
	m_Priority(0),
	m_IsSeep(false),
	m_Func(nullptr)
{
}

RenderTask::~RenderTask()
{
	if (m_Func != nullptr)
	{
		RenderManager::Unregist(this);
	}
}

bool RenderTask::GetSleep() const
{
	return m_IsSeep;
}

unsigned int RenderTask::GetPriority() const
{
	return m_Priority;
}

void RenderTask::Regist(unsigned int priority, std::function<void()> func)
{
	assert(m_Func == nullptr && "RenderTask‚Í“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_Priority = priority;
	m_Func = func;
	RenderManager::Regist(this);
}

void RenderTask::Unregist()
{
	m_Priority = 0;
	m_Func = nullptr;
	RenderManager::Unregist(this);
}

void RenderTask::SetPriority(unsigned int priority)
{
	m_Priority = priority;
	RenderManager::Sort();
}

void RenderTask::Sleep()
{
	m_IsSeep = true;
}

void RenderTask::Awake()
{
	m_IsSeep = false;
}

void RenderTask::Render() const
{
	m_Func();
}

void RenderTask::DebugDraw() const
{
	std::cout << "Priority : " << m_Priority << '\n';
}

class RenderManager::RenderTaskPimpl
{
public:
	~RenderTaskPimpl()
	{
		RenderList.clear();
	}

public:
	std::list<RenderTask*> RenderList;
};

RenderManager::RenderManager()
{
	m_pRenderTaskPimpl = new RenderTaskPimpl();
}

RenderManager::~RenderManager()
{
	delete m_pRenderTaskPimpl;
}

void RenderManager::AllClear()
{
	RenderManager *temp = GetInstance();
	auto it = temp->m_pRenderTaskPimpl->RenderList.begin();
	auto itEnd = temp->m_pRenderTaskPimpl->RenderList.end();
	for (; it != itEnd;)
	{
		it = temp->m_pRenderTaskPimpl->RenderList.erase(it);
	}
	temp->m_pRenderTaskPimpl->RenderList.clear();
	OutputDebugString(TEXT("RenderTaskƒŠƒXƒg‚Ìíœ‚ðŠ®—¹‚µ‚Ü‚µ‚½\n"));
}

void RenderManager::DebugDraw()
{
	static unsigned int cnt = 0;
	RenderManager *temp = GetInstance();
	auto it = temp->m_pRenderTaskPimpl->RenderList.begin();
	auto itEnd = temp->m_pRenderTaskPimpl->RenderList.end();
	for (; it != itEnd; it++)
	{
		std::cout << "RenderTask " << cnt << " : ";
		cnt++;
		(*it)->DebugDraw();
	}
	cnt = 0;
}

void RenderManager::Render()
{
	RenderManager *temp = GetInstance();
	auto it = temp->m_pRenderTaskPimpl->RenderList.begin();
	auto itEnd = temp->m_pRenderTaskPimpl->RenderList.end();
	for (; it != itEnd; it++)
	{
		(*it)->Render();
	}
}

RenderManager *RenderManager::GetInstance()
{
	static RenderManager renderManager;
	return &renderManager;
}

bool less(const RenderTask *data1, const RenderTask *data2)
{
	return data1->GetPriority() < data2->GetPriority();
};

void RenderManager::Regist(RenderTask *renderTask)
{
	RenderManager *temp = GetInstance();
	temp->m_pRenderTaskPimpl->RenderList.emplace_back(renderTask);
	temp->m_pRenderTaskPimpl->RenderList.sort(&less);
}

void RenderManager::Unregist(RenderTask *pRenderTask)
{
	RenderManager *temp = GetInstance();
	auto it = temp->m_pRenderTaskPimpl->RenderList.begin();
	auto itEnd = temp->m_pRenderTaskPimpl->RenderList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pRenderTask)
		{
			it = temp->m_pRenderTaskPimpl->RenderList.erase(it);
			break;
		}
	}
}

void RenderManager::Sort()
{
	GetInstance()->m_pRenderTaskPimpl->RenderList.sort(&less);
}