#ifndef _MESHBASE_H_
#define _MESHBASE_H_

#include "StaticMeshAsset.h"
#include "WorldMatrixManager.h"

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
	Vector3D GetScale() const;
	Vector3D GetRotation() const;
	Vector3D GetTranselate() const;
	Vector3D GetDiffuse(int materialIndex) const;
	Vector3D GetSpecular(int materialIndex) const;
	Vector3D GetAmbient(int materialIndex) const;
	Vector3D GetAxisX(float length) const;
	Vector3D GetAxisY(float length) const;
	Vector3D GetAxisZ(float length) const;
	const Matrix *GetMatrix() const;
	void DebugAxis();

protected:
	Matrix m_LocalMatrix;
	Matrix m_Matrix;
	Vector3D m_Rotation;
	Vector3D m_Scale;
	Vector3D m_Transelate;
	WorldMatrixInfo m_WorldMatrixInfo;
	std::vector<Vector4D> m_Diffuse;
	std::vector<Vector3D> m_Specular;
	std::vector<Vector3D> m_Ambient;
};

#endif