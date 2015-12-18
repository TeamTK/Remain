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
	void SetRenewalTime(float animSpeed); //アニメーション速度更新
	void SetTime(float animTime);		  //指定のアニメーション時間に設定
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
	std::string m_buffer; //階層戻り時にボーン名格納
	int m_BornIndex;	  //ボーンのインデックスカウント
	unsigned int m_AnimSetNum;	  //現在のアニメーションセット
	int m_Back;			  //下の階層の番号
	float m_FrameAnim;	  //現在のアニメーション時間
};

#endif