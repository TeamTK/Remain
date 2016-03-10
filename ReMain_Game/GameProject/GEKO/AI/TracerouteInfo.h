#ifndef _TRACEROUTE_INFO_H_
#define _TRACEROUTE_INFO_H_

#include "..\System\Math.h"
#include "..\Mesh\StaticMesh\StaticMesh.h"
#include "..\Shader\CopmuteShader\LineTriangleCopmute.h"

class LineTriangleCopmute;

//ポリゴンの隣接情報
struct PolyLinkInfo
{
	int linkPolyIndex[3];		 //隣接しているポリゴン番号
	float linkPolyDistance[3]; //隣接しているポリゴンの中心座標までの距離
	Vector3D centerPosition;	 //ポリゴンの中心座標

	PolyLinkInfo()
	{
		linkPolyIndex[0] = -1;
		linkPolyIndex[1] = -1;
		linkPolyIndex[2] = -1;

		linkPolyDistance[0] = 0.0f;
		linkPolyDistance[1] = 0.0f;
		linkPolyDistance[2] = 0.0f;
	}
};

//経路探索処理用の1ポリゴンの情報
struct PathPlanningUnit
{
	int polyIndex;						 //ポリゴン番号
	int prevPolyIndex;					 //経路探索で確定した経路上の一つ前のポリゴン(当ポリゴンが経路上に無い場-1)
	int NextPolyIndex;					 //経路探索で確定した経路上の一つ先のポリゴン(当ポリゴンが経路上に無い場-1)
	float totalDistance;				 //経路探索でこのポリゴンに到達するまでに通過したポリゴン間の距離の合計
	PathPlanningUnit *pActiveNextUnit; //経路探索処理対象になっている次のポリゴンのメモリアドレスを格納する変数
										 
	PathPlanningUnit() :
		polyIndex(-1),
		prevPolyIndex(-1),
		NextPolyIndex(-1),
		totalDistance(0.0f),
		pActiveNextUnit(nullptr) {};
};

//経路探索処理で使用する情報を保存する為の構造体
struct PathPlanning
{
	PathPlanningUnit *pUnitArray;		  //経路探索処理で使用する全ポリゴンの情報配列が格納されたメモリ領域の先頭メモリアドレスを格納する変数
	PathPlanningUnit *pActiveFirstUnit; //経路探索処理対象になっているポリゴン群の最初のポリゴン情報へのメモリアドレスを格納する変数
	PathPlanningUnit *pStartUnit;		  //経路のスタート地点にあるポリゴン情報へのメモリアドレスを格納する変数
	PathPlanningUnit *pGoalUnit;		  //経路のゴール地点にあるポリゴン情報へのメモリアドレスを格納する変数

	PathPlanning() :
		pUnitArray(nullptr),
		pActiveFirstUnit(nullptr),
		pStartUnit(nullptr),
		pGoalUnit(nullptr){};
};

//探索した経路を移動する処理に使用する情報を纏めた構造体
struct PathMoveInfo
{
	int nowPolyIndex;						      //現在乗っているポリゴンの番号
	const Vector3D *pNowPosition;				  //現在位置
	Vector3D MoveDirection;					  //移動方向
	PathPlanningUnit *pNowPathPlanningUnit;	  //現在乗っているポリゴンの経路探索情報が格納されているメモリアドレスを格納する変数
	PathPlanningUnit *pTargetPathPlanningUnit; //次の中間地点となる経路上のポリゴンの経路探索情報が格納されているメモリアドレスを格納する変数
};

//経路情報
struct TracerouteInfo
{
	Vector3D *pNormal;
	StaticMesh *pStaticMesh;
	PolyLinkInfo *pPoryLinkInfo;
	LineTriangleCopmute copmute;
	bool isGPU;

	TracerouteInfo() :
		pNormal(nullptr),
		pStaticMesh(nullptr),
		pPoryLinkInfo(nullptr),
		isGPU(false){};
};

#endif