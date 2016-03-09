#include "StaticMesh.h"
#include "..\..\Figure\Fiqure.h"
#include "..\..\Shader\StaticMeshShader\StaticMeshShader.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\..\Shader\ConstantShader.h"
#include "..\..\ImageSystem\Image.h"

StaticMesh::StaticMesh() :
	m_pMeshData(nullptr)
{
}

StaticMesh::StaticMesh(const std::string &meshName, bool isLightInterrupted)
{
	AllocationMeshData(meshName);
	if (isLightInterrupted)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

StaticMesh::~StaticMesh()
{
	ShaderShadowMap::GetInstance()->Clear(this);
	m_pMeshData = nullptr;
}

void StaticMesh::SetAsset(const std::string &meshName, bool isLightInterrupted)
{
	AllocationMeshData(meshName);
	if (isLightInterrupted)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void StaticMesh::SetModelMatrixBuilding()
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
	m_SynthesisMatrix = m_pMeshData->GetMeshInfo()->localMatrix * m_WorldMatrix;
}

const IndexInfo *StaticMesh::GetIndex() const
{
	return m_pMeshData->GetMeshInfo()->pIndex;
}

const VertexInfo *StaticMesh::GetVertex() const
{
	return m_pMeshData->GetVertex();
}

const MaterialInfo *StaticMesh::GetMaterial() const
{
	return m_pMeshData->GetMeshInfo()->pMaterial;
}

const Matrix *StaticMesh::GetLocalMatrix() const
{
	return &m_pMeshData->GetMeshInfo()->localMatrix;
}

const int StaticMesh::GetFaceAllNum() const
{
	return m_pMeshData->GetMeshInfo()->faceNumAll;
}

const int StaticMesh::GetMaterialAllNum() const
{
	return m_pMeshData->GetMeshInfo()->materialNumAll;
}

void StaticMesh::Render(bool isShadow)
{
	RenderFunc(m_SynthesisMatrix, isShadow);
}

void StaticMesh::RenderMatrix(Matrix &matrix, bool isShadow)
{
	Matrix local = m_pMeshData->GetMeshInfo()->localMatrix;;
	m_SynthesisMatrix = local * m_SynthesisMatrix * matrix;
	RenderFunc(m_SynthesisMatrix, isShadow);
}

void StaticMesh::DebugNormal() const
{
	Vector3D v1, v2, v3;
	Vector3D normal1, normal2, normal3;
	VertexInfo *vertex = m_pMeshData->GetVertex();
	IndexInfo *index = m_pMeshData->GetMeshInfo()->pIndex;
	int polyNum = m_pMeshData->GetMeshInfo()->faceNumAll;

	//全ての頂点の法線描画
	for (int i = 0; i < polyNum; i++)
	{
		v1 = vertex[index[i].vertexIndex[0]].pos * m_SynthesisMatrix;
		v2 = vertex[index[i].vertexIndex[1]].pos * m_SynthesisMatrix;
		v3 = vertex[index[i].vertexIndex[2]].pos * m_SynthesisMatrix;

		normal1 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[0]].normal, m_SynthesisMatrix).GetNormalize();
		normal2 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[1]].normal, m_SynthesisMatrix).GetNormalize();
		normal3 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[2]].normal, m_SynthesisMatrix).GetNormalize();

		Fiqure::RenderLine3D(v1, normal1 + v1, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v2, normal2 + v2, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v3, normal3 + v3, Vector3D(0.0f, 1.0f, 0.8f));
	}
}

void StaticMesh::DebugPolygon() const
{
	Vector3D v1, v2, v3;
	VertexInfo *vertex = m_pMeshData->GetVertex();
	IndexInfo *index = m_pMeshData->GetMeshInfo()->pIndex;
	int polyNum = m_pMeshData->GetMeshInfo()->faceNumAll;

	//全ての頂点の描画
	for (int i = 0; i < polyNum; i++)
	{
		v1 = vertex[index[i].vertexIndex[0]].pos * m_SynthesisMatrix;
		v2 = vertex[index[i].vertexIndex[1]].pos * m_SynthesisMatrix;
		v3 = vertex[index[i].vertexIndex[2]].pos * m_SynthesisMatrix;

		Fiqure::RenderLine3D(v1, v2, Vector3D(0.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(v2, v3, Vector3D(0.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(v3, v1, Vector3D(0.0f, 0.0f, 0.0f));
	}
}

void StaticMesh::AllocationMeshData(const std::string &meshName)
{
	//初期化
	m_Diffuse.clear();
	m_Diffuse.shrink_to_fit();
	m_Specular.clear();
	m_Specular.shrink_to_fit();
	m_Ambient.clear();
	m_Ambient.shrink_to_fit();
	m_pMeshData = nullptr;

	m_pMeshData = StaticMeshAsset::GetMesh(meshName);
	MeshInfo *data = m_pMeshData->GetMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;

	//マテリアル割り当て
	for (int i = 0; i < data->materialNumAll; i++)
	{
		diffuse = data->pMaterial[i].diffuse;
		specular = data->pMaterial[i].specular;
		ambient = data->pMaterial[i].ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}

	m_WorldMatrixInfo.pLocalMatrix = &data->localMatrix;
}

void StaticMesh::RenderFunc(Matrix &matrix, bool isShadow) const
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	assert(m_pMeshData != nullptr && "メッシュ情報がありません");

	MeshInfo *data = m_pMeshData->GetMeshInfo();

	ConstantShader::GetInstance()->SetCommonInfoConstantBuffer();

	ConstantShader::GetInstance()->SetTransformMatrixConstantBuffer(pDeviceContext, matrix, isShadow);

	StaticMeshShader::GetInstance()->SetShader(pDeviceContext, data->isTexture, isShadow);

	//マルチテクスチャ
	if (m_pImage != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImage->m_pImageData->GetImageInfo()->pTexture);
	}

	//バーテックスバッファーをセット
	UINT Stride = sizeof(VertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &Stride, &offset);

	Vector4D specular;
	Vector4D ambient;
	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (int i = 0; i < data->materialNumAll; i++)
	{
		//使用されていないマテリアル対策
		if (data->pMaterial[i].numPolygon == 0) continue;
		//インデックスバッファーをセット
		pDeviceContext->IASetIndexBuffer(data->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素を（シェーダー）に渡す
		specular.x = m_Specular[i].x;
		specular.y = m_Specular[i].y;
		specular.z = m_Specular[i].z;
		specular.w = m_Diffuse[i].w;

		ambient.x = m_Ambient[i].x;
		ambient.y = m_Ambient[i].y;
		ambient.z = m_Ambient[i].z;
		ambient.w = m_Diffuse[i].w;

		ConstantShader::GetInstance()->SetMaterialConstantBuffer(pDeviceContext, m_Diffuse[i], specular, ambient);

		pDeviceContext->PSSetSamplers(0, 1, &data->pSampleLinear);
		pDeviceContext->PSSetShaderResources(0, 1, &data->pMaterial[i].pTexture);

		//プリミティブをレンダリング
		pDeviceContext->DrawIndexed(data->pMaterial[i].numPolygon * 3, 0, 0);
	}

	//テクスチャリソース初期化
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(1, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(2, 1, &pShaderResource);
}