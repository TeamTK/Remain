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
	DynamicMesh(const std::string &meshName, bool isLightInterrupted = false);
	~DynamicMesh();
	void SetAsset(const std::string &meshName, bool isLightInterrupted = false);
	void ChangeAnimation(unsigned int num); //アニメーションを変更
	void SetPlayTime(float animSpeed); //アニメーション速度更新
	void SetTime(float animTime);	   //指定のアニメーション時間に設定
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