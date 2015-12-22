#ifndef _DYNAMIC_MESH_H_
#define _DYNAMIC_MESH_H_

#include "..\Mesh\MeshBase.h"
#include "DynamicMeshAsset.h"

class DynamicMesh : public MeshBase
{
public:
	DynamicMesh();
	~DynamicMesh();
	void SetAsset(SkinMeshData *meshData);
	void SetAsset(const std::string &MeshName);
	void ChangeAnimation(unsigned int num); //アニメーションを変更
	void SetPlayTime(float animSpeed); //アニメーション速度更新
	void SetTime(float animTime);	   //指定のアニメーション時間に設定
	const SkinVertexInfo *GetVertex() const; 
	float GetPlayTime() const;
	int GetPlayAnimation() const;
	int GetFaceAllNum() const;
	int GetBornNum(std::string name) const;
	int GetBornAllNum() const;
	std::string GetBornName(int bornIndex) const;
	Matrix GetBornMatrix(int bornIndex, bool isWorld) const;
	Matrix GetBornMatrix(std::string name, bool isWorld) const;
	Vector3D GetBornPos(int bornIndex) const;
	Vector3D GetBornPos(std::string name) const;
	void Render();
	void RenderMatrix(Matrix &matrix);
	void BornDebug(eBornDebug eBornDebug);
	void AnimationDebug(int animNum);

private:
	void RenderFunc(Matrix &matrix);
	void ReleseCopyBornTree(CopyBorn *pBornCopy);

private:
	SkinMeshData *m_pSkinMeshData;
	CopyBorn m_Born;
	std::vector<CopyBorn*> m_CopyBornArray;
	float m_AinmFrame;
	int m_AinmNum;
};

#endif