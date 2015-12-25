#ifndef _RENDER_H_
#define _RENDER_H_

#include "Task.h"
#include <functional>

#define REGIST_RENDER_FUNC(member_Func) std::bind(&member_Func, this)

class RenderManager;

class RenderTask
{
	friend RenderManager;

public:
	RenderTask();
	~RenderTask();
	bool GetSleep() const;
	unsigned int GetPriority() const;
	void Regist(unsigned int priority, std::function<void()> func);
	void Unregist();
	void SetPriority(unsigned int priority);
	void Sleep();
	void Awake();
	void Render() const;
	void DebugDraw() const;

private:
	unsigned int m_Priority;
	bool m_IsSeep;
	std::function<void()> m_Func;
};

class RenderManager
{
	friend RenderTask;

public:
	~RenderManager();
	static void AllClear();
	static void DebugDraw();
	static void Render();

private:
	RenderManager();
	static RenderManager *GetInstance();
	static void Regist(RenderTask *renderTask);
	static void Unregist(RenderTask *renderTask);
	static void Sort();

private:
	class RenderTaskPimpl;
	RenderTaskPimpl *m_pRenderTaskPimpl;
};

#endif