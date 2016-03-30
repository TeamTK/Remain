#ifndef _COPMUTE_INFO_
#define _COPMUTE_INFO_

#include "../../System/Direct3D11.h"

/*****************************/
//線分とスタティックメッシュの当たり判定GPUの情報
/*****************************/

//シェーダーに送る情報三角形
struct TriangleInfo_GPU
{
	Vector3D vertexPos1;
	Vector3D vertexPos2;
	Vector3D vertexPos3;
	Vector3D normal;
	Vector3D contactPos;
	bool isHit;
};

//シェーダーに送る情報線分
struct LineBuffer
{
	Vector4D start;
	Vector4D end;
};
/*****************************/

/*****************************/
//球とスタティックメッシュの当たり判定GPUの情報
/*****************************/

//シェーダーに送る情報三角形（球版）
struct Sphere_TriangleInfo_GPU
{
	Vector3D vertexPos1;
	Vector3D vertexPos2;
	Vector3D vertexPos3;
	Vector3D normal;
	Vector3D contactPos;
	float dist;
	bool isHit;
};

//シェーダーに送る情報球
struct SphereBuffer
{
	Vector4D point;
	float radius;
};
/*****************************/

#endif