#include "StaticMesh.h"
#include "..\..\Figure\Fiqure.h"
#include "..\..\Shader\StaticMeshShader\StaticMeshShader.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\..\Shader\ConstantShader.h"
#include "..\..\ImageSystem\Image.h"

StaticMesh::StaticMesh() :
	MeshBase(false, 0, 0, MeshState::eNothing),
	m_pMeshData(nullptr)
{
}

StaticMesh::StaticMesh(const std::string &meshName, unsigned int priorityGroup, unsigned int priority, unsigned int meshState) :
	MeshBase(true, priorityGroup, priority, meshState)
{
	AllocationMeshData(meshName);

	//モデルが光を遮って影になる対象
	if (meshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

StaticMesh::~StaticMesh()
{
	if (m_MeshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
	}
	m_pMeshData = nullptr;
}

void StaticMesh::SetMeshState(unsigned int meshState)
{
	//事前に登録していたら解除
	if (m_MeshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
	}

	m_MeshState = meshState;

	//モデルが光を遮って影になる対象
	if (m_MeshState & MeshState::eBlockingLight)
	{
		assert(m_IsRenderingRegister && "レンダリングが登録されていないため「eBlockingLight」ができません");
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void StaticMesh::SetAsset(const std::string &meshName)
{
	AllocationMeshData(meshName);
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

const int StaticMesh::GetFaceAllNum() const
{
	return m_pMeshData->GetMeshInfo()->faceNumAll;
}

const int StaticMesh::GetMaterialAllNum() const
{
	return m_pMeshData->GetMeshInfo()->materialNumAll;
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
		v1 = vertex[index[i].vertexIndex[0]].pos * m_ModelMatrix;
		v2 = vertex[index[i].vertexIndex[1]].pos * m_ModelMatrix;
		v3 = vertex[index[i].vertexIndex[2]].pos * m_ModelMatrix;

		normal1 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[0]].normal, m_ModelMatrix).GetNormalize();
		normal2 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[1]].normal, m_ModelMatrix).GetNormalize();
		normal3 = Vector3D::Matrix3x3(vertex[index[i].vertexIndex[2]].normal, m_ModelMatrix).GetNormalize();

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
		v1 = vertex[index[i].vertexIndex[0]].pos * m_ModelMatrix;
		v2 = vertex[index[i].vertexIndex[1]].pos * m_ModelMatrix;
		v3 = vertex[index[i].vertexIndex[2]].pos * m_ModelMatrix;

		Fiqure::RenderLine3D(v1, v2, Vector3D(0.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(v2, v3, Vector3D(0.0f, 0.0f, 0.0f));
		Fiqure::RenderLine3D(v3, v1, Vector3D(0.0f, 0.0f, 0.0f));
	}
}

void StaticMesh::ForwardRendering()
{
	if (m_MeshState & eShadow)
	{
		RenderFunc(m_ModelMatrix, true);
	}
	else
	{
		RenderFunc(m_ModelMatrix, false);
	}
}

void StaticMesh::DeferredRendering()
{

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

	m_pLocalMatrix = &data->localMatrix;
}

void StaticMesh::RenderFunc(Matrix &matrix, bool isShadow) const
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();
	MeshInfo *data = m_pMeshData->GetMeshInfo();

	ConstantShader::GetInstance()->SetTransformMatrixConstantBuffer(pDeviceContext, matrix, isShadow);
	StaticMeshShader::GetInstance()->SetShader(pDeviceContext, data->isTexture, isShadow);

	//マルチテクスチャ
	if (m_pImageInfo != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImageInfo->pTexture);
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