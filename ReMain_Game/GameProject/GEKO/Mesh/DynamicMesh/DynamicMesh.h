#ifndef _DYNAMIC_MESH_H_
#define _DYNAMIC_MESH_H_

#include "..\MeshBase.h"
#include "DynamicMeshAsset.h"

class ShaderShadowMap;

//アニメーションをする動的な3Dモデルを描画
class DynamicMesh : public MeshBase
{
public:
	friend ShaderShadowMap;

	DynamicMesh();
	DynamicMesh(const std::string &meshName, unsigned int priorityGroup, unsigned int priority, unsigned int meshState = MeshState::eNothing);
	~DynamicMesh();
	void SetMeshState(unsigned int meshState);
	void SetAsset(const std::string &meshName);
	void ChangeAnimation(unsigned int num); //アニメーションを変更
	void SetPlayTime(float animSpeed); //アニメーション速度更新
	void SetTime(float animTime);	   //指定のアニメーション時間に設定
	void StartAnimation();
	void StopAnimation();
	const SkinVertexInfo *GetVertex() const;
	float GetPlayTime() const;
	int GetPlayAnimation() const;
	int GetFaceAllNum() const;
	int GetBoneNum(std::string name) const;
	int GetBoneAllNum() const;
	std::string GetBoneName(int boneIndex) const;
	Matrix GetBoneMatrix(int boneIndex, bool isWorld) const;
	Matrix GetBoneMatrix(std::string name, bool isWorld) const;
	Vector3D GetBonePos(int boneIndex) const;
	Vector3D GetBonePos(std::string name) const;
	void RenderOutline(float size);
	void BoneDebug(eBoneDebug eBoneDebug) const;
	void AnimationDebug(int animNum) const;

private:
	void ModelBoneBuilding();
	void ForwardRendering();
	void DeferredRendering();
	void AllocationSkinMeshData(const std::string &meshName);
	void RenderFunc(Matrix &matrix, bool isShadow);
	void ReleseCopyBoneTree(CopyBone *pBoneCopy) const;

private:
	bool m_IsAnimUpdate;
	bool m_IsAnimEnd;
	unsigned int m_AnimNum;
	float m_AnimFrame;
	DyanmicMeshData *m_pMeshData;
	CopyBone m_Bone;
	std::vector<CopyBone*> m_CopyBoneArray;
	BoneMatrixFuncInfo m_BoneMatrixFuncInfo;
};

#endif