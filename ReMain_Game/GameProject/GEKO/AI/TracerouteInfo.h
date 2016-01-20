#ifndef _TRACEROUTE_INFO_H_
#define _TRACEROUTE_INFO_H_

#include "..\System\Math.h"
#include "..\Mesh\StaticMesh.h"

struct PolyLinkInfo
{
	int linkPolyIndex[3];
	float linkPolyDistance[3];
	Vector3D centerPosition;
};

struct TracerouteInfo
{
	StaticMesh *pStaticMesh;
	PolyLinkInfo *pPoryLinkInfo;

	TracerouteInfo() :
		pStaticMesh(nullptr),
		pPoryLinkInfo(nullptr){};
};

#endif