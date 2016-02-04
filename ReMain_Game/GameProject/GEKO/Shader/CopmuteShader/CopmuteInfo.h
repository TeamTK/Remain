#ifndef _COPMUTE_INFO_
#define _COPMUTE_INFO_

#include "../../System/Direct3D11.h"

/*****************************/
//�����ƃX�^�e�B�b�N���b�V���̓����蔻��GPU�̏��
/*****************************/

//�V�F�[�_�[�ɑ�����O�p�`
struct TriangleInfo_GPU
{
	D3DXVECTOR3 vertexPos1;
	D3DXVECTOR3 vertexPos2;
	D3DXVECTOR3 vertexPos3;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 contactPos;
	bool isHit;
};

//�V�F�[�_�[�ɑ��������
struct LineBuffer
{
	D3DXVECTOR4 start;
	D3DXVECTOR4 end;
};
/*****************************/

/*****************************/
//���ƃX�^�e�B�b�N���b�V���̓����蔻��GPU�̏��
/*****************************/

//�V�F�[�_�[�ɑ�����O�p�`�i���Łj
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

//�V�F�[�_�[�ɑ�����
struct SphereBuffer
{
	D3DXVECTOR4 point;
	float radius;
};
/*****************************/

#endif