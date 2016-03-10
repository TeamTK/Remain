#ifndef _TRACEROUTE_INFO_H_
#define _TRACEROUTE_INFO_H_

#include "..\System\Math.h"
#include "..\Mesh\StaticMesh\StaticMesh.h"
#include "..\Shader\CopmuteShader\LineTriangleCopmute.h"

class LineTriangleCopmute;

//�|���S���̗אڏ��
struct PolyLinkInfo
{
	int linkPolyIndex[3];		 //�אڂ��Ă���|���S���ԍ�
	float linkPolyDistance[3]; //�אڂ��Ă���|���S���̒��S���W�܂ł̋���
	Vector3D centerPosition;	 //�|���S���̒��S���W

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

//�o�H�T�������p��1�|���S���̏��
struct PathPlanningUnit
{
	int polyIndex;						 //�|���S���ԍ�
	int prevPolyIndex;					 //�o�H�T���Ŋm�肵���o�H��̈�O�̃|���S��(���|���S�����o�H��ɖ�����-1)
	int NextPolyIndex;					 //�o�H�T���Ŋm�肵���o�H��̈��̃|���S��(���|���S�����o�H��ɖ�����-1)
	float totalDistance;				 //�o�H�T���ł��̃|���S���ɓ��B����܂łɒʉ߂����|���S���Ԃ̋����̍��v
	PathPlanningUnit *pActiveNextUnit; //�o�H�T�������ΏۂɂȂ��Ă��鎟�̃|���S���̃������A�h���X���i�[����ϐ�
										 
	PathPlanningUnit() :
		polyIndex(-1),
		prevPolyIndex(-1),
		NextPolyIndex(-1),
		totalDistance(0.0f),
		pActiveNextUnit(nullptr) {};
};

//�o�H�T�������Ŏg�p�������ۑ�����ׂ̍\����
struct PathPlanning
{
	PathPlanningUnit *pUnitArray;		  //�o�H�T�������Ŏg�p����S�|���S���̏��z�񂪊i�[���ꂽ�������̈�̐擪�������A�h���X���i�[����ϐ�
	PathPlanningUnit *pActiveFirstUnit; //�o�H�T�������ΏۂɂȂ��Ă���|���S���Q�̍ŏ��̃|���S�����ւ̃������A�h���X���i�[����ϐ�
	PathPlanningUnit *pStartUnit;		  //�o�H�̃X�^�[�g�n�_�ɂ���|���S�����ւ̃������A�h���X���i�[����ϐ�
	PathPlanningUnit *pGoalUnit;		  //�o�H�̃S�[���n�_�ɂ���|���S�����ւ̃������A�h���X���i�[����ϐ�

	PathPlanning() :
		pUnitArray(nullptr),
		pActiveFirstUnit(nullptr),
		pStartUnit(nullptr),
		pGoalUnit(nullptr){};
};

//�T�������o�H���ړ����鏈���Ɏg�p�������Z�߂��\����
struct PathMoveInfo
{
	int nowPolyIndex;						      //���ݏ���Ă���|���S���̔ԍ�
	const Vector3D *pNowPosition;				  //���݈ʒu
	Vector3D MoveDirection;					  //�ړ�����
	PathPlanningUnit *pNowPathPlanningUnit;	  //���ݏ���Ă���|���S���̌o�H�T����񂪊i�[����Ă��郁�����A�h���X���i�[����ϐ�
	PathPlanningUnit *pTargetPathPlanningUnit; //���̒��Ԓn�_�ƂȂ�o�H��̃|���S���̌o�H�T����񂪊i�[����Ă��郁�����A�h���X���i�[����ϐ�
};

//�o�H���
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