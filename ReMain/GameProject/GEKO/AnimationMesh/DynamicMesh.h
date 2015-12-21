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
	void ChangeAnimation(unsigned int num);
	void SetPlayTime(float animSpeed); //�A�j���[�V�������x�X�V
	void SetTime(float animTime);	   //�w��̃A�j���[�V�������Ԃɐݒ�
	SkinVertexInfo *GetVertex();
	float GetPlayTime();
	int GetPlayAnimation();
	int GetFaceAllNum();
	int GetBornNum(std::string name);
	int GetBornAllNum();
	std::string GetBornName(int bornIndex);
	Matrix GetBornMatrix(int bornIndex, bool isWorld);
	Matrix GetBornMatrix(std::string name, bool isWorld);
	Vector3D GetBornPos(int bornIndex);
	Vector3D GetBornPos(std::string name);
	void Render();
	void RenderMatrix(Matrix &matrix);
	void BornDebug(eBornDebug eBornDebug);
	void AnimationDebug(int animNum);

private:
	void RenderFunc(Matrix &matrix);

private:
	SkinMeshData *m_pSkinMeshData;
	float m_AnimTime;
};

#endif