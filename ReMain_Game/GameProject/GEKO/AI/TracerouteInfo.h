#ifndef _TRACEROUTE_INFO_H_
#define _TRACEROUTE_INFO_H_

#include "..\System\Math.h"
#include "..\Mesh\StaticMesh.h"

//ポリゴンの隣接情報
struct PolyLinkInfo
{
	int linkPolyIndex[3];		 //隣接しているポリゴン番号
	float linkPolyDistance[3]; //隣接しているポリゴンの中心座標までの距離
	Vector3D centerPosition;	 //ポリゴンの中心座標
};

//経路探索処理用の1ポリゴンの情報
struct PathPlanningUnit
{
	int polyIndex;						 //ポリゴン番号
	int prevPolyIndex;					 //経路探索でこのポリゴンに到達するまでに通過したポリゴン間の距離の合計
	int NextPolyIndex;					 //経路探索で確定した経路上の一つ前のポリゴン( 当ポリゴンが経路上に無い場
	float totalDistance;				 //経路探索で確定した経路上の一つ先のポリゴン( 当ポリゴンが経路上に無い場
	PathPlanningUnit *pActiveNextUnit; //経路探索処理対象になっている次のポリゴンのメモリアドレスを格納する変数
};

//経路探索処理で使用する情報を保存する為の構造体
struct PathPlanning
{
	PathPlanningUnit *pUnitArray;		  //経路探索処理で使用する全ポリゴンの情報配列が格納されたメモリ領域の先頭メモリアドレスを格納する変数
	PathPlanningUnit *pActiveFirstUnit; //経路探索処理対象になっているポリゴン群の最初のポリゴン情報へのメモリアドレスを格納する変数
	PathPlanningUnit *pStartUnit;		  //経路のスタート地点にあるポリゴン情報へのメモリアドレスを格納する変数
	PathPlanningUnit *pGoalUnit;		  //経路のゴール地点にあるポリゴン情報へのメモリアドレスを格納する変数
};

struct PathMoveInfo
{
	int nowPolyIndex;							  //現在乗っているポリゴンの番号
	Vector3D nowPosition;						  //現在位置
	Vector3D MoveDirection;					  //移動方向
	PathPlanningUnit *pNowPathPlanningUnit;	  //現在乗っているポリゴンの経路探索情報が格納されているメモリアドレスを格納する変数
	PathPlanningUnit *pTargetPathPlanningUnit; //次の中間地点となる経路上のポリゴンの経路探索情報が格納されているメモリアドレスを格納する変数
};

//経路情報
struct TracerouteInfo
{
	StaticMesh *pStaticMesh;
	PolyLinkInfo *pPoryLinkInfo;
	PathPlanning pathPlanning;

	TracerouteInfo() :
		pStaticMesh(nullptr),
		pPoryLinkInfo(nullptr) {};
};

#endif