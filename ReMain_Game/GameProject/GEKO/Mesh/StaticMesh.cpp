#include "StaticMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include "..\Figure\Fiqure.h"

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
	m_pMeshData = nullptr;
}

void StaticMesh::SetAsset(MeshData *meshData)
{
	m_pMeshData = meshData;
	MeshInfo *data = m_pMeshData->GetMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;
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

void StaticMesh::SetAsset(const std::string &MeshName)
{
	m_pMeshData = StaticMeshAsset::GetMesh(MeshName);
	MeshInfo *data = m_pMeshData->GetMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;
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

void StaticMesh::Render()
{
	RenderFunc(m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix);
}

void StaticMesh::RenderMatrix(Matrix &matrix)
{
	RenderFunc(matrix);
}

VertexInfo *StaticMesh::GetVertex()
{
	return m_pMeshData->GetMeshInfo()->pvVertex;
}

Matrix *StaticMesh::GetLocalMatrix()
{
	return &m_pMeshData->GetMeshInfo()->m_LocalMat;
}

int StaticMesh::GetFaceNum()
{
	return m_pMeshData->GetMeshInfo()->faceNumAll;
}

void StaticMesh::RenderFunc(Matrix &matrix)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//assert(m_pMeshData != nullptr && "メッシュ情報がありません");

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
		D3DXVec4Normalize(&sg.vLightDir, DirectionalLight::GetDirection());
		sg.fIntensity = DirectionalLight::GetLightColor()->Change();

		//視点位置を渡す
		D3DXVECTOR3 EyePos(Camera::GetEyePositionD3D());
		sg.vEye = D3DXVECTOR4(EyePos.x, EyePos.y, EyePos.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer0));
		pDeviceContext->Unmap(data->m_pConstantBuffer0, 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(data->m_pVertexLayout);

	//プリミティブ・トポロジーをセット
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
			sg.vDiffuse = diffuse;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = specular;//スペキュラーをシェーダーに渡す
			sg.vAmbient = ambient;//アンビエントををシェーダーに渡す
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

void StaticMesh::DebugNormal()
{
	Vector3D v1, v2, v3;
	Vector3D normal1, normal2, normal3;

	Matrix m = m_pMeshData->GetMeshInfo()->m_LocalMat * m_Matrix;

	for (int i = 0; i < GetFaceNum(); i++)
	{
		v1 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3].vPos;
		v2 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3 + 1].vPos;
		v3 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3 + 2].vPos;
		normal1 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3].vNormal;
		normal2 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3 + 1].vNormal;
		normal3 = m_pMeshData->GetMeshInfo()->pvVertex[i * 3 + 2].vNormal;

		v1 = v1 * m;
		v2 = v2 * m;
		v3 = v3 * m;
		normal1 = Vector3D::Matrix3x3(normal1, m).GetNormalize();
		normal2 = Vector3D::Matrix3x3(normal2, m).GetNormalize();
		normal3 = Vector3D::Matrix3x3(normal3, m).GetNormalize();

		Fiqure::RenderLine3D(v1, normal1 + v1, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v2, normal2 + v2, Vector3D(0.0f, 1.0f, 0.8f));
		Fiqure::RenderLine3D(v3, normal3 + v3, Vector3D(0.0f, 1.0f, 0.8f));
	}
}