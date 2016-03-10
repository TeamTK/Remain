#ifndef _MESHBASE_H_
#define _MESHBASE_H_

#include "StaticMesh\StaticMeshAsset.h"
#include "WorldMatrixManager.h"

class Image;

//StaticMesh・DynamicMesh型の基底メッシュ二つの共通項目を定義
class MeshBase
{
public:
	MeshBase();
	~MeshBase();
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
	void SetTexture(Image *pImage);
	Vector3D GetScale() const;
	Vector3D GetRotation() const;
	Vector3D GetTranselate() const;
	Vector3D GetDiffuse(int materialIndex) const;
	Vector3D GetSpecular(int materialIndex) const;
	Vector3D GetAmbient(int materialIndex) const;
	Vector3D GetAxisX(float length) const;
	Vector3D GetAxisX(const Matrix &matrix, float length) const;
	Vector3D GetAxisY(float length) const;
	Vector3D GetAxisY(const Matrix &matrix, float length) const;
	Vector3D GetAxisZ(float length) const;
	Vector3D GetAxisZ(const Matrix &matrix, float length) const;
	const Matrix *GetWorldMatrix() const;
	const Matrix *GetSynthesisMatrix() const;
	void DebugAxis(const Matrix &matrix);
	void DebugAxis();

protected:
	Matrix m_WorldMatrix;			   //モデルのワールド行列
	Matrix m_SynthesisMatrix;		   //モデルのローカル × ワールド行列
	Vector3D m_Rotation;			   //モデルの回転値（X軸・Y軸・Z軸）
	Vector3D m_Scale;				   //モデルの拡大率（X軸・Y軸・Z軸）
	Vector3D m_Transelate;			   //モデルの平行移動（X軸・Y軸・Z軸）
	WorldMatrixInfo m_WorldMatrixInfo; //モデルの回転・拡大・平行移動を他の場所で計算するための情報
	std::vector<Vector4D> m_Diffuse;   //モデルの拡散反射率
	std::vector<Vector3D> m_Specular;  //モデルの鏡面反射率
	std::vector<Vector3D> m_Ambient;   //モデルの環境光率
	Image *m_pImage;				   //マルチテクスチャ適用の画像
};

#endif