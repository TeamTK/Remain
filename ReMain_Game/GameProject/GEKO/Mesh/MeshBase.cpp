#include "MeshBase.h"
#include "..\System\Math.h"
#include "..\Figure\Fiqure.h"
#include "..\ImageSystem\Image.h"

Matrix g_LocalMatrixTemporary;

MeshBase::MeshBase(bool isRegister, unsigned int priorityGroup, unsigned int priority, unsigned int meshState) :
	m_IsRenderingRegister(isRegister),
	m_MeshState(meshState),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_pImageInfo(nullptr),
	m_pLocalMatrix(nullptr),
	m_RenderingType(eForward)
{
	//レンダリング管理に登録
	m_RenderingInfo.priorityGroup = priorityGroup;
	m_RenderingInfo.priority = priority;
	m_RenderingInfo.func = std::bind(&MeshBase::ForwardRendering, this);

	if (m_IsRenderingRegister)
	{
		RenderingManager::GetInstance()->AddForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}

	m_WorldMatrixInfo.pWorldMatrix = &m_WorldMatrix;
	m_WorldMatrixInfo.pModelMatrix = &m_ModelMatrix;
	m_WorldMatrixInfo.pRotation = &m_Rotation;
	m_WorldMatrixInfo.pScale = &m_Scale;
	m_WorldMatrixInfo.pTranselate = &m_Transelate;
	m_WorldMatrixInfo.pLocalMatrix = &g_LocalMatrixTemporary;
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

	m_pImageInfo = nullptr;
	
	//レンダリング管理削除
	if (m_IsRenderingRegister)
	{
		if (m_RenderingType == eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
	}

	m_WorldMatrixInfo.pWorldMatrix = nullptr;
	m_WorldMatrixInfo.pRotation = nullptr;
	m_WorldMatrixInfo.pScale = nullptr;
	m_WorldMatrixInfo.pTranselate = nullptr;
	WorldMatrixManager::GetInstance()->Clear();
}

void MeshBase::SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister != isRegister&& "レンダリングがすでに登録されています");

	auto *pRendering = RenderingManager::GetInstance();

	if (isRegister)
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			m_RenderingInfo.priorityGroup = priorityGroup;
			m_RenderingInfo.priority = priority;
			pRendering->AddDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
		else
		{
			m_RenderingInfo.priorityGroup = priorityGroup;
			m_RenderingInfo.priority = priority;
			pRendering->AddForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
	}
	else
	{
		//レンダリング管理に登録
		if (m_RenderingType == eDeferred)
		{
			pRendering->ClearDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
		else
		{
			pRendering->ClearForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
	}
	m_IsRenderingRegister = isRegister;
}

void MeshBase::SetRenderingType(RenderingType renderingType)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");
	if (m_RenderingType == renderingType) return;
	m_RenderingType = renderingType;

	//レンダリング管理再登録
	if (m_RenderingType == eDeferred)
	{
		m_RenderingInfo.func = std::bind(&MeshBase::DeferredRendering, this);
		RenderingManager::GetInstance()->ClearForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		RenderingManager::GetInstance()->AddDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
	else
	{
		m_RenderingInfo.func = std::bind(&MeshBase::ForwardRendering, this);
		RenderingManager::GetInstance()->ClearDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		RenderingManager::GetInstance()->AddForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
}

void MeshBase::SetPriority(unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsRenderingRegister && "レンダリングが登録されていません");

	//優先順位グループに変化がない場合
	if (m_RenderingInfo.priorityGroup == priorityGroup)
	{
		m_RenderingInfo.priority = priority;

		if (m_RenderingType == eDeferred)
		{
			RenderingManager::GetInstance()->SortDeferred(m_RenderingInfo.priorityGroup);
		}
		else
		{
			RenderingManager::GetInstance()->SortForward(m_RenderingInfo.priorityGroup);
		}
	}
	else
	{
		if (m_RenderingType == eDeferred)
		{
			RenderingManager::GetInstance()->ClearDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
			m_RenderingInfo.priorityGroup = priorityGroup;
			m_RenderingInfo.priority = priority;
			RenderingManager::GetInstance()->AddDeferred(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
		else
		{
			RenderingManager::GetInstance()->ClearForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
			m_RenderingInfo.priorityGroup = priorityGroup;
			m_RenderingInfo.priority = priority;
			RenderingManager::GetInstance()->AddForward(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		}
	}
}

void MeshBase::SetScale(float x, float y, float z)
{
	m_Scale = Vector3D(x, y, z);
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

void MeshBase::SetTexture(const std::string &assetName)
{
	m_pImageInfo = ImageAsset::GetImage(assetName)->GetImageInfo();
}

void MeshBase::SetModelMatrixBuilding()
{
	D3DXQUATERNION qOut(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qX(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qY(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qZ(0, 0, 0, 1); //単位クォータニオン
	Vector3D xAxis(1, 0, 0); //Xの中心軸
	Vector3D yAxis(0, 1, 0); //Yの中心軸
	Vector3D zAxis(0, 0, 1); //Zの中心軸
	Matrix Mat;

	Matrix mat;

	D3DXQuaternionRotationAxis(&qX, &xAxis, m_Rotation.x);
	D3DXQuaternionRotationAxis(&qY, &yAxis, m_Rotation.y);
	D3DXQuaternionRotationAxis(&qZ, &zAxis, m_Rotation.z);
	qOut = qX * qY * qZ;

	//クオータニオンから行列に変更
	D3DXMatrixRotationQuaternion(&mat, &qOut);

	//拡大縮小
	mat._11 *= m_Scale.x;
	mat._21 *= m_Scale.x;
	mat._31 *= m_Scale.x;

	mat._12 *= m_Scale.y;
	mat._22 *= m_Scale.y;
	mat._32 *= m_Scale.y;

	mat._13 *= m_Scale.z;
	mat._23 *= m_Scale.z;
	mat._33 *= m_Scale.z;

	//平行移動
	mat._41 = m_Transelate.x;
	mat._42 = m_Transelate.y;
	mat._43 = m_Transelate.z;

	m_WorldMatrix = mat;
	m_ModelMatrix = *m_pLocalMatrix * m_WorldMatrix;
}

void MeshBase::SetSynthesisMatirx(Matrix &matrix)
{
	m_ModelMatrix = *m_pLocalMatrix * m_ModelMatrix * matrix;
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

Vector3D MeshBase::GetAxisX(MatrixType matrixType) const
{
	Matrix matrix;
	switch (matrixType)
	{
	case MatrixType::eLocalMatrix:
		matrix = *m_pLocalMatrix;
		break;

	case MatrixType::eWorldMatrix:
		matrix = m_WorldMatrix;
		break;

	case MatrixType::eModelMatrix:
		matrix = m_ModelMatrix;
		break;
	}

	return Vector3D::Matrix3x3(Vector3D(1.0f, 0.0f, 0.0f), matrix);
}

Vector3D MeshBase::GetAxisY(MatrixType matrixType) const
{
	Matrix matrix;
	switch (matrixType)
	{
	case MatrixType::eLocalMatrix:
		matrix = *m_pLocalMatrix;
		break;

	case MatrixType::eWorldMatrix:
		matrix = m_WorldMatrix;
		break;

	case MatrixType::eModelMatrix:
		matrix = m_ModelMatrix;
		break;
	}

	return Vector3D::Matrix3x3(Vector3D(0.0f, 1.0f, 0.0f), matrix);
}

Vector3D MeshBase::GetAxisZ(MatrixType matrixType) const
{
	Matrix matrix;
	switch (matrixType)
	{
	case MatrixType::eLocalMatrix:
		matrix = *m_pLocalMatrix;
		break;

	case MatrixType::eWorldMatrix:
		matrix = m_WorldMatrix;
		break;

	case MatrixType::eModelMatrix:
		matrix = m_ModelMatrix;
		break;
	}

	return Vector3D::Matrix3x3(Vector3D(0.0f, 0.0f, 1.0f), matrix);
}

const Matrix *MeshBase::GetLocalMatrix() const
{
	return m_pLocalMatrix;
}

const Matrix *MeshBase::GetWorldMatrix() const
{
	return &m_WorldMatrix;
}

const Matrix *MeshBase::GetModelMatrix() const
{
	return &m_ModelMatrix;
}

void MeshBase::DebugAxis(MatrixType matrixType)
{
	Matrix matrix;
	Vector3D x_Normal(1.0f, 0.0f, 0.0f);
	Vector3D y_Normal(0.0f, 1.0f, 0.0f);
	Vector3D z_Normal(0.0f, 0.0f, 1.0f);

	switch (matrixType)
	{
	case MatrixType::eLocalMatrix:
		matrix = *m_pLocalMatrix;
		break;

	case MatrixType::eWorldMatrix:
		matrix = m_WorldMatrix;
		break;

	case MatrixType::eModelMatrix:
		matrix = m_ModelMatrix;
		break;
	}

	Vector3D pos(matrix._41, matrix._42, matrix._43);

	//モデルの変換後の法線
	x_Normal = Vector3D::Matrix3x3(x_Normal, matrix);
	y_Normal = Vector3D::Matrix3x3(y_Normal, matrix);
	z_Normal = Vector3D::Matrix3x3(z_Normal, matrix);

	Fiqure::RenderLine3D(pos, pos + x_Normal * 2, Vector3D(1.0f, 0.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + y_Normal * 2, Vector3D(0.0f, 1.0f, 0.0f));
	Fiqure::RenderLine3D(pos, pos + z_Normal * 2, Vector3D(0.0f, 0.0f, 1.0f));
}

void MeshBase::ForwardRendering()
{

}

void MeshBase::DeferredRendering()
{

}