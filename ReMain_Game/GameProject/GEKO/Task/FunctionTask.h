#ifndef _FUNCTION_TASK_
#define _FUNCTION_TASK_

#include <functional>
#include <map>

#define REGIST_FUNC_TASK(member_Func) std::bind(&member_Func, this)

struct FuncTaskInfo
{
	bool m_IsRunning;
	std::function<void()> func;

	FuncTaskInfo() {};
	FuncTaskInfo(std::function<void()> func) :
		m_IsRunning(true),
		func(func) {}
	~FuncTaskInfo() {};
};

class FunctionTask
{
public:
	FunctionTask();
	~FunctionTask();
	bool Running(const char *name);
	void Regist(const char *name, std::function<void()> func);
	void Stop(const char *name);
	void AllStop();
	void Start(const char *name);
	void AllStart();
	void Update();
	void RunningDraw();
	void RegistDraw();

private:
	std::map<const char*, FuncTaskInfo> m_funcMap;

};

#endif