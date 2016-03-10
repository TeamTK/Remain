#ifndef _DYNAMIC_MESH_H_
#define _DYNAMIC_MESH_H_

#include "..\MeshBase.h"
#include "DynamicMeshAsset.h"

class ShaderShadowMap;

//�A�j���[�V���������铮�I��3D���f����`��
class DynamicMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	DynamicMesh();
	DynamicMesh(const std::string &meshName, bool isLightInterrupted = false);
	~DynamicMesh();
	void SetAsset(const std::string &meshName, bool isLightInterrupted = false);
	void ChangeAnimation(unsigned int num); //�A�j���[�V������ύX
	void SetPlayTime(float animSpeed); //�A�j���[�V�������x�X�V
	void SetTime(float animTime);	   //�w��̃A�j���[�V�������Ԃɐݒ�
	void StartAnimation();
	void StopAnimation();
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
	unsigned int m_AnimNum;
	float m_AnimFrame;
	float m_AnimSpeed;
	DyanmicMeshData *m_pMeshData;
	CopyBorn m_Born;
	std::vector<CopyBorn*> m_CopyBornArray;
};

#endif