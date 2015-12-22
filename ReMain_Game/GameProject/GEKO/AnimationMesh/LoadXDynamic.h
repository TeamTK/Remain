#ifndef _LOAD_X_DYNAMIC_H_
#define _LOAD_X_DYNAMIC_H_

#include "SkinMeshData.h"

class LoadXDynamic : public SkinMeshData
{
public:
	LoadXDynamic(std::string fileName);
	~LoadXDynamic();
	void Relese();
	void CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal);
	int GetBornAllNum();
	std::string GetBornName(int bornIndex);
	void Update(CopyBorn *pCopyBorn, float *pAinmFrame, int *pAinmNum);
	void BornDebug(eBornDebug eBornDebug);
	void AnimationDebug(int animNum);

private:
	HRESULT LoadXMesh(std::string fileName);
	void LoadAnimation(std::string fileName, SkinVertexInfo* pVB);
	void LoadMat(Born *pBorn, std::ifstream *pIfs);
	void BornMatUpdate(CopyBorn *pCopyBorn, Born *pBorn, D3DXMATRIX &bornMat);
	void AnimUpdate(CopyBorn *pCopyBorn, Born *pBorn, float ainmFrame, unsigned int ainmNum, std::vector<int> *pFrameNum);
	void DeleteHierarchy(Born *pBorn);
	bool AddBoneHierarchy(Born *pBorn, std::ifstream *pIfs, int hierarchy);

private:
	std::string m_buffer; //階層戻り時にボーン名格納
	int m_BornIndex;	  //ボーンのインデックスカウント
	int m_Back;			  //下の階層の番号
};

#endif