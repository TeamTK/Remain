#include "TracerouteSearch.h"
#include "TracerouteManager.h"

TracerouteSearch::TracerouteSearch() :
	m_IsSerch(true),
	m_IsSerchMove(true),
	m_IsGoal(false),
	m_NameTopography(nullptr),
	m_NameTarget(nullptr)
{
	TracerouteManager::AddSearch(this);
}

TracerouteSearch::~TracerouteSearch()
{
	//�o�H�T�����Z�o���ꂽ���폜
	m_PathPlanning.pActiveFirstUnit = nullptr;
	m_PathPlanning.pGoalUnit = nullptr;
	m_PathPlanning.pStartUnit = nullptr;
	if(m_PathPlanning.pUnitArray != nullptr) delete[] m_PathPlanning.pUnitArray;

	TracerouteManager::ClearSearch(this);
}

bool TracerouteSearch::GetIsGoal() const
{
	return m_IsGoal;
}

bool TracerouteSearch::GetIsGoalExist() const
{
	if (m_PathPlanning.pGoalUnit) return true;
	return false;
}

const Vector3D* TracerouteSearch::GetTargetDirection() const
{
	return &m_PathMoveInfo.MoveDirection;
}

void TracerouteSearch::SetPosition(const Vector3D *pos)
{
	m_PathMoveInfo.pNowPosition = pos;
}

void TracerouteSearch::SetTopography(const char* name)
{
	m_NameTopography = name;
}

void TracerouteSearch::SetTargetName(const char* name)
{
	m_NameTarget = name;
}

void TracerouteSearch::StartSerch()
{
	m_IsSerch = true;
}

void TracerouteSearch::StopSerch()
{
	m_IsSerch = false;
}

void TracerouteSearch::StartMove()
{
	m_IsSerchMove = true;
}

void TracerouteSearch::StopMove()
{
	m_IsSerchMove = false;
}

void TracerouteSearch::DebugMigrationPath()
{
	TracerouteManager::DebugMigrationPath(m_NameTopography, this);
}

void TracerouteSearch::DebugCenterPath()
{
	TracerouteManager::DebugCenterPath(m_NameTopography, this);
}