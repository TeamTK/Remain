#ifndef _LOAD_X_DYNAMIC_H_
#define _LOAD_X_DYNAMIC_H_

#include "../MeshData.h"

class LoadXDynamic : public DyanmicMeshData
{
public:
	LoadXDynamic(std::string fileName);
	~LoadXDynamic();
	void CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal);
	void Update(CopyBorn *pCopyBorn, unsigned int animNum, float *animFrame, bool *pIsAnimEnd);

private:
	HRESULT LoadXMesh(std::string fileName);
	void LoadAnimation(FILE *fp, SkinVertexInfo* pVB);
	void LoadMat(Born *pBorn, FILE *fp);
	void BornMatUpdate(CopyBorn *pCopyBorn, Born *pBorn, Matrix &bornMat);
	void AnimUpdate(CopyBorn *pCopyBorn, Born *pBorn, int animNum, float animFrame, unsigned int frameAnimNum);
	bool AddBoneHierarchy(Born *pBorn, FILE *fp, int hierarchy);

private:
	std::string m_buffer; //�K�w�߂莞�Ƀ{�[�����i�[
	int m_BornIndex;	  //�{�[���̃C���f�b�N�X�J�E���g
	int m_Back;			  //���̊K�w�̔ԍ�
};

#endif