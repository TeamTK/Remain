#include "MeshBase.h"
#include "..\System\Math.h"
#include "..\Figure\Fiqure.h"
#include <cassert>

MeshBase::MeshBase() :
	m_Scale(1.0f, 1.0f, 1.0f)
{
	m_WorldMatrixInfo.pMatrix = &m_Matrix;
	m_WorldMatrixInfo.pRotation = &m_Rotation;
	m_WorldMatrixInfo.pScale = &m_Scale;
	m_WorldMatrixInfo.pTranselate = &m_Transelate;
	WorldMatrixManager::GetInstance()->Add(&m_WorldMatrixInfo);
}

MeshBase::~MeshBase()
{
	//マテリアル消去
	m_Diffuse.shrink_to_fit();
	m_Specular.shrink_to_fit();
	m_Ambient.shrink_to_fit();
	m_Diffuse.clear();
	m_Specular.clear();
	m_Specular.clear();

	m_WorldMatrixInfo.pMatrix = nullptr;
	m_WorldMatrixInfo.pRotation = nullptr;
	m_WorldMatrixInfo.pScale = nullptr;
	m_WorldMatrixInfo.pTranselate = nullptr;
	WorldMatrixManager::GetInstance()->Clear();
}

void MeshBase::SetRotationRadian(float x, float y, float z)
{
	m_Rotation = Vector3D(x, y, z);
}

void MeshBase::SetRotationDegree(int x, int y, int z)
{
	m_Rotation = Vector3D(Math::ChangeToRadian((float)x),
						  Math::ChangeToRadian((float)y),
						  Math::ChangeToRadian((float)z));
}

void MeshBase::SetScale(float x, float y, float z)
{
	m_Scale = Vector3D(x, y, z);
}

void MeshBase::SetTranselate(float x, float y, float z)
{
	m_Transelate = Vector3D(x, y, z);
}

void MeshBase::SetTranselate(const Vector3D &pos)
{
	m_Transelate = pos;
}

void MeshBase::SetDiffuse(int r, int g, int b, int materialIndex)
{
	m_Diffuse[materialIndex].x = (float)r * RGB;
	m_Diffuse[materialIndex].y = (float)g * RGB;
	m_Diffuse[materialIndex].z = (float)b * RGB;
}

void MeshBase::SetSpecular(int r, int g, int b, int materialIndex)
{
	Vector3D specular = Vector3D((float)r * RGB, (float)g * RGB, (float)b * RGB);
	m_Specular[materialIndex] = specular;
}

void MeshBase::SetAmbient(int r, int g, int b, int materialIndex)
{
	Vector3D ambient = Vector3D((float)r * RGB, (float)g * RGB, (float)b * RGB);
	m_Ambient[materialIndex] = ambient;
}

void MeshBase::SetAlpha(int alpha, int materialIndex)
{
	m_Diffuse[materialIndex].w = ((float)alpha * RGB);
}

void MeshBase::SetAlphaAll(int alpha)
{
	float a = ((float)alpha * RGB);
	for (unsigned int i = 0; i < m_Diffuse.size(); i++) m_Diffuse[i].w = a;
}

Vector3D MeshBase::GetScale() const
{
	return m_Scale;
}

Vector3D MeshBase::GetRotation() const
{
	return m_Rotation;
}

Vector3D MeshBase::GetTranselate() const
{
	return m_Transelate;
}

Vector3D MeshBase::GetDiffuse(int materialIndex) const
{
	return Vector3D(m_Diffuse[materialIndex].x,
					m_Diffuse[materialIndex].y,
					m_Diffuse[materialIndex].z);
}

Vector3D MeshBase::GetSpecular(int materialIndex) const
{
	return m_Specular[materialIndex];
}

Vector3D MeshBase::GetAmbient(int materialIndex) const
{
	return m_Ambient[materialIndex];
}

Vector3D MeshBase::GetAxisX(float length) const
{
	return Vector3D::Matrix3x3(Vector3D(1.0f, 0.0f, 0.0f), m_Matrix) * length;
}

Vector3D MeshBase::GetAxisX(const Matrix &matrix, float length) const
{
	return Vector3D::Matrix3x3(Vector3D(1.0f, 0.0f, 0.0f), matrix) * length;
}

Vector3D MeshBase::GetAxisY(float length) const
{
	return Vector3D::Matrix3x3(Vector3D(0.0f, 1.0f, 0.0f), m_Matrix) * length;
}

Vector3D MeshBase::GetAxisY(const Matrix &matrix, float length) const
{
	return Vector3D::Matrix3x3(Vector3D(0.0f, 1.0f, 0.0f), matrix) * length;
}

Vector3D MeshBase::GetAxisZ(float length) const
{
	return Vector3D::Matrix3x3(Vector3D(0.0f, 0.0f, 1.0f), m_Matrix) * length;
}

Vector3D MeshBase::GetAxisZ(const Matrix &matrix, float length) const
{
	return Vector3D::Matrix3x3(Vector3D(0.0f, 0.0f, 1.0f), matrix) * length;
}

const Matrix *MeshBase::GetWorldMatrix() const
{
	return &m_Matrix;
}

void MeshBase::DebugAxis()
{
	Vector3D x_Normal(1.0f, 0.0f, 0.0f);
	Vector3D y_Normal(0.0f, 1.0f, 0.0f);
	Vector3D z_Normal(0.0f, 0.0f, 1.0f);

	Vector3D pos(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	//モデルの変換後の法線
	x_Normal = Vector3D::Matrix3x3(x_Normal, m_Matrix);
	y_Normal = Vector3D::Matrix3x3(y_Normal, m_Matrix);
	z_Normal = Vector3D::Matrix3x3(z_Normal, m_Matrix);

	Fiqure::RenderLine3D(pos, pos + x_Normal * 2, Vector3D(1.0f, 0.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + y_Normal * 2, Vector3D(0.0f, 1.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + z_Normal * 2, Vector3D(0.0f, 0.0f, 1.0f));
}

void MeshBase::DebugAxis(const Matrix &matrix)
{
	Vector3D x_Normal(1.0f, 0.0f, 0.0f);
	Vector3D y_Normal(0.0f, 1.0f, 0.0f);
	Vector3D z_Normal(0.0f, 0.0f, 1.0f);

	Vector3D pos(matrix._41, matrix._42, matrix._43);

	//モデルの変換後の法線
	x_Normal = Vector3D::Matrix3x3(x_Normal, m_Matrix);
	y_Normal = Vector3D::Matrix3x3(y_Normal, m_Matrix);
	z_Normal = Vector3D::Matrix3x3(z_Normal, m_Matrix);

	Fiqure::RenderLine3D(pos, pos + x_Normal * 2, Vector3D(1.0f, 0.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + y_Normal * 2, Vector3D(0.0f, 1.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + z_Normal * 2, Vector3D(0.0f, 0.0f, 1.0f));
}