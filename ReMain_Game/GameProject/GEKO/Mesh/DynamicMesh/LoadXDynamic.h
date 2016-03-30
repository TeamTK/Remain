#ifndef _LOAD_X_DYNAMIC_H_
#define _LOAD_X_DYNAMIC_H_

#include "../MeshData.h"

class LoadXDynamic : public DyanmicMeshData
{
public:
	LoadXDynamic(std::string fileName);
	~LoadXDynamic();
	void CopyBoneTree(CopyBone *pBoneCopy, std::vector<CopyBone*> *pCopyBoneArray, Bone *pBoneOriginal);
	void Update(CopyBone *pCopyBone, unsigned int animNum, float *animFrame, bool *pIsAnimEnd);

private:
	HRESULT LoadXMesh(std::string fileName);
	void LoadAnimation(FILE *fp, SkinVertexInfo* pVB);
	void LoadMat(Bone *pBone, FILE *fp);
	void BoneMatUpdate(CopyBone *pCopyBone, Bone *pBone, Matrix &boneMat);
	void AnimUpdate(CopyBone *pCopyBone, Bone *pBone, int animNum, float animFrame, unsigned int frameAnimNum);
	bool AddBoneHierarchy(Bone *pBone, FILE *fp, int hierarchy);

private:
	std::string m_buffer; //�K�w�߂莞�Ƀ{�[�����i�[
	int m_BoneIndex;	  //�{�[���̃C���f�b�N�X�J�E���g
	int m_Back;			  //���̊K�w�̔ԍ�
};

#endif