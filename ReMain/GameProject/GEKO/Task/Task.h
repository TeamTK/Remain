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
	bool GetKill();
	const char *GetName();
	virtual void Update();
	void DrawName();

protected:
	virtual ~Task();
	void SetPriority(unsigned int priority);
	unsigned int GetPriority();

private:
	void SetPrev(Task *prevPointer);
	void SetNext(Task *nextPointer);
	Task* GetPrev();
	Task* GetNext();

private:
	Task *m_Prev;
	Task *m_Next;
	const char* m_Name;
	unsigned int m_Priority;
	bool m_IsKill;
};

class TaskManager
{
public:
	static void ClearInstance();
	static void Add(Task* taskPointer);
	static void PartClear(const char* taskName);
	static void AllClear();
	static void Update();
	static void DrawName();

private:
	inline static TaskManager* GetInstance();
	TaskManager();
	~TaskManager();
	static TaskManager *m_Instance;
	Task* m_Begin;
};

#endif