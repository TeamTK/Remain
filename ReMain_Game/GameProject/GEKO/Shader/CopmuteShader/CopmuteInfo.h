#ifndef _COPMUTE_INFO_
#define _COPMUTE_INFO_

#include "../../System/Direct3D11.h"

/*****************************/
//線分とスタティックメッシュの当たり判定GPUの情報
/*****************************/

//シェーダーに送る情報三角形
struct TriangleInfo_GPU
{
	D3DXVECTOR3 vertexPos1;
	D3DXVECTOR3 vertexPos2;
	D3DXVECTOR3 vertexPos3;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 contactPos;
	bool isHit;
};

//シェーダーに送る情報線分
struct LineBuffer
{
	D3DXVECTOR4 start;
	D3DXVECTOR4 end;
};
/*****************************/

/*****************************/
//球とスタティックメッシュの当たり判定GPUの情報
/*****************************/

//シェーダーに送る情報三角形（球版）
struct Sphere_TriangleInfo_GPU
{
	D3DXVECTOR3 vertexPos1;
	D3DXVECTOR3 vertexPos2;
	D3DXVECTOR3 vertexPos3;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 contactPos;
	float dist;
	bool isHit;
};

//シェーダーに送る情報球
struct SphereBuffer
{
	D3DXVECTOR4 point;
	float radius;
};
/*****************************/

#endif