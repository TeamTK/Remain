#ifndef _FUNCTION_TASK_
#define _FUNCTION_TASK_

#include <functional>
#include <map>

#define REGIST_FUNC_TASK(member_Func) std::bind(&member_Func, this)

struct FuncTaskInfo
{
	bool m_IsSleep;
	std::function<void()> func;

	FuncTaskInfo() {};
	FuncTaskInfo(std::function<void()> func) :
		m_IsSleep(false),
		func(func) {}
	~FuncTaskInfo() {};
};

class FunctionTask
{
public:
	FunctionTask();
	~FunctionTask();
	void Regist(const char *name, std::function<void()> func);
	void Sleep(const char *name);
	void Awake(const char *name);
	void Update();
	void OperationDraw();
	void RegistDraw();

private:
	std::map<const char*, FuncTaskInfo> m_funcMap;

};

#endif