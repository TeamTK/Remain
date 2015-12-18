#ifndef _LOAD_X_DYNAMIC_H_
#define _LOAD_X_DYNAMIC_H_

#include "SkinMeshData.h"

class LoadXDynamic : public SkinMeshData
{
public:
	LoadXDynamic(std::string fileName);
	~LoadXDynamic();
	void Relese();
	void ChangeAnimation(unsigned int num);
	void SetRenewalTime(float animSpeed); //�A�j���[�V�������x�X�V
	void SetTime(float animTime);		  //�w��̃A�j���[�V�������Ԃɐݒ�
	float GetPlayTime();
	int GetPlayAnimation();
	int GetBornAllNum();
	std::string GetBornName(int bornIndex);
	Matrix GetBornWorld(int bornIndex);
	Vector3D GetBornPos(int bornIndex);
	void Update();
	void BornDebug(eBornDebug eBornDebug);
	void AnimationDebug(int animNum);

private:
	HRESULT LoadXMesh(std::string fileName);
	void LoadAnimation(std::string fileName, SkinVertexInfo* pVB);
	void LoadMat(Born *pBorn, std::ifstream *pIfs);
	void BornMatUpdate(Born *pBorn, D3DXMATRIX &bornMat);
	void AnimUpdate(Born *pBorn);
	void DeleteHierarchy(Born *pBorn);
	bool AddBoneHierarchy(Born *pBorn, std::ifstream *pIfs, int hierarchy);

private:
	std::string m_buffer; //�K�w�߂莞�Ƀ{�[�����i�[
	int m_BornIndex;	  //�{�[���̃C���f�b�N�X�J�E���g
	unsigned int m_AnimSetNum;	  //���݂̃A�j���[�V�����Z�b�g
	int m_Back;			  //���̊K�w�̔ԍ�
	float m_FrameAnim;	  //���݂̃A�j���[�V��������
};

#endif