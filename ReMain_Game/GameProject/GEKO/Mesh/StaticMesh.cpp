#include "StaticMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include "..\Figure\Fiqure.h"

StaticMesh::StaticMesh() :
	m_pMeshData(nullptr)
{
}

StaticMesh::StaticMesh(const std::string &meshName)
{
	AllocationMeshData(meshName);
}

StaticMesh::~StaticMesh()
{
	m_pMeshData = nullptr;
}

void StaticMesh::SetAsset(const std::string &meshName)
{
	AllocationMeshData(meshName);
}

void StaticMesh::WorldMatrixBuilding()
{
	D3DXQUATERNION qOut(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qX(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qY(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qZ(0, 0, 0, 1); //単位クォータニオン
	D3DXVECTOR3 xAxis(1, 0, 0); //Xの中心軸
	D3DXVECTOR3 yAxis(0, 1, 0); //Yの中心軸
	D3DXVECTOR3 zAxis(0, 0, 1); //Zの中心軸
	D3DXMATRIX Mat;

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

	m_Matrix = mat;
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

void StaticMesh::Render() const
{
	RenderFunc(m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix);
}

void StaticMesh::RenderMatrix(Matrix &matrix) const
{
	RenderFunc(m_pMeshData->GetMeshInfo()->m_LocalMat * matrix);
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

void StaticMesh::RenderFunc(Matrix &matrix) const
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	assert(m_pMeshData != nullptr && "メッシュ情報がありません");

	MeshInfo *data = m_pMeshData->GetMeshInfo();

	D3DXMATRIX World = matrix; //ワールド行列格納

	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(data->m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(data->m_pPixelShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		data->m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 sg;

		//ワールド行列を渡す
		sg.mW = World;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = World * (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		//ライトの方向を渡す
		D3DXVec4Normalize(&sg.LightDir, DirectionalLight::GetDirection());
		sg.fIntensity = DirectionalLight::GetLightColor()->Change();

		//視点位置を渡す
		D3DXVECTOR3 EyePos(Camera::GetEyePositionD3D());
		sg.eyePos = D3DXVECTOR4(EyePos.x, EyePos.y, EyePos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer0));
		pDeviceContext->Unmap(data->m_pConstantBuffer0, 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);

	//頂点インプットレイアウトをセット
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetInputLayout(data->m_pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//属性ごとにレンダリング

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

		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(pDeviceContext->Map(data->m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 sg;
			sg.diffuse = diffuse;   //ディフューズカラーをシェーダーに渡す
			sg.specular = specular; //スペキュラーをシェーダーに渡す
			sg.ambient = ambient;   //アンビエントををシェーダーに渡す

			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer1));
			pDeviceContext->Unmap(data->m_pConstantBuffer1, 0);
		}
		pDeviceContext->VSSetConstantBuffers(1, 1, &data->m_pConstantBuffer1);
		pDeviceContext->PSSetConstantBuffers(1, 1, &data->m_pConstantBuffer1);

		//テクスチャーをシェーダーに渡す
		if (data->m_pMaterial[i].pTexture)
		{
			pDeviceContext->PSSetSamplers(0, 1, &data->m_pSampleLinear);
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_pMaterial[i].pTexture);
		}
		else
		{
			pDeviceContext->PSSetShaderResources(0, 1, &data->m_Nothing);
		}

		//プリミティブをレンダリング
		pDeviceContext->DrawIndexed(data->m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}