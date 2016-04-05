#ifndef _MESH_BASE_H_
#define _MESH_BASE_H_

#include "StaticMesh\StaticMeshAsset.h"
#include "WorldMatrixManager.h"
#include "../Shader/RenderingManager.h"

struct ImageInfo;

//StaticMesh・DynamicMesh型の基底メッシュ二つの共通項目を定義
class MeshBase
{
public:
	MeshBase(bool isRegister, unsigned int priorityGroup, unsigned int priority, unsigned int meshState);
	virtual ~MeshBase();
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetScale(float x, float y, float z);
	void SetRotationRadian(float x, float y, float z);
	void SetRotationDegree(int x, int y, int z);
	void SetTranselate(float x, float y, float z);
	void SetTranselate(const Vector3D &pos);
	void SetDiffuse(int r, int g, int b, int materialIndex);
	void SetSpecular(int r, int g, int b, int materialIndex);
	void SetAmbient(int r, int g, int b, int materialIndex);
	void SetAlpha(int alpha, int materialIndex);
	void SetAlphaAll(int alpha);
	void SetTexture(const std::string &assetName, bool isTexture = true);
	void SetModelMatrixBuilding();
	void SetParentMatirx(const Matrix &parentMatrix);
	Vector3D GetScale() const;
	Vector3D GetRotation() const;
	Vector3D GetTranselate() const;
	Vector3D GetDiffuse(int materialIndex) const;
	Vector3D GetSpecular(int materialIndex) const;
	Vector3D GetAmbient(int materialIndex) const;
	Vector3D GetAxisX(MatrixType matrixType) const;
	Vector3D GetAxisY(MatrixType matrixType) const;
	Vector3D GetAxisZ(MatrixType matrixType) const;
	const Matrix *GetLocalMatrix() const;
	const Matrix *GetWorldMatrix() const;
	const Matrix *GetModelMatrix() const;
	void DebugAxis(MatrixType matrixType);

protected:
	virtual void ForwardRendering();
	virtual void DeferredRendering();

protected:
	bool m_IsRenderingRegister;	   //レンダリング登録判断
	unsigned int m_MeshState;		   //モデルの状態を判断
	Matrix *m_pLocalMatrix;		   //モデルのローカル行列へのポインタ
	Matrix m_WorldMatrix;			   //モデルのワールド行列
	Matrix m_ModelMatrix;		       //モデルのローカル × ワールド行列
	const Matrix *m_pParentMatirx;	   //親のモデル行列
	Vector3D m_Rotation;			   //モデルの回転値（X軸・Y軸・Z軸）
	Vector3D m_Scale;				   //モデルの拡大率（X軸・Y軸・Z軸）
	Vector3D m_Transelate;			   //モデルの平行移動（X軸・Y軸・Z軸）
	WorldMatrixInfo m_WorldMatrixInfo; //モデルの回転・拡大・平行移動を他の場所で計算するための情報
	std::vector<Vector4D> m_Diffuse;   //モデルの拡散反射率
	std::vector<Vector3D> m_Specular;  //モデルの鏡面反射率
	std::vector<Vector3D> m_Ambient;   //モデルの環境光率
	ImageInfo *m_pImageInfo;		   //マルチテクスチャ適用の画像情報へのポインタ
	RenderingType m_RenderingType;	   //レンダリングの種類
	RenderingInfo m_RenderingInfo;	   //レンダリング管理情報
};

#endif