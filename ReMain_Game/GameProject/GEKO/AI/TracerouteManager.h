#ifndef _TRACEROUTE_MANAGER_H_
#define _TRACEROUTE_MANAGER_H_

class StaticMesh;
class Vector3D;

class TracerouteManager
{
public:
	~TracerouteManager();
	static void Add(StaticMesh *staticMesh, const char* name);
	static void Clear(const char* name);
	static void AlClear();
	static void DebugRender(const char* name);
	static void DebugMigrationPath(const char* name);
	static void ProcessBuilding(const char* name);
	static void SetupPathPlanning(const char* name, const Vector3D &startPos, const Vector3D &goalPos);

private:
	TracerouteManager();
	static TracerouteManager* GetInstance();
	int CheckOnPolyIndex(StaticMesh *pStaticMesh, const Vector3D &pos);

private:
	class TraceroutePimpl;
	TraceroutePimpl *m_pTraceroutePimpl;
};

#endif