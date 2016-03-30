#ifndef _COPMUTE_INFO_
#define _COPMUTE_INFO_

#include "../../System/Direct3D11.h"

/*****************************/
//�����ƃX�^�e�B�b�N���b�V���̓����蔻��GPU�̏��
/*****************************/

//�V�F�[�_�[�ɑ�����O�p�`
struct TriangleInfo_GPU
{
	Vector3D vertexPos1;
	Vector3D vertexPos2;
	Vector3D vertexPos3;
	Vector3D normal;
	Vector3D contactPos;
	bool isHit;
};

//�V�F�[�_�[�ɑ��������
struct LineBuffer
{
	Vector4D start;
	Vector4D end;
};
/*****************************/

/*****************************/
//���ƃX�^�e�B�b�N���b�V���̓����蔻��GPU�̏��
/*****************************/

//�V�F�[�_�[�ɑ�����O�p�`�i���Łj
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

//�V�F�[�_�[�ɑ�����
struct SphereBuffer
{
	Vector4D point;
	float radius;
};
/*****************************/

#endif