#ifndef _COMMAND_TASK_H_
#define _COMMAND_TASK_H_

class CommandTask
{
public:
	CommandTask(const char* name);
	virtual ~CommandTask();
	virtual void Update();
	void DebugOperation();

private:
	const char *m_Name;

};

class CommandObject
{
public:
	CommandObject();
	~CommandObject();
	void Regist(CommandTask *pCommand);
	void Update();
	void DebugOperation();

private:
	CommandTask *m_pCommandTask;

};

#endif