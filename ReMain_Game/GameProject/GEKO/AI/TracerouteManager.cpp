#include "TracerouteManager.h"
#include "..\Mesh\StaticMesh.h"
#include "TracerouteInfo.h"
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
	pInfo->m_pTraceroutePimpl->tracerouteInfo[name];
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

}

void TracerouteManager::ProcessBuilding(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;
	const auto *pVertex = pMesh->GetVertex();
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
	pPoryLinkInfo = new PolyLinkInfo[polyNum];

	//全てのポリゴンの中心座標取得
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = pVertex[index[i].vertexIndex[0]].pos * mat;
		pos[1] = pVertex[index[i].vertexIndex[1]].pos * mat;
		pos[2] = pVertex[index[i].vertexIndex[2]].pos * mat;

		pPoryLinkInfo[i].centerPosition = (pos[0] + pos[1] + pos[2]) * 0.33333333333f;
	}

	//ポリゴン同士の隣接情報の構築
	for (int i = 0; i < polyNum; i++)
	{
		pPoryLinkInfo[i].linkPolyIndex[0] = -1;
		pPoryLinkInfo[i].linkPolyIndex[1] = -1;
		pPoryLinkInfo[i].linkPolyIndex[2] = -1;

		//隣接するポリゴンを探す
		for (int j = 0; j < polyNum; j++)
		{
			if (i == j) continue; //自分のポリゴンなら飛ばす

			bool is = (pPoryLinkInfo[i].linkPolyIndex[0] == -1);

			if (pPoryLinkInfo[i].linkPolyIndex[0] == -1 &&
				((index[i].vertexIndex[0] == index[j].vertexIndex[0])))
			{

			}
		}
	}
}