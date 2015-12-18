#ifndef _TRACEROUTE_MANAGER_H_
#define _TRACEROUTE_MANAGER_H_

class StaticMesh;

class TracerouteManager
{
public:
	~TracerouteManager();
	void Add(StaticMesh &staticMesh);
	void Clear();
	void AlClear();
	void DebugRender();

private:
	TracerouteManager();

};

#endif