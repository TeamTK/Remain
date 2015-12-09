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
	m_Diffuse[materialIndex].x = (float)r / 255;
	m_Diffuse[materialIndex].y = (float)g / 255;
	m_Diffuse[materialIndex].z = (float)b / 255;
}

void MeshBase::SetSpecular(int r, int g, int b, int materialIndex)
{
	Vector3D specular = Vector3D((float)r / 255, (float)g / 255, (float)b / 255);
	m_Specular[materialIndex] = specular;
}

void MeshBase::SetAmbient(int r, int g, int b, int materialIndex)
{
	Vector3D ambient = Vector3D((float)r / 255, (float)g / 255, (float)b / 255);
	m_Ambient[materialIndex] = ambient;
}

void MeshBase::SetAlpha(int alpha, int materialIndex)
{
	m_Diffuse[materialIndex].w = ((float)alpha / 255);
}

void MeshBase::SetAlphaAll(int alpha)
{
	float a = ((float)alpha / 255);
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

Vector3D MeshBase::GetAxisX(float length)
{
	Vector3D x(1.0f, 0.0f, 0.0f);
	Vector3D x_Normal(1.0f, 0.0f, 0.0f);

	x = x * m_Matrix;
	x_Normal = Vector3D::Matrix3x3(x_Normal, m_Matrix);

	return x + x_Normal * length;
}

Vector3D MeshBase::GetAxisY(float length)
{
	Vector3D y(0.0f, 1.0f, 0.0f);
	Vector3D y_Normal(0.0f, 1.0f, 0.0f);

	y = y * m_Matrix;
	y_Normal = Vector3D::Matrix3x3(y_Normal, m_Matrix);

	return y + y_Normal * length;
}

Vector3D MeshBase::GetAxisZ(float length)
{
	Vector3D z(0.0f, 0.0f, 1.0f);
	Vector3D z_Normal(0.0f, 0.0f, 1.0f);

	z = z * m_Matrix;
	z_Normal = Vector3D::Matrix3x3(z_Normal, m_Matrix);

	return z + z_Normal * length;
}


Matrix *MeshBase::GetMatrix()
{
	return &m_Matrix;
}

void MeshBase::DebugAxis()
{
	//基底ベクトル
	Vector3D x(1.0f, 0.0f, 0.0f);
	Vector3D y(0.0f, 1.0f, 0.0f);
	Vector3D z(0.0f, 0.0f, 1.0f);

	Vector3D x_Normal(1.0f, 0.0f, 0.0f);
	Vector3D y_Normal(0.0f, 1.0f, 0.0f);
	Vector3D z_Normal(0.0f, 0.0f, 1.0f);

	//モデルの変換後位置
	x = x * m_Matrix;
	y = y * m_Matrix;
	z = z * m_Matrix;

	//モデルの変換後の法線
	x_Normal = Vector3D::Matrix3x3(x_Normal, m_Matrix);
	y_Normal = Vector3D::Matrix3x3(y_Normal, m_Matrix);
	z_Normal = Vector3D::Matrix3x3(z_Normal, m_Matrix);

	Fiqure::RenderLine3D(x, x + x_Normal * 2, Vector3D(1.0f, 0.0f, 0.0f));
	Fiqure::RenderLine3D(y, y + y_Normal * 2, Vector3D(0.0f, 1.0f, 0.0f));
	Fiqure::RenderLine3D(z, z + z_Normal * 2, Vector3D(0.0f, 0.0f, 1.0f));
}