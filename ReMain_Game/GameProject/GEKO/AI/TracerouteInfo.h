#ifndef _TRACEROUTE_INFO_H_
#define _TRACEROUTE_INFO_H_

#include "..\System\Math.h"
#include "..\Mesh\StaticMesh.h"

//�|���S���̗אڏ��
struct PolyLinkInfo
{
	int linkPolyIndex[3];		 //�אڂ��Ă���|���S���ԍ�
	float linkPolyDistance[3]; //�אڂ��Ă���|���S���̒��S���W�܂ł̋���
	Vector3D centerPosition;	 //�|���S���̒��S���W
};

//�o�H�T�������p��1�|���S���̏��
struct PathPlanningUnit
{
	int polyIndex;						 //�|���S���ԍ�
	int prevPolyIndex;					 //�o�H�T���ł��̃|���S���ɓ��B����܂łɒʉ߂����|���S���Ԃ̋����̍��v
	int NextPolyIndex;					 //�o�H�T���Ŋm�肵���o�H��̈�O�̃|���S��( ���|���S�����o�H��ɖ�����
	float totalDistance;				 //�o�H�T���Ŋm�肵���o�H��̈��̃|���S��( ���|���S�����o�H��ɖ�����
	PathPlanningUnit *pActiveNextUnit; //�o�H�T�������ΏۂɂȂ��Ă��鎟�̃|���S���̃������A�h���X���i�[����ϐ�
};

//�o�H�T�������Ŏg�p�������ۑ�����ׂ̍\����
struct PathPlanning
{
	PathPlanningUnit *pUnitArray;		  //�o�H�T�������Ŏg�p����S�|���S���̏��z�񂪊i�[���ꂽ�������̈�̐擪�������A�h���X���i�[����ϐ�
	PathPlanningUnit *pActiveFirstUnit; //�o�H�T�������ΏۂɂȂ��Ă���|���S���Q�̍ŏ��̃|���S�����ւ̃������A�h���X���i�[����ϐ�
	PathPlanningUnit *pStartUnit;		  //�o�H�̃X�^�[�g�n�_�ɂ���|���S�����ւ̃������A�h���X���i�[����ϐ�
	PathPlanningUnit *pGoalUnit;		  //�o�H�̃S�[���n�_�ɂ���|���S�����ւ̃������A�h���X���i�[����ϐ�
};

struct PathMoveInfo
{
	int nowPolyIndex;							  //���ݏ���Ă���|���S���̔ԍ�
	Vector3D nowPosition;						  //���݈ʒu
	Vector3D MoveDirection;					  //�ړ�����
	PathPlanningUnit *pNowPathPlanningUnit;	  //���ݏ���Ă���|���S���̌o�H�T����񂪊i�[����Ă��郁�����A�h���X���i�[����ϐ�
	PathPlanningUnit *pTargetPathPlanningUnit; //���̒��Ԓn�_�ƂȂ�o�H��̃|���S���̌o�H�T����񂪊i�[����Ă��郁�����A�h���X���i�[����ϐ�
};

//�o�H���
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