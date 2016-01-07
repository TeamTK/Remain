#ifndef _TASK_H_
#define _TASK_H_

class TaskManager;

class Task
{
	friend TaskManager;

public:
	Task(const char* name, unsigned int priority);
	Task();
	void SetKill();
	void SetPriority(unsigned int priority);
	void Stop();
	void Start();
	bool Running() const;
	bool GetKill() const;
	unsigned int GetPriority() const;
	const char *GetName() const;
	virtual void Update();
	void DrawName() const;

protected:
	virtual ~Task();

private:
	const char* m_Name;
	unsigned int m_Priority;
	bool m_IsKill;
	bool m_IsSleep;
};

class TaskManager
{
	friend Task;

public:
	static void Kill(const char* taskName);
	static void AllKill();
	static void PartClear(const char* taskName);
	static void AllClear();
	static void Update();
	static void DrawName();
	static void Stop(const char* taskName);
	static void AllStop();
	static void Start(const char* taskName);
	static void AllStart();

protected:
	static void Add(Task* taskPointer);
	static void Sort();

private:
	inline static TaskManager* GetInstance();
	TaskManager();
	~TaskManager();

private:
	class TaskPimpl;
	TaskPimpl *m_pTaskPimpl;
};

#endif