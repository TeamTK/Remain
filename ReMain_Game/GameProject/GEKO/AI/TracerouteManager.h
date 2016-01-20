#ifndef _TRACEROUTE_MANAGER_H_
#define _TRACEROUTE_MANAGER_H_

class StaticMesh;

class TracerouteManager
{
public:
	~TracerouteManager();
	static void Add(StaticMesh *staticMesh, const char* name);
	static void Clear(const char* name);
	static void AlClear();
	static void DebugRender(const char* name);
	static void ProcessBuilding(const char* name);

private:
	TracerouteManager();
	static TracerouteManager* GetInstance();

private:
	class TraceroutePimpl;
	TraceroutePimpl *m_pTraceroutePimpl;
};

#endif