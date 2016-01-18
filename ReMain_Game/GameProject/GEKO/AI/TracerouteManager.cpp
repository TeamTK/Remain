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

void TracerouteManager::Add(const StaticMesh *staticMesh, const char* name)
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
	auto *pMesh = pInfo->pStaticMesh;

	const auto *pVertex = pMesh->GetVertex();
	const auto *pMaterial = pMesh->GetMaterial();
	const int materialAllNum = pMesh->GetMaterialAllNum();
	const int *index;
	int faceNum = 0;
	Vector3D pos[3];

	//*pMesh->WorldMatrixBuilding();
	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;

	if (pInfo->pPoryLinkInfo != nullptr)
	{
		delete[] pInfo->pPoryLinkInfo;
		pInfo->pPoryLinkInfo = nullptr;
	}
	pInfo->pPoryLinkInfo = new PolyLinkInfo[pMesh->GetFaceAllNum()];

	//マテリアル別にポリゴンの中心座標を算出
	for (int i = 0; i < materialAllNum; i++)
	{
		//マテリアル別のインデックスを取得
		index = pMesh->GetPolygonIndex(i);
		faceNum = pMaterial[i].dwNumFace;
		for (int j = 0; j < faceNum; j++)
		{
			pos[0] = pVertex[index[j * 3]].vPos;
			pos[1] = pVertex[index[j * 3 + 1]].vPos;
			pos[2] = pVertex[index[j * 3 + 2]].vPos;

			//ワールド座標に変換
			pos[0] = pos[0] * mat;
			pos[1] = pos[1] * mat;
			pos[2] = pos[2] * mat;

			pInfo->pPoryLinkInfo[j].centerPosition = (pos[0] + pos[1] + pos[2]) * 0.3f;
		}
	}

	//マテリアル別にポリゴンの中心座標を算出
	for (int i = 0; i < materialAllNum; i++)
	{
		//マテリアル別のインデックスを取得
		index = pMesh->GetPolygonIndex(i);
		faceNum = pMaterial[i].dwNumFace;
		for (int j = 0; j < faceNum; j++)
		{
			pInfo->pPoryLinkInfo[j].linkPolyIndex[0] = -1;
			pInfo->pPoryLinkInfo[j].linkPolyIndex[1] = -1;
			pInfo->pPoryLinkInfo[j].linkPolyIndex[2] = -1;


		}
	}
}