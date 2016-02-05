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
	std::map<const char*, TracerouteInfo> tracerouteInfo; //経路探索をするための情報
	std::list<TracerouteSearch*> pathPlanning; //経路探索が算出された情報
	std::list<TracerouteTarget*> targetInfo; //経路探索対象の情報
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
	//経路探索をするための情報削除
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

	assert(pInfo->pStaticMesh && "経路探索用の地形データがありません");

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

void TracerouteManager::DebugTopography(const char* name)
{
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pInfo = &pTraceroute->tracerouteInfo[name];

	assert(pInfo->pStaticMesh && "経路探索用の地形データがありません");

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
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

	assert(pInfo->pStaticMesh && "経路探索用の地形データがありません");
	pInfo->isGPU = isGPU;

	//ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
	const VertexInfo *pVertex = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();
	const int polyNum = pMesh->GetFaceAllNum();
	Vector3D pos[3];

	//メッシュの行列
	pMesh->WorldMatrixBuilding();
	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;

	//隣接するポリゴン情報
	if (pInfo->pPoryLinkInfo != nullptr)
	{
		delete[] pInfo->pPoryLinkInfo;
		pInfo->pPoryLinkInfo = nullptr;
	}
	pInfo->pPoryLinkInfo = new PolyLinkInfo[polyNum];
	auto *pPoryLinkInfo = pInfo->pPoryLinkInfo;

	//経路探索地形の当たり判定に使う法線
	if (pInfo->pNormal != nullptr)
	{
		delete[] pInfo->pNormal;
		pInfo->pNormal = nullptr;
	}
	pInfo->pNormal = new Vector3D[polyNum];

	//全てのポリゴンの中心座標取得
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = pVertex[index[i].vertexIndex[0]].pos;
		pos[1] = pVertex[index[i].vertexIndex[1]].pos;
		pos[2] = pVertex[index[i].vertexIndex[2]].pos;

		//三角形の中心点
		pPoryLinkInfo[i].centerPosition = (pos[0] + pos[1] + pos[2]) * mat * 0.33333333333f;

		//当たり判定用の法線
		pInfo->pNormal[i] = Vector3D::Cross(pos[2] - pos[0], pos[1] - pos[0]).GetNormalize();
	}

	//GPUで地形の当たり判定構築
	if (pInfo->isGPU)
	{
		//pInfo->copmute = new LineTriangleCopmute;
		pInfo->copmute.Init(*pMesh, pInfo->pNormal);
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

void TracerouteManager::Update()
{
	TraceroutePimpl *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *pPathPlanning = &pTraceroute->pathPlanning;
	auto *pTarget = &pTraceroute->targetInfo;
	PathMoveInfo *pMove;
	PathPlanningUnit *pUnit;

	//指定された経路探索をする
	for (auto& i : *pPathPlanning)
	{
		if (!i->m_IsSerch) continue;

		for (auto& j : *pTarget)
		{
			//探索側が探索対象の名前と一致したら経路探索開始
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

	//決められた経路を進む
	for (auto& i : *pPathPlanning)
	{
		if (!i->m_IsSerchMove) continue;
		if (i->m_PathPlanning.pGoalUnit == nullptr) continue;
		if (i->m_PathPlanning.pStartUnit == nullptr) continue;

		pMove = &i->m_PathMoveInfo;
		
		//ゴールしたら方向をゼロにする
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

		// 現在乗っているポリゴンが移動中間地点のポリゴンの場合は次の中間地点を決定する処理を行う
		if (pMove->pNowPathPlanningUnit == pMove->pTargetPathPlanningUnit)
		{
			pUnit = &i->m_PathPlanning.pUnitArray[pMove->pTargetPathPlanningUnit->NextPolyIndex];
			pMove->pTargetPathPlanningUnit = pUnit; //次の経路に変更
		}
		else
		{
			// 現在の座標で乗っているポリゴンを検索する
			pMove->nowPolyIndex = GetInstance()->CheckOnPolyIndex(pInfo, *pMove->pNowPosition);

			// 現在の座標で乗っているポリゴンの経路探索情報のメモリアドレスを代入する
			pMove->pNowPathPlanningUnit = &i->m_PathPlanning.pUnitArray[pMove->nowPolyIndex];
		}
		
		//次の経路の中心座標に進む方角取得
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

	// 開始座標と目標座標の y座標値を 0.0f にして、平面上の判定にする
	//startPos.y = 0.0f;
	//targetPos.y = 0.0f;
	auto *pTraceroute = GetInstance()->m_pTraceroutePimpl;
	auto *path = &pTraceroute->tracerouteInfo[TopographyName];
	
	//開始座標と目標座標の直上、若しくは直下に存在するポリゴンを検索する
	startPory = CheckOnPolyIndex(path->pStaticMesh, path->pNormal, startPos);
	targetPoty = CheckOnPolyIndex(path->pStaticMesh, path->pNormal, targetPos);

	if (startPory == -1 || targetPoty == -1) return false;
}

bool TracerouteManager::CheckPolyMoveWidth(const char* TopographyName, const Vector3D& startPos, const Vector3D& targetPos, float width)
{
	Vector3D direcion;
	Vector3D sideDirecion;
	Vector3D temp;

	//最初に開始座標から目標座標に直線で移動できるか確認
	if (!CheckPolyMove(TopographyName, startPos, targetPos)) return false;

	direcion = targetPos - startPos;
	direcion.y = 0.0f;

	//開始座標から目標座標に向かうベクトルに直角な正規化ベクトルを算出
	sideDirecion = Vector3D::Cross(direcion, Vector3D(0.0f, 1.0f, 0.0f)).GetNormalize();

	//開始座標と目標座標を Width / 2.0f 分だけ垂直方向にずらして、再度直線的に移動できるかどうかを確認
	temp = sideDirecion * (width / 2.0f);
	if (!CheckPolyMove(TopographyName, startPos + temp, targetPos + temp)) return false;

	temp = sideDirecion * (-width / 2.0f);
	if (!CheckPolyMove(TopographyName, startPos + temp, targetPos + temp)) return false;

	return true;
}
*/

int TracerouteManager::CheckOnPolyIndex(TracerouteInfo *pInfo, const Vector3D &pos)
{
	assert(pInfo->pStaticMesh && "経路探索用の地形データがありません");

	//モデルの逆行列算出
	Matrix world = *pInfo->pStaticMesh->GetWorldMatrix();
	Matrix local = *pInfo->pStaticMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Matrix inverse = mat.GetInverse();

	//ポリゴン情報
	const VertexInfo *pVertex = pInfo->pStaticMesh->GetVertex();
	const IndexInfo *index = pInfo->pStaticMesh->GetIndex();
	const int polyNum = pInfo->pStaticMesh->GetFaceAllNum();

	TriangleInfo triangle;
	Vector3D point = pos * inverse;

	//当たったポリゴン番号を返す
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
	//線の当たり判定情報
	Vector3D linePosStart(pos.x, 1000.0f, pos.z);
	Vector3D linePosEnd(pos.x, -1000.0f, pos.z);

	//GPUで計算
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
		//モデルの逆行列算出
		Matrix world = *pInfo->pStaticMesh->GetWorldMatrix();
		Matrix local = *pInfo->pStaticMesh->GetLocalMatrix();
		Matrix mat = local * world;
		Matrix inverse = mat.GetInverse();

		HitResult_SegmentTriangle hit;

		//当たり判定の形状
		TriangleInfo hitTriangle;
		LineSegmentInfo hitLine(linePosStart * inverse, linePosEnd * inverse);

		//ポリゴン情報
		const VertexInfo *pVertex = pInfo->pStaticMesh->GetVertex();
		const IndexInfo *index = pInfo->pStaticMesh->GetIndex();
		const int polyNum = pInfo->pStaticMesh->GetFaceAllNum();

		//当たったポリゴン番号を返す
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

	assert(pInfo->pStaticMesh && "経路探索用の地形データがありません");

	if (!pInfo->pStaticMesh) return;

	int polyIndex = 0;
	PathPlanningUnit *pUnit = nullptr;
	PathPlanningUnit *pUnitSub = nullptr;

	//データが残っていたら削除
	if (pPath->pUnitArray != nullptr)
	{
		delete[]pPath->pUnitArray;
		pPath->pUnitArray = nullptr;
	}
	int polyNum = pInfo->pStaticMesh->GetFaceAllNum();
	pPath->pUnitArray = new PathPlanningUnit[polyNum];

	//経路探索用情報の初期化
	for (int i = 0; i < polyNum; i++)
	{
		pPath->pUnitArray[i].polyIndex = i;
	}
	pPath->pGoalUnit = nullptr;
	pPath->pStartUnit = nullptr;

	//スタートの位置にあるポリゴン番号を取得して、ポリゴンの経路探索処理用の構造体のアドレスを保存
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo, *pSearch->m_PathMoveInfo.pNowPosition);
	if (polyIndex == -1) return;
	pPath->pStartUnit = &pPath->pUnitArray[polyIndex];

	//経路探索処理対象のポリゴンとしてスタート地点にあるポリゴンを登録する
	pPath->pActiveFirstUnit = &pPath->pUnitArray[polyIndex];

	//ゴールの位置にあるポリゴン番号を取得して、ポリゴンの経路探索処理用の構造体のアドレスを保存
	polyIndex = TracerouteManager::GetInstance()->CheckOnPolyIndex(pInfo, goalPos);
	if (polyIndex == -1) return;
	pPath->pGoalUnit = &pPath->pUnitArray[polyIndex];

	//すでにゴールしていたら終了
	if (pPath->pGoalUnit == pPath->pStartUnit)
	{
		pSearch->m_IsGoal = true;
		return;
	}
	else
	{
		pSearch->m_IsGoal = false;
	}

	//経路を探索してゴールに地点にたどりつくまで繰り返す
	PathPlanningFunc(pPath->pStartUnit, nullptr, 0, pInfo, pPath);

	//ゴール地点からスタート地点まで辿り次に移動するポリゴン番号を格納
	pUnit = pPath->pGoalUnit;
	//ゴールまで探索できなかった
	if (pUnit->prevPolyIndex == -1) return;

	do
	{
		//後ろに下がる
		pUnitSub = pUnit;
		pUnit = &pPath->pUnitArray[pUnitSub->prevPolyIndex];

		//次にあるポリゴン番号格納
		pUnit->NextPolyIndex = pUnitSub->polyIndex;

	} while (pUnit != pPath->pStartUnit);
}

void TracerouteManager::PathPlanningFunc(PathPlanningUnit *pUnit, PathPlanningUnit *pUintPrev, int idx, TracerouteInfo *pInfo, PathPlanning *pPathPlanning)
{
	if (pUintPrev) 
	{
		float distance = pUintPrev->totalDistance + pInfo->pPoryLinkInfo[pUintPrev->polyIndex].linkPolyDistance[idx];
		// 既に経路探索処理が行われていて、且つより距離の長い経路となっているか、
		// スタート地点のポリゴンだった場合は何もしない
		if (((pUnit->prevPolyIndex != -1) && (pUnit->totalDistance <= distance)) ||
			(pUnit->polyIndex == pPathPlanning->pStartUnit->polyIndex)) return;

		//ポリゴンに経路情報になる経由元のポリゴン番号を代入
		pUnit->prevPolyIndex = pUintPrev->polyIndex;

		//ポリゴンに到達するまでの距離を代入
		pUnit->totalDistance = distance;
	}
	//隣接するポリゴンがゴールなら
	if (pUnit->polyIndex == pPathPlanning->pGoalUnit->polyIndex) return;

	//ポリゴンの辺の数だけ繰り返す
	for (int i = 0; i < 3; i++)
	{
		//隣接なし
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
	auto *pInfo = &pTraceroute->tracerouteInfo[name]; //経路探索用地形データ
													   //ポリゴン情報
	auto *pMesh = pInfo->pStaticMesh; //メッシュ
	const VertexInfo *pVer = pMesh->GetVertex();
	const IndexInfo *index = pMesh->GetIndex();

	Matrix world = *pMesh->GetWorldMatrix();
	Matrix local = *pMesh->GetLocalMatrix();
	Matrix mat = local * world;
	Vector3D pos[3];

	//スタート地点かゴール地点が見つからなかったら飛ばす
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
	auto *pInfo = &pTraceroute->tracerouteInfo[name]; //経路探索用地形データ

	Vector3D pos[2];

	//スタート地点かゴール地点が見つからなかったら飛ばす
	if (pSearch->m_PathPlanning.pGoalUnit == nullptr) return;
	if (pSearch->m_PathPlanning.pStartUnit == nullptr) return;

	PathPlanningUnit *pUnit = pSearch->m_PathPlanning.pGoalUnit;
	for (;;)
	{
		pos[0] = pInfo->pPoryLinkInfo[pUnit->prevPolyIndex].centerPosition;

		//後ろに向かうパス
		pUnit = &pSearch->m_PathPlanning.pUnitArray[pUnit->prevPolyIndex];
		if (pUnit->prevPolyIndex == -1) break;

		pos[1] = pInfo->pPoryLinkInfo[pUnit->prevPolyIndex].centerPosition;

		Fiqure::RenderLine3D(pos[0], pos[1], Vector3D(0.0f, 1.0f, 0.0f));
	}
}