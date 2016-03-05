#ifndef _DYNAMIC_MESH_H_
#define _DYNAMIC_MESH_H_

#include "..\Mesh\MeshBase.h"
#include "DynamicMeshAsset.h"

class ShaderShadowMap;

class DynamicMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	DynamicMesh();
	DynamicMesh(const std::string &meshName, bool isLightInterrupted = false);
	~DynamicMesh();
	void SetAsset(const std::string &meshName, bool isLightInterrupted = false);
	void ChangeAnimation(unsigned int num); //�A�j���[�V������ύX
	void PartChangeAnimation(int bornIndex, unsigned int num); //�A�j���[�V������ύX
	void PartRangeChangeAnimation(int bornStart, int bornEnd, unsigned int num); //�A�j���[�V������ύX
	void SetPlayTime(float animSpeed); //�A�j���[�V�������x�X�V
	void SetPartPlayTime(int bornIndex, float animSpeed); //�A�j���[�V�������x�X�V
	void SetPartRangePlayTime(int bornStart, int bornEnd, float animSpeed); //�A�j���[�V�������x�X�V
	void SetTime(float animTime);	   //�w��̃A�j���[�V�������Ԃɐݒ�
	void SetPartTime(int bornIndex, float animTime);	   //�w��̃A�j���[�V�������Ԃɐݒ�
	void SetPartRangeTime(int bornStart, int bornEnd, float animTime);	   //�w��̃A�j���[�V�������Ԃɐݒ�
	void StartAnimation();
	void StopAnimation();
	const SkinVertexInfo *GetVertex() const; 
	float GetPlayTime(int bornIndex) const;
	int GetPlayAnimation(int bornIndex) const;
	int GetFaceAllNum() const;
	int GetBornNum(std::string name) const;
	int GetBornAllNum() const;
	std::string GetBornName(int bornIndex) const;
	Matrix GetBornMatrix(int bornIndex, bool isWorld) const;
	Matrix GetBornMatrix(std::string name, bool isWorld) const;
	Vector3D GetBornPos(int bornIndex) const;
	Vector3D GetBornPos(std::string name) const;
	void Render();
	void RenderOutline(float size);
	void RenderMatrix(Matrix &matrix);
	void BornDebug(eBornDebug eBornDebug) const;
	void AnimationDebug(int animNum) const;

private:
	void AllocationSkinMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix);
	void ReleseCopyBornTree(CopyBorn *pBornCopy) const;

private:
	bool m_IsAnimUpdate;
	bool m_IsAnimEnd;
	float m_AnimSpeed;
	SkinMeshData *m_pSkinMeshData;
	CopyBorn m_Born;
	std::vector<CopyBorn*> m_CopyBornArray;
};

#endif