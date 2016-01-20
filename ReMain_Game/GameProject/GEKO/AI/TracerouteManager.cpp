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

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
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

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
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

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
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

	//全てのポリゴンの中心座標取得
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = pVertex[index[i].vertexIndex[0]].pos * mat;
		pos[1] = pVertex[index[i].vertexIndex[1]].pos * mat;
		pos[2] = pVertex[index[i].vertexIndex[2]].pos * mat;

		pPoryLinkInfo[i].centerPosition = (pos[0] + pos[1] + pos[2]) * 0.33333333333f;

		//ポリゴンの隣接情報を初期化
		pPoryLinkInfo[i].linkPolyIndex[0] = -1;
		pPoryLinkInfo[i].linkPolyIndex[1] = -1;
		pPoryLinkInfo[i].linkPolyIndex[2] = -1;
	}

	bool isNoNeighborhood = false;
	bool isIndex[3] = {false, false, false};

	//ポリゴン同士の隣接情報の構築
	for (int i = 0; i < polyNum; i++)
	{
		//隣接するポリゴンを探す
		for (int j = 0; j < polyNum; j++)
		{
			if (i == j) continue; //自分のポリゴンなら飛ばす

			/*
			//頂点番号0と1で辺のと隣接したら隣接した頂点番号と隣接ポリゴンの中心座標までの距離取得
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
			//頂点番号0と1で辺のと隣接したら隣接した頂点番号と隣接ポリゴンの中心座標までの距離取得
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
			//頂点番号0と1で辺のと隣接したら隣接した頂点番号と隣接ポリゴンの中心座標までの距離取得
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
				//頂点番号0と1で辺のと隣接したら隣接した頂点番号と隣接ポリゴンの中心座標までの距離取得
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

	//データが残っていたら削除
	if (pInfo->pathPlanning.pUnitArray == nullptr)
	{
		delete[] pInfo->pathPlanning.pUnitArray;
		pInfo->pathPlanning.pUnitArray = nullptr;
	}
	int polyNum = pInfo->pStaticMesh->GetFaceAllNum();
	pInfo->pathPlanning.pUnitArray = new PathPlanningUnit[polyNum];
	PathPlanning *pPathPlanning = &pInfo->pathPlanning;

	//経路探索用情報の初期化
	pUnit = pPathPlanning->pUnitArray;
	for (int i = 0; i < polyNum; i++)
	{
		pPathPlanning->pUnitArray[i].polyIndex = i;
		pPathPlanning->pUnitArray[i].prevPolyIndex = -1;
		pPathPlanning->pUnitArray[i].NextPolyIndex = -1;
		pPathPlanning->pUnitArray[i].totalDistance = 0.0f;
		pPathPlanning->pUnitArray[i].pActiveNextUnit = nullptr;
	}

	//スタートの位置にあるポリゴン番号を取得して、ポリゴンの経路探索処理用の構造体のアドレスを保存
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo->pStaticMesh, startPos);
	if (polyIndex == -1) return;
	pPathPlanning->pStartUnit = &pPathPlanning->pUnitArray[polyIndex];

	//経路探索処理対象のポリゴンとしてスタート地点にあるポリゴンを登録する
	pPathPlanning->pActiveFirstUnit = &pPathPlanning->pUnitArray[polyIndex];

	//ゴールの位置にあるポリゴン番号を取得して、ポリゴンの経路探索処理用の構造体のアドレスを保存
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo->pStaticMesh, goalPos);
	if (polyIndex == -1) return;
	pPathPlanning->pGoalUnit = &pPathPlanning->pUnitArray[polyIndex];

	//すでにゴールしていたら終了
	if (pPathPlanning->pGoalUnit == pPathPlanning->pStartUnit) return;

	//経路を探索してゴールに地点にたどりつくまで繰り返す
	float distance = 0.0f;
	while (!isGoal)
	{
		//経路対象のポリゴンすべて処理する
		pUnit = pPathPlanning->pActiveFirstUnit;
		pPathPlanning->pActiveFirstUnit = nullptr;
		for (; pUnit != nullptr; pUnit = pUnit->pActiveNextUnit)
		{
			//ポリゴンの辺の数だけ繰り返す
			for (int i = 0; i < 3; i++)
			{
				//隣接なし
				if (pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i] == -1) continue;

				pUnitSub = &pPathPlanning->pUnitArray[pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyIndex[i]];
				distance = pUnit->totalDistance + pInfo->pPoryLinkInfo[pUnit->polyIndex].linkPolyDistance[i];

				// 隣接するポリゴンが既に経路探索処理が行われていて、且つより距離の長い経路となっているか、
				// スタート地点のポリゴンだった場合は何もしない
				if ((pUnitSub->prevPolyIndex != -1 && pUnitSub->totalDistance <= distance) ||
					pUnitSub->polyIndex == pPathPlanning->pStartUnit->polyIndex) continue;

				//隣接するポリゴンがゴールなら
				if (pUnitSub->polyIndex == pPathPlanning->pGoalUnit->polyIndex) isGoal = true;

				//隣接するポリゴンに経路情報になる自分のポリゴン番号を代入
				pUnitSub->prevPolyIndex = pUnit->polyIndex;

				//隣接するポリゴンに到達するまでの距離を代入
				pUnitSub->totalDistance = distance;

				//次に行うループで経路探索処理対象に追加する、すでに追加されたら追加しない
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

		//nullptrならスタート地点からゴール地点にたどり着けない
		if (pPathPlanning->pActiveFirstUnit == nullptr) return;
	}

	//ゴール地点からスタート地点まで辿り次に移動するポリゴン番号を格納
	pUnit = pPathPlanning->pGoalUnit;
	do
	{
		//後ろに下がる
		pUnitSub = pUnit;
		pUnit = &pPathPlanning->pUnitArray[pUnitSub->prevPolyIndex];

		//次にあるポリゴン番号格納
		pUnit->NextPolyIndex = pUnitSub->polyIndex;

	} while (pUnit != pPathPlanning->pStartUnit);
	//return;
}

int TracerouteManager::CheckOnPolyIndex(StaticMesh *pStaticMesh, const Vector3D &pos)
{
	//モデルの逆行列算出
	Matrix world = *pStaticMesh->GetWorldMatrix();
	Matrix local = *pStaticMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Matrix inverse = mat.GetInverse();

	//線の当たり判定情報
	Vector3D linePosStart(pos.x, 1000.0f, pos.z);
	Vector3D linePosEnd(pos.x, -1000.0f, pos.z);
	HitResult_SegmentTriangle hit;

	//当たり判定の形状
	TriangleInfo hitTriangle;
	LineSegmentInfo hitLine(linePosStart * inverse, linePosEnd * inverse);

	//ポリゴン情報
	const VertexInfo *pVertex = pStaticMesh->GetVertex();
	const IndexInfo *index = pStaticMesh->GetIndex();
	const int polyNum = pStaticMesh->GetFaceAllNum();

	//当たったポリゴン番号を返す
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