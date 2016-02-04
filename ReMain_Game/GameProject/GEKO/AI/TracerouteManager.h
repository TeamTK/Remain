#ifndef _TRACEROUTE_MANAGER_H_
#define _TRACEROUTE_MANAGER_H_

class StaticMesh;
class Vector3D;
class TracerouteSearch;
class TracerouteTarget;

struct PathPlanningUnit;
struct TracerouteInfo;
struct PathPlanning;

class TracerouteManager
{
	friend TracerouteSearch;
	friend TracerouteTarget;

public:
	~TracerouteManager();
	static void AllClear();
	static void RegistTopography(StaticMesh *pStaticMesh, const char* name, bool isGPU);
	static void ClearTopography(const char* name);
	static void DebugRender(const char* name);
	static void DebugTopography(const char* name);
	static void ProcessBuilding(const char* name, bool isGPU); //地形の経路情報構築（地形が回転・拡大・平行移動するたびに呼ぶ事）
	static void Update();

private:
	TracerouteManager();
	static TracerouteManager* GetInstance();
	//bool CheckPolyMove(const char* TopographyName, const Vector3D& startPos, const Vector3D& targetPos);
	//bool CheckPolyMoveWidth(const char* TopographyName, const Vector3D& startPos, const Vector3D& targetPos, float width);
	int CheckOnPolyIndex(TracerouteInfo *pInfo, const Vector3D &pos); //探索用ポリゴンのどこのポリゴンにいるか判定
	static void SetupPathPlanning(const char* name, TracerouteSearch *pSearch, const Vector3D &goalPos); //経路探索開始
	static void PathPlanningFunc(PathPlanningUnit *pUnit, PathPlanningUnit *pUintPrev, int idx, TracerouteInfo *pInfo, PathPlanning *pPathPlanning);
	static void AddSearch(TracerouteSearch *pSearch);
	static void AddTarget(TracerouteTarget *pTarget);
	static void ClearSearch(TracerouteSearch *pSearch);
	static void ClearTarget(TracerouteTarget *pTarget);
	static void DebugMigrationPath(const char *name, TracerouteSearch *pSearche);
	static void DebugCenterPath(const char *name, TracerouteSearch *pSearch);

private:
	class TraceroutePimpl;
	TraceroutePimpl *m_pTraceroutePimpl;
};

#endif