#include "StaticMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include "..\Figure\Fiqure.h"
#include "..\Shader\StaticMeshShader\StaticMeshShader.h"
#include "..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\ImageSystem\Image.h"

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

const IndexInfo *StaticMesh::GetIndex() const
{
	return m_pMeshData->GetMeshInfo()->pIndex;
}

const VertexInfo *StaticMesh::GetVertex() const
{
	return m_pMeshData->GetMeshInfo()->pVertex;
}

const MaterialInfo *StaticMesh::GetMaterial() const
{
	return m_pMeshData->GetMeshInfo()->m_pMaterial;
}

const Matrix *StaticMesh::GetLocalMatrix() const
{
	return &m_pMeshData->GetMeshInfo()->m_LocalMat;
}

const int StaticMesh::GetFaceAllNum() const
{
	return m_pMeshData->GetMeshInfo()->faceNumAll;
}

const int StaticMesh::GetMaterialAllNum() const
{
	return m_pMeshData->GetMeshInfo()->materialNumAll;
}

void StaticMesh::Render(bool isShadow) const
{
	RenderFunc(m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix, isShadow);
}

void StaticMesh::RenderMatrix(Matrix &matrix, bool isShadow) const
{
	RenderFunc(m_pMeshData->GetMeshInfo()->m_LocalMat * matrix, isShadow);
}

void StaticMesh::DebugNormal() const
{
	Vector3D v1, v2, v3;
	Vector3D normal1, normal2, normal3;
	Matrix m = m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix;
	VertexInfo *vertex = m_pMeshData->GetMeshInfo()->pVertex;
	IndexInfo *index = m_pMeshData->GetMeshInfo()->pIndex;
	int polyNum = m_pMeshData->GetMeshInfo()->faceNumAll;

	//全ての頂点の法線描画
	for (int i = 0; i < polyNum; i++)
	{
		v1 = vertex[index[i].vertexIndex[0]].pos * m;
		v2 = vertex[index[i].vertexIndex[1]].pos * m;
		v3 = vertex[index[i].vertexIndex[2]].pos * m;

		normal1 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[0]].normal, m).GetNormalize();
		normal2 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[1]].normal, m).GetNormalize();
		normal3 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[2]].normal, m).GetNormalize();

		Fiqure::RenderLine3D(v1, normal1 + v1, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v2, normal2 + v2, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v3, normal3 + v3, Vector3D(0.0f, 1.0f, 0.8f));
	}
}

void StaticMesh::DebugPolygon() const
{
	Vector3D v1, v2, v3;
	Matrix m = m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix;
	VertexInfo *vertex = m_pMeshData->GetMeshInfo()->pVertex;
	IndexInfo *index = m_pMeshData->GetMeshInfo()->pIndex;
	int polyNum = m_pMeshData->GetMeshInfo()->faceNumAll;

	//全ての頂点の描画
	for (int i = 0; i < polyNum; i++)
	{
		v1 = vertex[index[i].vertexIndex[0]].pos * m;
		v2 = vertex[index[i].vertexIndex[1]].pos * m;
		v3 = vertex[index[i].vertexIndex[2]].pos * m;

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
		diffuse = data->m_pMaterial[i].Diffuse;
		specular = data->m_pMaterial[i].Specular;
		ambient = data->m_pMaterial[i].Ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}
}

void StaticMesh::RenderFunc(Matrix &matrix, bool isShadow) const
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	assert(m_pMeshData != nullptr && "メッシュ情報がありません");

	MeshInfo *data = m_pMeshData->GetMeshInfo();

	StaticMeshShader::GetInstance()->BaseConstantBuffer(pDeviceContext, matrix, isShadow);

	StaticMeshShader::GetInstance()->SetVertexShader(pDeviceContext, data->m_IsTexture, isShadow);
	StaticMeshShader::GetInstance()->SetPixelShader(pDeviceContext, data->m_IsTexture, isShadow);
	StaticMeshShader::GetInstance()->SetInputLayout(pDeviceContext, data->m_IsTexture);

	//マルチテクスチャ
	if (m_pImage != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImage->m_pImageData->GetImageInfo()->pTexture);
	}

	//バーテックスバッファーをセット
	UINT Stride = sizeof(VertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->m_pVertexBuffer, &Stride, &offset);

	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 ambient;
	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (int i = 0; i < data->materialNumAll; i++)
	{
		//使用されていないマテリアル対策
		if (data->m_pMaterial[i].dwNumFace == 0) continue;
		//インデックスバッファーをセット
		pDeviceContext->IASetIndexBuffer(data->m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		diffuse.x = m_Diffuse[i].x;
		diffuse.y = m_Diffuse[i].y;
		diffuse.z = m_Diffuse[i].z;
		diffuse.w = m_Diffuse[i].w;

		specular.x = m_Specular[i].x;
		specular.y = m_Specular[i].y;
		specular.z = m_Specular[i].z;
		specular.w = m_Diffuse[i].w;

		ambient.x = m_Ambient[i].x;
		ambient.y = m_Ambient[i].y;
		ambient.z = m_Ambient[i].z;
		ambient.w = m_Diffuse[i].w;

		StaticMeshShader::GetInstance()->MaterialConstantBuffer(pDeviceContext, diffuse, specular, ambient);

		//テクスチャーをシェーダーに渡す
		if (data->m_pMaterial[i].pTexture)
		{
			pDeviceContext->PSSetSamplers(0, 1, &data->m_pSampleLinear);
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_pMaterial[i].pTexture);
		}

		//プリミティブをレンダリング
		pDeviceContext->DrawIndexed(data->m_pMaterial[i].dwNumFace * 3, 0, 0);
	}

	//テクスチャリソース初期化
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(1, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(2, 1, &pShaderResource);
}