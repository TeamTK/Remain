#ifndef _TRACEROUTE_OBJECT_H_
#define _TRACEROUTE_OBJECT_H_

class StaticMesh;

class TracerouteObject
{
public:
	TracerouteObject();
	~TracerouteObject();
	void SetStaticMesh(StaticMesh *staticMesh);

private:
	StaticMesh *m_pStaticMesh;
};

#endif