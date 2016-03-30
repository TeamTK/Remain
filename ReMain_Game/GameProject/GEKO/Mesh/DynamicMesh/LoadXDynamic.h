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
	std::string m_buffer; //階層戻り時にボーン名格納
	int m_BoneIndex;	  //ボーンのインデックスカウント
	int m_Back;			  //下の階層の番号
};

#endif