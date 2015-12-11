#include "DynamicMesh.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include <assert.h>

DynamicMesh::DynamicMesh()
{
}

DynamicMesh::~DynamicMesh()
{
	m_pSkinMeshData = nullptr;
}

void DynamicMesh::SetAsset(SkinMeshData *meshData)
{
	m_pSkinMeshData = meshData;
	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
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

void DynamicMesh::SetAsset(const std::string &MeshName)
{
	m_pSkinMeshData = DynamicMeshAsset::GetMesh(MeshName);
	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
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

void DynamicMesh::ChangeAnimation(int num)
{
	m_pSkinMeshData->ChangeAnimation(num);
}

void DynamicMesh::SetPlayTime(float animSpeed)
{
	m_pSkinMeshData->SetRenewalTime(animSpeed);
}

void DynamicMesh::SetTime(float animTime)
{
	m_pSkinMeshData->SetTime(animTime);
}

SkinVertexInfo *DynamicMesh::GetVertex()
{
	return m_pSkinMeshData->GetSkinMeshInfo()->pvVertex;
}

float DynamicMesh::GetPlayTime()
{
	return m_pSkinMeshData->GetPlayTime();
}

int DynamicMesh::GetPlayAnimation()
{
	return m_pSkinMeshData->GetPlayAnimation();
}

int DynamicMesh::GetFaceAllNum()
{
	return m_pSkinMeshData->GetSkinMeshInfo()->faceNumAll;
}

int DynamicMesh::GetBornNum(std::string name)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return i;
		}
	}
	return -1;
}

int DynamicMesh::GetBornAllNum()
{
	return m_pSkinMeshData->GetBornAllNum();
}

std::string DynamicMesh::GetBornName(int bornIndex)
{
	return m_pSkinMeshData->GetBornName(bornIndex);
}

Matrix DynamicMesh::GetBornMatrix(int bornIndex, bool isWorld)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //ワールド行列格納
	if (isWorld) return m_pSkinMeshData->GetBornWorld(bornIndex) * m * m_Matrix;
	return m_pSkinMeshData->GetBornWorld(bornIndex);
}

Matrix DynamicMesh::GetBornMatrix(std::string name, bool isWorld)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //ワールド行列格納
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			if (isWorld) return m_pSkinMeshData->GetBornWorld(i) * m * m_Matrix;
			return m_pSkinMeshData->GetBornWorld(i);
		}
	}
	
	return Matrix();
}

Vector3D DynamicMesh::GetBornPos(int bornIndex)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //ワールド行列格納
	return m_pSkinMeshData->GetBornPos(bornIndex) * m * m_Matrix;
}

Vector3D DynamicMesh::GetBornPos(std::string name)
{
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();
	Matrix m = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat; //ワールド行列格納
	for (unsigned int i = 0; i < pBornData->BornList.size(); i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return m_pSkinMeshData->GetBornPos(i) * m * m_Matrix;
		}
	}
	return Vector3D();
}

void DynamicMesh::Render()
{
	RenderFunc(m_Matrix);
}

void DynamicMesh::RenderMatrix(Matrix &matrix)
{
	RenderFunc(matrix);
}

void DynamicMesh::BornDebug(eBornDebug eBornDebug)
{
	m_pSkinMeshData->BornDebug(eBornDebug);
}

void DynamicMesh::AnimationDebug(int animNum)
{
	m_pSkinMeshData->AnimationDebug(animNum);
}

void DynamicMesh::RenderFunc(Matrix &matrix)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//assert(m_pSkinMeshData != nullptr && "メッシュ情報がありません");

	SkinMeshInfo *data = m_pSkinMeshData->GetSkinMeshInfo();
	BornInfo *pBornData = m_pSkinMeshData->GetBornInfo();

	D3DXMATRIX World = pBornData->BornList[0]->initMat * pBornData->BornList[pBornData->BornList.size() - 1]->initMat * matrix; //ワールド行列格納

	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(data->m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(data->m_pPixelShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		data->m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SkinConstantBuffer0 sg;
		
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
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer0));
		pDeviceContext->Unmap(data->m_pConstantBuffer0, 0);
	}

	//コンスタントバッファーを使うシェーダーの登録
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->m_pConstantBuffer0);

	m_pSkinMeshData->Update();
	if (SUCCEEDED(pDeviceContext->Map(data->m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BornConstantBuffer sg;
		for (int i = 0; i < m_pSkinMeshData->GetBornAllNum(); i++)
		{
			D3DXMATRIX m = data->BornBuffer.bornMat[i];
			D3DXMatrixTranspose(&m, &m);
			sg.bornMat[i] = m;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BornConstantBuffer));
		pDeviceContext->Unmap(data->m_pConstantBufferBone, 0);
	}

	pDeviceContext->VSSetConstantBuffers(2, 1, &data->m_pConstantBufferBone);
	pDeviceContext->PSSetConstantBuffers(2, 1, &data->m_pConstantBufferBone);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(data->m_pVertexLayout);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//属性ごとにレンダリング

	//バーテックスバッファーをセット
	UINT Stride = sizeof(SkinVertexInfo);
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

		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(pDeviceContext->Map(data->m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SkinConstantBuffer1 sg;
			sg.vDiffuse = diffuse;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = specular;//スペキュラーをシェーダーに渡す
			sg.vAmbient = ambient;//アンビエントををシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer1));
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
