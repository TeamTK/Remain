#include "TracerouteManager.h"
#include "TracerouteInfo.h"
#include "TracerouteSearch.h"
#include "TracerouteTarget.h"
#include "..\Mesh\StaticMesh.h"
#include "..\Figure\Fiqure.h"
#include "..\System\Collision.h"
#include "..\Collider\BaseCollider.h"
#include <map>
#include <list>

class TracerouteManager::TraceroutePimpl
{
public:
	std::map<const char*, TracerouteInfo> tracerouteInfo; //�o�H�T�������邽�߂̏��
	std::list<TracerouteSearch*> pathPlanning; //�o�H�T�����Z�o���ꂽ���
	std::list<TracerouteTarget*> targetInfo; //�o�H�T���Ώۂ̏��
};

TracerouteManager::TracerouteManager()
{
	m_pTraceroutePimpl = new TraceroutePimpl;
}

TracerouteManager::~TracerouteManager()
{
	AllClear();
	delete m_pTraceroutePimpl;
}

TracerouteManager* TracerouteManager::GetInstance()
{
	static TracerouteManager tracerouteManager;
	return &tracerouteManager;
}

void TracerouteManager::RegistTopography(StaticMesh *pStaticMesh, const char* name, bool isGPU)
{
	TracerouteManager *pInfo = GetInstance();
	pInfo->m_pTraceroutePimpl->tracerouteInfo[name].pStaticMesh = pStaticMesh;
	ProcessBuilding(name, isGPU);
}

void TracerouteManager::ClearTopography(const char* name)
{
	auto *p = &GetInstance()->m_pTraceroutePimpl->tracerouteInfo;
	auto it = p->begin();
	auto itEnd = p->end();

	for (; it != itEnd; it++)
	{
		if (it->first == name)
		{
			if (it->second.pPoryLinkInfo != nullptr) delete[] it->second.pPoryLinkInfo;
			if (it->second.pNormal != nullptr) delete[] it->second.pNormal;
			//if (it->second.pCopmute != nullptr) delete it->second.pCopmute;
			it->second.copmute.Release();
			it->second.pStaticMesh = nullptr;
			it = p->erase(it);
			break;
		}
	}
}

void TracerouteManager::AllClear()
{
	//�o�H�T�������邽�߂̏��폜
	for (auto& i : GetInstance()->m_pTraceroutePimpl->tracerouteInfo)
	{
		if (i.second.pPoryLinkInfo != nullptr) delete[] i.second.pPoryLinkInfo;
		if (i.second.pNormal != nullptr) delete[] i.second.pNormal;
		//if (i.second.pCopmute != nullptr) delete i.second.pCopmute;
		i.second.copmute.Release();
		i.second.pStaticMesh = nullptr;
	}

	GetInstance()->m_pTraceroutePimpl->tracerouteInfo.clear();
	GetInstance()->m_pTraceroutePimpl->pathPlanning.clear();
	GetInstance()->m_pTraceroutePimpl->targetInfo.clear();
}

void TracerouteManager::DebugRender(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	assert(pInfo->pStaticMesh && "�o�H�T���p�̒n�`�f�[�^������܂���");

	//�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;
	const int polyNum = pMesh->GetFaceAllNum();
	Vector3D pos[3];

	int num = 0;
	int num2 = 0;
	int num3 = 0;

	for (int i = 0; i < polyNum; i++)
	{
		num = pPoryLinkInfo[i].linkPolyIndex[0];
		num2 = pPoryLinkInfo[i].linkPolyIndex[1];
		num3 = pPoryLinkInfo[i].linkPolyIndex[2];

		pos[0] = pPoryLinkInfo[num].centerPosition;
		pos[1] = pPoryLinkInfo[num2].centerPosition;
		pos[2] = pPoryLinkInfo[num3].centerPosition;

		Vector3D center = pPoryLinkInfo[i].centerPosition;

		if (num != -1) Fiqure::RenderLine3D(center, pos[0], Vector3D(0.0f, 1.0f, 0.0f));
		if (num2 != -1) Fiqure::RenderLine3D(center, pos[1], Vector3D(0.0f, 1.0f, 0.0f));
		if (num3 != -1) Fiqure::RenderLine3D(center, pos[2], Vector3D(0.0f, 1.0f, 0.0f));
	}
}

void TracerouteManager::DebugTopography(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	assert(pInfo->pStaticMesh && "�o�H�T���p�̒n�`�f�[�^������܂���");

	//�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	const auto *index = pMesh->GetIndex();
	const auto *vertex = pMesh->GetVertex();
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;
	const int polyNum = pMesh->GetFaceAllNum();

	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Vector3D pos[3];

	for (int i = 0; i < polyNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pos[0] = vertex[index[pPoryLinkInfo[i].linkPolyIndex[j]].vertexIndex[0]].pos * mat;
			pos[1] = vertex[index[pPoryLinkInfo[i].linkPolyIndex[j]].vertexIndex[1]].pos * mat;
			pos[2] = vertex[index[pPoryLinkInfo[i].linkPolyIndex[j]].vertexIndex[2]].pos * mat;

			if (pPoryLinkInfo[i].linkPolyIndex[j] != -1) Fiqure::RenderLine3D(pos[0], pos[1], Vector3D(1.0f, 1.0f, 0.0f));
			if (pPoryLinkInfo[i].linkPolyIndex[j] != -1) Fiqure::RenderLine3D(pos[1], pos[2], Vector3D(1.0f, 1.0f, 0.0f));
			if (pPoryLinkInfo[i].linkPolyIndex[j] != -1) Fiqure::RenderLine3D(pos[2], pos[0], Vector3D(1.0f, 1.0f, 0.0f));
		}
	}
}

void TracerouteManager::ProcessBuilding(const char* name, bool isGPU)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	assert(pInfo->pStaticMesh && "�o�H�T���p�̒n�`�f�[�^������܂���");
	pInfo->isGPU = isGPU;

	//�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	const VertexInfo *pVertex = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();
	const int polyNum = pMesh->GetFaceAllNum();
	Vector3D pos[3];

	//���b�V���̍s��
	pMesh->WorldMatrixBuilding();
	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;

	//�אڂ���|���S�����
	if (pInfo->pPoryLinkInfo != nullptr)
	{
		delete[] pInfo->pPoryLinkInfo;
		pInfo->pPoryLinkInfo = nullptr;
	}
	pInfo->pPoryLinkInfo = new PolyLinkInfo[polyNum];
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;

	//�o�H�T���n�`�̓����蔻��Ɏg���@��
	if (pInfo->pNormal != nullptr)
	{
		delete[] pInfo->pNormal;
		pInfo->pNormal = nullptr;
	}
	pInfo->pNormal = new Vector3D[polyNum];

	//�S�Ẵ|���S���̒��S���W�擾
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = pVertex[index[i].vertexIndex[0]].pos;
		pos[1] = pVertex[index[i].vertexIndex[1]].pos;
		pos[2] = pVertex[index[i].vertexIndex[2]].pos;

		//�O�p�`�̒��S�_
		pPoryLinkInfo[i].centerPosition = (pos[0] + pos[1] + pos[2]) * mat * 0.33333333333f;

		//�����蔻��p�̖@��
		pInfo->pNormal[i] = Vector3D::Cross(pos[2] - pos[0], pos[1] - pos[0]).GetNormalize();
	}

	//GPU�Œn�`�̓����蔻��\�z
	if (pInfo->isGPU)
	{
		//pInfo->copmute = new LineTriangleCopmute;
		pInfo->copmute.Init(*pMesh, pInfo->pNormal);
	}

	bool isNoNeighborhood = false;
	bool isIndex[3] = {false, false, false};

	//�|���S�����m�̗אڏ��̍\�z
	for (int i = 0; i < polyNum; i++)
	{
		//�אڂ���|���S����T��
		for (int j = 0; j < polyNum; j++)
		{
			if (i == j) continue; //�����̃|���S���Ȃ��΂�

			for (int k = 0; k < 3; k++)
			{
				//���_�ԍ�0��1�ŕӂ̂Ɨאڂ�����אڂ������_�ԍ��Ɨאڃ|���S���̒��S���W�܂ł̋����擾
				isNoNeighborhood = (pPoryLinkInfo[i].linkPolyIndex[k] == -1);
				isIndex[0] = (pVertex[index[i].vertexIndex[k]].pos == pVertex[index[j].vertexIndex[0]].pos && pVertex[index[i].vertexIndex[(k + 1) % 3]].pos == pVertex[index[j].vertexIndex[2]].pos);
				isIndex[1] = (pVertex[index[i].vertexIndex[k]].pos == pVertex[index[j].vertexIndex[1]].pos && pVertex[index[i].vertexIndex[(k + 1) % 3]].pos == pVertex[index[j].vertexIndex[0]].pos);
				isIndex[2] = (pVertex[index[i].vertexIndex[k]].pos == pVertex[index[j].vertexIndex[2]].pos && pVertex[index[i].vertexIndex[(k + 1) % 3]].pos == pVertex[index[j].vertexIndex[1]].pos);

				if (isNoNeighborhood && (isIndex[0] || isIndex[1] || isIndex[2]))
				{
					pPoryLinkInfo[i].linkPolyIndex[k] = j;
					pPoryLinkInfo[i].linkPolyDistance[k] = (pPoryLinkInfo[j].centerPosition - pPoryLinkInfo[i].centerPosition).Length();
					break;
				}
			}
		}
	}
}

void TracerouteManager::Update()
{
	TraceroutePimpl *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pPathPlanning = &pTraceroute->pathPlanning;
	auto *pTarget = &pTraceroute->targetInfo;
	PathMoveInfo *pMove;
	PathPlanningUnit *pUnit;

	//�w�肳�ꂽ�o�H�T��������
	for (auto& i : *pPathPlanning)
	{
		if (!i->m_IsSerch) continue;

		for (auto& j : *pTarget)
		{
			//�T�������T���Ώۂ̖��O�ƈ�v������o�H�T���J�n
			if (i->m_NameTarget == j->m_Name)
			{
				//if(i->m_PathPlanning.pGoalUnit == i->m_PathPlanning.pPrevGoalUnit) break;

				SetupPathPlanning(i, *j->m_pPosition);
				//i.first->m_IsSerch = false;
				i->m_PathMoveInfo.pNowPathPlanningUnit = i->m_PathPlanning.pStartUnit;
				i->m_PathMoveInfo.pTargetPathPlanningUnit = i->m_PathPlanning.pStartUnit;
				break;
			}
		}
	}

	//���߂�ꂽ�o�H��i��
	for (auto& i : *pPathPlanning)
	{
		if (!i->m_IsSerchMove) continue;
		if (i->m_PathPlanning.pGoalUnit == nullptr) continue;
		if (i->m_PathPlanning.pStartUnit == nullptr) continue;

		pMove = &i->m_PathMoveInfo;
		
		//�S�[��������������[���ɂ���
		if (pMove->pNowPathPlanningUnit == i->m_PathPlanning.pGoalUnit)
		{
			i->m_IsGoal = true;
			//pMove->MoveDirection = Vector3D(0.0f, 0.0f, 0.0f);
			continue;
		}
		else
		{
			i->m_IsGoal = false;
		}

		auto *pInfo = &pTraceroute->tracerouteInfo[i->m_NameTopography];

		// ���ݏ���Ă���|���S�����ړ����Ԓn�_�̃|���S���̏ꍇ�͎��̒��Ԓn�_�����肷�鏈�����s��
		if (pMove->pNowPathPlanningUnit == pMove->pTargetPathPlanningUnit)
		{
			pUnit = &i->m_PathPlanning.pUnitArray[pMove->pTargetPathPlanningUnit->NextPolyIndex];
			pMove->pTargetPathPlanningUnit = pUnit; //���̌o�H�ɕύX
		}
		else
		{
			// ���݂̍��W�ŏ���Ă���|���S������������
			pMove->nowPolyIndex = GetInstance()->CheckOnPolyIndex(pInfo, *pMove->pNowPosition);

			// ���݂̍��W�ŏ���Ă���|���S���̌o�H�T�����̃������A�h���X��������
			pMove->pNowPathPlanningUnit = &i->m_PathPlanning.pUnitArray[pMove->nowPolyIndex];
		}
		
		//���̌o�H�̒��S���W�ɐi�ޕ��p�擾
		Vector3D center = pInfo->pPoryLinkInfo[pMove->pTargetPathPlanningUnit->polyIndex].centerPosition;
		pMove->MoveDirection = (center - *pMove->pNowPosition);
		pMove->MoveDirection.SetNormalize();
	}
}

/*
bool TracerouteManager::CheckPolyMove(const char* TopographyName, const Vector3D& startPos, const Vector3D& targetPos)
{
	int startPory = -1;
	int targetPoty = -1;
	PolyLinkInfo *pStartLink = nullptr;
	PolyLinkInfo *pTargetLink = nullptr;
	Vector3D pos[3];
	int checkPory[3];
	int checkPoryPrev[3];
	int checkPoryNum;
	int NextCheckPoly[3];
	int NextCheckPolyPrev[3];
	int NextCheckPolyNum;

	// �J�n���W�ƖڕW���W�� y���W�l�� 0.0f �ɂ��āA���ʏ�̔���ɂ���
	//startPos.y = 0.0f;
	//targetPos.y = 0.0f;
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *path = &pTraceroute->tracerouteInfo[TopographyName];
	
	//�J�n���W�ƖڕW���W�̒���A�Ⴕ���͒����ɑ��݂���|���S������������
	startPory = CheckOnPolyIndex(path->pStaticMesh, path->pNormal, startPos);
	targetPoty = CheckOnPolyIndex(path->pStaticMesh, path->pNormal, targetPos);

	if (startPory == -1 || targetPoty == -1) return false;
}

bool TracerouteManager::CheckPolyMoveWidth(const char* TopographyName, const Vector3D& startPos, const Vector3D& targetPos, float width)
{
	Vector3D direcion;
	Vector3D sideDirecion;
	Vector3D temp;

	//�ŏ��ɊJ�n���W����ڕW���W�ɒ����ňړ��ł��邩�m�F
	if (!CheckPolyMove(TopographyName, startPos, targetPos)) return false;

	direcion = targetPos - startPos;
	direcion.y = 0.0f;

	//�J�n���W����ڕW���W�Ɍ������x�N�g���ɒ��p�Ȑ��K���x�N�g�����Z�o
	sideDirecion = Vector3D::Cross(direcion, Vector3D(0.0f, 1.0f, 0.0f)).GetNormalize();

	//�J�n���W�ƖڕW���W�� Width / 2.0f ���������������ɂ��炵�āA�ēx�����I�Ɉړ��ł��邩�ǂ������m�F
	temp = sideDirecion * (width / 2.0f);
	if (!CheckPolyMove(TopographyName, startPos + temp, targetPos + temp)) return false;

	temp = sideDirecion * (-width / 2.0f);
	if (!CheckPolyMove(TopographyName, startPos + temp, targetPos + temp)) return false;

	return true;
}
*/

int TracerouteManager::CheckOnPolyIndex(TracerouteInfo *pInfo, const Vector3D &pos)
{
	assert(pInfo->pStaticMesh && "�o�H�T���p�̒n�`�f�[�^������܂���");

	//���f���̋t�s��Z�o
	Matrix world = *pInfo->pStaticMesh->GetWorldMatrix();
	Matrix local = *pInfo->pStaticMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Matrix inverse = mat.GetInverse();

	//�|���S�����
	const VertexInfo *pVertex = pInfo->pStaticMesh->GetVertex();
	const IndexInfo *index = pInfo->pStaticMesh->GetIndex();
	const int polyNum = pInfo->pStaticMesh->GetFaceAllNum();

	TriangleInfo triangle;
	Vector3D point = pos * inverse;

	//���������|���S���ԍ���Ԃ�
	for (int i = 0; i < polyNum; i++)
	{
		triangle.v1 = pVertex[index[i].vertexIndex[0]].pos;
		triangle.v2 = pVertex[index[i].vertexIndex[1]].pos;
		triangle.v3 = pVertex[index[i].vertexIndex[2]].pos;

		if (CollisionMath::TriangleIntersect(point, triangle, pInfo->pNormal[i]))
		{
			return i;
		}	
	}

	/*
	//���̓����蔻����
	Vector3D linePosStart(pos.x, 1000.0f, pos.z);
	Vector3D linePosEnd(pos.x, -1000.0f, pos.z);

	//GPU�Ōv�Z
	if (pInfo->isGPU)
	{
		Result_Porygon pory;

		if (pInfo->copmute.Calculation(*pInfo->pStaticMesh, linePosStart, linePosEnd, &pory))
		{
			return pory.PoryIndex;
		}
	}
	else
	{
		//���f���̋t�s��Z�o
		Matrix world = *pInfo->pStaticMesh->GetWorldMatrix();
		Matrix local = *pInfo->pStaticMesh->GetLocalMatrix();
		Matrix mat = local * world;
		Matrix inverse = mat.GetInverse();

		HitResult_SegmentTriangle hit;

		//�����蔻��̌`��
		TriangleInfo hitTriangle;
		LineSegmentInfo hitLine(linePosStart * inverse, linePosEnd * inverse);

		//�|���S�����
		const VertexInfo *pVertex = pInfo->pStaticMesh->GetVertex();
		const IndexInfo *index = pInfo->pStaticMesh->GetIndex();
		const int polyNum = pInfo->pStaticMesh->GetFaceAllNum();

		//���������|���S���ԍ���Ԃ�
		for (int i = 0; i < polyNum; i++)
		{
			hitTriangle.v1 = pVertex[index[i].vertexIndex[0]].pos;
			hitTriangle.v2 = pVertex[index[i].vertexIndex[1]].pos;
			hitTriangle.v3 = pVertex[index[i].vertexIndex[2]].pos;

			hit = Collision3D::LineSegmentTriangle(hitLine, hitTriangle, pInfo->pNormal[i]);
			if (hit.isHit) return i;
		}

	}
	*/
	return -1;
}

void TracerouteManager::SetupPathPlanning(TracerouteSearch *pSearch, const Vector3D &goalPos)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[pSearch->m_NameTopography];
	auto *pPath = &pSearch->m_PathPlanning;

	assert(pInfo->pStaticMesh && "�o�H�T���p�̒n�`�f�[�^������܂���");

	if (!pInfo->pStaticMesh) return;

	int polyIndex = 0;
	PathPlanningUnit *pUnit = nullptr;
	PathPlanningUnit *pUnitSub = nullptr;

	//�f�[�^���c���Ă�����폜
	if (pPath->pUnitArray != nullptr)
	{
		delete[]pPath->pUnitArray;
		pPath->pUnitArray = nullptr;
	}
	int polyNum = pInfo->pStaticMesh->GetFaceAllNum();
	pPath->pUnitArray = new PathPlanningUnit[polyNum];

	//�o�H�T���p���̏�����
	for (int i = 0; i < polyNum; i++)
	{
		pPath->pUnitArray[i].polyIndex = i;
	}
	pPath->pGoalUnit = nullptr;
	pPath->pStartUnit = nullptr;

	//�X�^�[�g�̈ʒu�ɂ���|���S���ԍ����擾���āA�|���S���̌o�H�T�������p�̍\���̂̃A�h���X��ۑ�
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo, *pSearch->m_PathMoveInfo.pNowPosition);
	if (polyIndex == -1) return;
	pPath->pStartUnit = &pPath->pUnitArray[polyIndex];

	//�o�H�T�������Ώۂ̃|���S���Ƃ��ăX�^�[�g�n�_�ɂ���|���S����o�^����
	pPath->pActiveFirstUnit = &pPath->pUnitArray[polyIndex];

	//�S�[���̈ʒu�ɂ���|���S���ԍ����擾���āA�|���S���̌o�H�T�������p�̍\���̂̃A�h���X��ۑ�
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo, goalPos);
	if (polyIndex == -1) return;
	pPath->pGoalUnit = &pPath->pUnitArray[polyIndex];

	//���łɃS�[�����Ă�����I��
	if (pPath->pGoalUnit == pPath->pStartUnit)
	{
		pSearch->m_IsGoal = true;
		return;
	}
	else
	{
		pSearch->m_IsGoal = false;
	}

	//�o�H��T�����ăS�[���ɒn�_�ɂ��ǂ���܂ŌJ��Ԃ�
	PathPlanningFunc(pPath->pStartUnit, nullptr, 0, pInfo, pPath);

	//�S�[���n�_����X�^�[�g�n�_�܂ŒH�莟�Ɉړ�����|���S���ԍ����i�[
	pUnit = pPath->pGoalUnit;
	//�S�[���܂ŒT���ł��Ȃ�����
	if (pUnit->prevPolyIndex == -1) return;

	do
	{
		//���ɉ�����
		pUnitSub = pUnit;
		pUnit = &pPath->pUnitArray[pUnitSub->prevPolyIndex];

		//���ɂ���|���S���ԍ��i�[
		pUnit->NextPolyIndex = pUnitSub->polyIndex;

	} while (pUnit != pPath->pStartUnit);
}

void TracerouteManager::PathPlanningFunc(PathPlanningUnit *pUnit, PathPlanningUnit *pUintPrev, int idx, TracerouteInfo *pInfo, PathPlanning *pPathPlanning)
{
	if (pUintPrev) 
	{
		float distance = pUintPrev->totalDistance + pInfo->pPoryLinkInfo[pUintPrev->polyIndex].linkPolyDistance[idx];
		// ���Ɍo�H�T���������s���Ă��āA����苗���̒����o�H�ƂȂ��Ă��邩�A
		// �X�^�[�g�n�_�̃|���S���������ꍇ�͉������Ȃ�
		if (((pUnit->prevPolyIndex != -1) && (pUnit->totalDistance <= distance)) ||
			(pUnit->polyIndex == pPathPlanning->pStartUnit->polyIndex)) return;

		//�|���S���Ɍo�H���ɂȂ�o�R���̃|���S���ԍ�����
		pUnit->prevPolyIndex = pUintPrev->polyIndex;

		//�|���S���ɓ��B����܂ł̋�������
		pUnit->totalDistance = distance;
	}
	//�אڂ���|���S�����S�[���Ȃ�
	if (pUnit->polyIndex == pPathPlanning->pGoalUnit->polyIndex) return;

	//�|���S���̕ӂ̐������J��Ԃ�
	for (int i = 0; i < 3; i++)
	{
		//�אڂȂ�
		if (pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i] == -1) continue;
		PathPlanningUnit *pUnitSub = nullptr;
		pUnitSub = &pPathPlanning->pUnitArray[pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i]];
		PathPlanningFunc(pUnitSub, pUnit, i, pInfo, pPathPlanning);
	}
}

void TracerouteManager::AddSearch(TracerouteSearch *pSearch)
{
	TracerouteManager *pInfo = GetInstance();
	pInfo->m_pTraceroutePimpl->pathPlanning.push_back(pSearch);
}

void TracerouteManager::AddTarget(TracerouteTarget *pTarget)
{
	GetInstance()->m_pTraceroutePimpl->targetInfo.push_back(pTarget);
}

void TracerouteManager::ClearSearch(TracerouteSearch *pSearch)
{
	auto *p = &GetInstance()->m_pTraceroutePimpl->pathPlanning;
	auto it = p->begin();
	auto itEnd = p->end();

	for (; it != itEnd; it++)
	{
		if (*it == pSearch)
		{
			it = p->erase(it);
			break;
		}
	}
}

void TracerouteManager::ClearTarget(TracerouteTarget *pTarget)
{
	auto *p = &GetInstance()->m_pTraceroutePimpl->targetInfo;
	auto it = p->begin();
	auto itEnd = p->end();

	for (; it != itEnd; it++)
	{
		if (*it == pTarget)
		{
			it = p->erase(it);
			break;
		}
	}
}

void TracerouteManager::DebugMigrationPath(const char *name, TracerouteSearch *pSearch)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name]; //�o�H�T���p�n�`�f�[�^
													   //�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	const VertexInfo *pVer = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();

	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Vector3D pos[3];

	//�X�^�[�g�n�_���S�[���n�_��������Ȃ��������΂�
	if (pSearch->m_PathPlanning.pGoalUnit == nullptr) return;
	if (pSearch->m_PathPlanning.pStartUnit == nullptr) return;

	PathPlanningUnit *pUnit = pSearch->m_PathPlanning.pGoalUnit;
	for (;;)
	{
		pos[0] = pVer[index[pUnit->polyIndex].vertexIndex[0]].pos * mat;
		pos[1] = pVer[index[pUnit->polyIndex].vertexIndex[1]].pos * mat;
		pos[2] = pVer[index[pUnit->polyIndex].vertexIndex[2]].pos * mat;

		//pos[0] += pVer[index[pUnit->polyIndex].vertexIndex[0]].normal * 0.2f;
		//pos[1] += pVer[index[pUnit->polyIndex].vertexIndex[1]].normal * 0.2f;
		//pos[2] += pVer[index[pUnit->polyIndex].vertexIndex[2]].normal * 0.2f;

		Fiqure::RenderLine3D(pos[0], pos[1], Vector3D(1.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(pos[1], pos[2], Vector3D(1.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(pos[2], pos[0], Vector3D(1.0f, 0.0f, 0.0f));

		if (pUnit->prevPolyIndex == -1) break;
		pUnit = &pSearch->m_PathPlanning.pUnitArray[pUnit->prevPolyIndex];
	}
}

void TracerouteManager::DebugCenterPath(const char *name, TracerouteSearch *pSearch)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name]; //�o�H�T���p�n�`�f�[�^

	Vector3D pos[2];

	//�X�^�[�g�n�_���S�[���n�_��������Ȃ��������΂�
	if (pSearch->m_PathPlanning.pGoalUnit == nullptr) return;
	if (pSearch->m_PathPlanning.pStartUnit == nullptr) return;

	PathPlanningUnit *pUnit = pSearch->m_PathPlanning.pGoalUnit;
	for (;;)
	{
		pos[0] = pInfo->pPoryLinkInfo[pUnit->prevPolyIndex].centerPosition;

		//���Ɍ������p�X
		pUnit = &pSearch->m_PathPlanning.pUnitArray[pUnit->prevPolyIndex];
		if (pUnit->prevPolyIndex == -1) break;

		pos[1] = pInfo->pPoryLinkInfo[pUnit->prevPolyIndex].centerPosition;

		Fiqure::RenderLine3D(pos[0], pos[1], Vector3D(0.0f, 1.0f, 0.0f));
	}
}