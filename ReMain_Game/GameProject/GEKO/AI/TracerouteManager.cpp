#include "TracerouteManager.h"
#include "..\Mesh\StaticMesh.h"
#include "TracerouteInfo.h"
#include "..\Figure\Fiqure.h"
#include "..\System\Collision.h"
#include <map>

class TracerouteManager::TraceroutePimpl
{
public:
	std::map<const char*, TracerouteInfo> tracerouteInfo;
};

TracerouteManager::TracerouteManager()
{
	m_pTraceroutePimpl = new TraceroutePimpl;
}

TracerouteManager::~TracerouteManager()
{
	for (auto& i : m_pTraceroutePimpl->tracerouteInfo)
	{
		if(i.second.pPoryLinkInfo) delete[] i.second.pPoryLinkInfo;
		if(i.second.pathPlanning.pUnitArray == nullptr) delete[] i.second.pathPlanning.pUnitArray;
		i.second.pStaticMesh = nullptr;
	}

	m_pTraceroutePimpl->tracerouteInfo.clear();
	delete m_pTraceroutePimpl;
}

TracerouteManager* TracerouteManager::GetInstance()
{
	static TracerouteManager tracerouteManager;
	return &tracerouteManager;
}

void TracerouteManager::Add(StaticMesh *staticMesh, const char* name)
{
	TracerouteManager *pInfo = GetInstance();
	pInfo->m_pTraceroutePimpl->tracerouteInfo[name].pStaticMesh = staticMesh;
}

void TracerouteManager::Clear(const char* name)
{

}

void TracerouteManager::AlClear()
{

}

void TracerouteManager::DebugRender(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

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

void TracerouteManager::DebugMigrationPath(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	//�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	const VertexInfo *pVer = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();

	pMesh->WorldMatrixBuilding();
	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Vector3D pos[3];

	PathPlanningUnit *pUnit = pInfo->pathPlanning.pGoalUnit;
	for (;;)
	{
		pos[0] = pVer[index[pUnit->polyIndex].vertexIndex[0]].pos * mat;
		pos[1] = pVer[index[pUnit->polyIndex].vertexIndex[1]].pos * mat;
		pos[2] = pVer[index[pUnit->polyIndex].vertexIndex[2]].pos * mat;

		pos[0].y += 0.2f;
		pos[1].y += 0.2f;
		pos[2].y += 0.2f;

		Fiqure::RenderLine3D(pos[0], pos[1], Vector3D(1.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(pos[1], pos[2], Vector3D(1.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(pos[2], pos[0], Vector3D(1.0f, 0.0f, 0.0f));

		if (pUnit->prevPolyIndex == -1) break;

		pUnit = &pInfo->pathPlanning.pUnitArray[pUnit->prevPolyIndex];
 	}
}

void TracerouteManager::ProcessBuilding(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	//�|���S�����
	auto *pMesh = pInfo->pStaticMesh; //���b�V��
	const VertexInfo *pVertex = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();
	const int polyNum = pMesh->GetFaceAllNum();
	Vector3D pos[3];

	pMesh->WorldMatrixBuilding();
	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;

	if (pInfo->pPoryLinkInfo != nullptr)
	{
		delete[] pInfo->pPoryLinkInfo;
		pInfo->pPoryLinkInfo = nullptr;
	}

	pInfo->pPoryLinkInfo = new PolyLinkInfo[polyNum];
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;

	//�S�Ẵ|���S���̒��S���W�擾
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = pVertex[index[i].vertexIndex[0]].pos * mat;
		pos[1] = pVertex[index[i].vertexIndex[1]].pos * mat;
		pos[2] = pVertex[index[i].vertexIndex[2]].pos * mat;

		pPoryLinkInfo[i].centerPosition = (pos[0] + pos[1] + pos[2]) * 0.33333333333f;

		//�|���S���̗אڏ���������
		pPoryLinkInfo[i].linkPolyIndex[0] = -1;
		pPoryLinkInfo[i].linkPolyIndex[1] = -1;
		pPoryLinkInfo[i].linkPolyIndex[2] = -1;
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

			/*
			//���_�ԍ�0��1�ŕӂ̂Ɨאڂ�����אڂ������_�ԍ��Ɨאڃ|���S���̒��S���W�܂ł̋����擾
			if ((pPoryLinkInfo[i].linkPolyIndex[0] == -1) &&
				(
					(pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[0]].pos && pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[2]].pos) ||
					(pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[1]].pos && pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[0]].pos) ||
					(pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[2]].pos && pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[1]].pos) )
				)
			{
				pPoryLinkInfo[i].linkPolyIndex[0] = j;
				pPoryLinkInfo[i].linkPolyDistance[0] = (pPoryLinkInfo[j].centerPosition - pPoryLinkInfo[i].centerPosition).Length();
			}
			else
			//���_�ԍ�0��1�ŕӂ̂Ɨאڂ�����אڂ������_�ԍ��Ɨאڃ|���S���̒��S���W�܂ł̋����擾
			if ((pPoryLinkInfo[i].linkPolyIndex[1] == -1) && 
				(
					(pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[0]].pos && pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[2]].pos) ||
					(pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[1]].pos && pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[0]].pos) ||
					(pVertex[index[i].vertexIndex[1]].pos == pVertex[index[j].vertexIndex[2]].pos && pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[1]].pos))
				)
			{
				pPoryLinkInfo[i].linkPolyIndex[1] = j;
				pPoryLinkInfo[i].linkPolyDistance[1] = (pPoryLinkInfo[j].centerPosition - pPoryLinkInfo[i].centerPosition).Length();
			}
			else
			//���_�ԍ�0��1�ŕӂ̂Ɨאڂ�����אڂ������_�ԍ��Ɨאڃ|���S���̒��S���W�܂ł̋����擾
			if ((pPoryLinkInfo[i].linkPolyIndex[2] == -1) &&
				(
					(pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[0]].pos && pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[2]].pos) ||
					(pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[1]].pos && pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[0]].pos) ||
					(pVertex[index[i].vertexIndex[2]].pos == pVertex[index[j].vertexIndex[2]].pos && pVertex[index[i].vertexIndex[0]].pos == pVertex[index[j].vertexIndex[1]].pos))
				)
			{
				pPoryLinkInfo[i].linkPolyIndex[2] = j;
				pPoryLinkInfo[i].linkPolyDistance[2] = (pPoryLinkInfo[j].centerPosition - pPoryLinkInfo[i].centerPosition).Length();
			}
			*/

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

void TracerouteManager::SetupPathPlanning(const char* name, const Vector3D &startPos, const Vector3D &goalPos)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	bool isGoal = false;
	int polyIndex = 0;
	PathPlanningUnit *pUnit = nullptr;
	PathPlanningUnit *pUnitSub = nullptr;
	PathPlanningUnit *pUnitSub2 = nullptr;

	//�f�[�^���c���Ă�����폜
	if (pInfo->pathPlanning.pUnitArray == nullptr)
	{
		delete[] pInfo->pathPlanning.pUnitArray;
		pInfo->pathPlanning.pUnitArray = nullptr;
	}
	int polyNum = pInfo->pStaticMesh->GetFaceAllNum();
	pInfo->pathPlanning.pUnitArray = new PathPlanningUnit[polyNum];
	PathPlanning *pPathPlanning = &pInfo->pathPlanning;

	//�o�H�T���p���̏�����
	pUnit = pPathPlanning->pUnitArray;
	for (int i = 0; i < polyNum; i++)
	{
		pPathPlanning->pUnitArray[i].polyIndex = i;
		pPathPlanning->pUnitArray[i].prevPolyIndex = -1;
		pPathPlanning->pUnitArray[i].NextPolyIndex = -1;
		pPathPlanning->pUnitArray[i].totalDistance = 0.0f;
		pPathPlanning->pUnitArray[i].pActiveNextUnit = nullptr;
	}

	//�X�^�[�g�̈ʒu�ɂ���|���S���ԍ����擾���āA�|���S���̌o�H�T�������p�̍\���̂̃A�h���X��ۑ�
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo->pStaticMesh, startPos);
	if (polyIndex == -1) return;
	pPathPlanning->pStartUnit = &pPathPlanning->pUnitArray[polyIndex];

	//�o�H�T�������Ώۂ̃|���S���Ƃ��ăX�^�[�g�n�_�ɂ���|���S����o�^����
	pPathPlanning->pActiveFirstUnit = &pPathPlanning->pUnitArray[polyIndex];

	//�S�[���̈ʒu�ɂ���|���S���ԍ����擾���āA�|���S���̌o�H�T�������p�̍\���̂̃A�h���X��ۑ�
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo->pStaticMesh, goalPos);
	if (polyIndex == -1) return;
	pPathPlanning->pGoalUnit = &pPathPlanning->pUnitArray[polyIndex];

	//���łɃS�[�����Ă�����I��
	if (pPathPlanning->pGoalUnit == pPathPlanning->pStartUnit) return;

	//�o�H��T�����ăS�[���ɒn�_�ɂ��ǂ���܂ŌJ��Ԃ�
	float distance = 0.0f;
	while (!isGoal)
	{
		//�o�H�Ώۂ̃|���S�����ׂď�������
		pUnit = pPathPlanning->pActiveFirstUnit;
		pPathPlanning->pActiveFirstUnit = nullptr;
		for (; pUnit != nullptr; pUnit = pUnit->pActiveNextUnit)
		{
			//�|���S���̕ӂ̐������J��Ԃ�
			for (int i = 0; i < 3; i++)
			{
				//�אڂȂ�
				if (pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i] == -1) continue;

				pUnitSub = &pPathPlanning->pUnitArray[pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i]];
				distance = pUnit->totalDistance + pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyDistance[i];

				// �אڂ���|���S�������Ɍo�H�T���������s���Ă��āA����苗���̒����o�H�ƂȂ��Ă��邩�A
				// �X�^�[�g�n�_�̃|���S���������ꍇ�͉������Ȃ�
				if ((pUnitSub->prevPolyIndex != -1 && pUnitSub->totalDistance <= distance) ||
					pUnitSub->polyIndex == pPathPlanning->pStartUnit->polyIndex) continue;

				//�אڂ���|���S�����S�[���Ȃ�
				if (pUnitSub->polyIndex == pPathPlanning->pGoalUnit->polyIndex) isGoal = true;

				//�אڂ���|���S���Ɍo�H���ɂȂ鎩���̃|���S���ԍ�����
				pUnitSub->prevPolyIndex = pUnit->polyIndex;

				//�אڂ���|���S���ɓ��B����܂ł̋�������
				pUnitSub->totalDistance = distance;

				//���ɍs�����[�v�Ōo�H�T�������Ώۂɒǉ�����A���łɒǉ����ꂽ��ǉ����Ȃ�
				for (pUnitSub2 = pPathPlanning->pActiveFirstUnit; pUnitSub2 != nullptr; pUnitSub2  = pUnitSub2->pActiveNextUnit)
				{
					if (pUnitSub2 == pUnitSub) break;
				}

				if (pUnitSub2 == nullptr)
				{
					pUnitSub->pActiveNextUnit = pPathPlanning->pActiveFirstUnit;
					pPathPlanning->pActiveFirstUnit = pUnitSub;
				}
			}
		}

		//nullptr�Ȃ�X�^�[�g�n�_����S�[���n�_�ɂ��ǂ蒅���Ȃ�
		if (pPathPlanning->pActiveFirstUnit == nullptr) return;
	}

	//�S�[���n�_����X�^�[�g�n�_�܂ŒH�莟�Ɉړ�����|���S���ԍ����i�[
	pUnit = pPathPlanning->pGoalUnit;
	do
	{
		//���ɉ�����
		pUnitSub = pUnit;
		pUnit = &pPathPlanning->pUnitArray[pUnitSub->prevPolyIndex];

		//���ɂ���|���S���ԍ��i�[
		pUnit->NextPolyIndex = pUnitSub->polyIndex;

	} while (pUnit != pPathPlanning->pStartUnit);
	//return;
}

int TracerouteManager::CheckOnPolyIndex(StaticMesh *pStaticMesh, const Vector3D &pos)
{
	//���f���̋t�s��Z�o
	Matrix world = *pStaticMesh->GetWorldMatrix();
	Matrix local = *pStaticMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Matrix inverse = mat.GetInverse();

	//���̓����蔻����
	Vector3D linePosStart(pos.x, 1000.0f, pos.z);
	Vector3D linePosEnd(pos.x, -1000.0f, pos.z);
	HitResult_SegmentTriangle hit;

	//�����蔻��̌`��
	TriangleInfo hitTriangle;
	LineSegmentInfo hitLine(linePosStart * inverse, linePosEnd * inverse);

	//�|���S�����
	const VertexInfo *pVertex = pStaticMesh->GetVertex();
	const IndexInfo *index = pStaticMesh->GetIndex();
	const int polyNum = pStaticMesh->GetFaceAllNum();

	//���������|���S���ԍ���Ԃ�
	for (int i = 0; i < polyNum; i++)
	{
		hitTriangle.v1 = pVertex[index[i].vertexIndex[0]].pos;
		hitTriangle.v2 = pVertex[index[i].vertexIndex[1]].pos;
		hitTriangle.v3 = pVertex[index[i].vertexIndex[2]].pos;

		hit = Collision3D::LineSegmentTriangle(hitLine, hitTriangle);
		if (hit.isHit) return i;
	}

	return -1;
}