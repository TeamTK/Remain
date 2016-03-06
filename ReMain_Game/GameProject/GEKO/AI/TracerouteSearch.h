#ifndef _TRACEROUTE_SEARCH_H_
#define _TRACEROUTE_SEARCH_H_

#include "TracerouteInfo.h"

class TracerouteManager;

class TracerouteSearch
{
	friend TracerouteManager;

public:
	TracerouteSearch();
	~TracerouteSearch();
	bool GetIsGoal() const;
	bool GetIsGoalExist() const;
	const Vector3D *GetTargetDirection() const;
	void SetPosition(const Vector3D *pos);
	void SetTopography(const char* name);
	void SetTargetName(std::string name);
	void StartSerch();
	void StopSerch();
	void StartMove();
	void StopMove();
	void DebugMigrationPath();
	void DebugCenterPath();

private:
	PathMoveInfo m_PathMoveInfo;
	PathPlanning m_PathPlanning;
	bool m_IsSerch;
	bool m_IsSerchMove;
	bool m_IsGoal;
	const char* m_NameTopography;
	std::string m_NameTarget;
};

#endif