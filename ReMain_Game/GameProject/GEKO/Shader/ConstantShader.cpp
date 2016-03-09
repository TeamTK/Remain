#include "ConstantShader.h"
#include "..\System\DirectionalLight.h"
#include "..\System\Camera.h"
#include "..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\Mesh\MeshInfo.h"

ConstantShader::ConstantShader()
{
	INIT_NULLPOINTR(m_pCommonInfoConstantBuffer);
	INIT_NULLPOINTR(m_pTransformMatrixConstantBuffer);
	INIT_NULLPOINTR(m_pMaterialConstantBuffer);
	INIT_NULLPOINTR(m_pBornConstantBuffer);
}

ConstantShader::~ConstantShader()
{
	Release();
}

ConstantShader *ConstantShader::GetInstance()
{
	static ConstantShader constantShader;
	return &constantShader;
}

bool ConstantShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	D3D11_BUFFER_DESC cb;

	//コンスタントバッファー作成  共通情報渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CommonInfoConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pCommonInfoConstantBuffer))) return false;

	//コンスタントバッファー作成　変換行列渡し用
	cb.ByteWidth = sizeof(TransformMatrixConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pTransformMatrixConstantBuffer))) return false;

	//コンスタントバッファー作成  マテリアル渡し用
	cb.ByteWidth = sizeof(MaterialConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pMaterialConstantBuffer))) return false;

	//コンスタントバッファー作成 ボーン用  
	cb.ByteWidth = sizeof(BornConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pBornConstantBuffer))) return false;

	return true;
}

void ConstantShader::SetCommonInfoConstantBuffer()
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pCommonInfoConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		CommonInfoConstantBuffer sg;

		//ライトの方向を渡す
		D3DXVec4Normalize(&sg.lightDir, DirectionalLight::GetDirection());
		sg.intensity = *DirectionalLight::GetLightColor();

		//視点位置を渡す
		Vector3D EyePos(Camera::GetEyePosition());
		sg.eyePos = Vector4D(EyePos.x, EyePos.y, EyePos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CommonInfoConstantBuffer));
		pDeviceContext->Unmap(m_pCommonInfoConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pCommonInfoConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pCommonInfoConstantBuffer);
}

void ConstantShader::SetTransformMatrixConstantBuffer(ID3D11DeviceContext *pDeviceContext, Matrix &mat, bool isShadow)
{
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pTransformMatrixConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		TransformMatrixConstantBuffer sg;

		//ワールド行列を渡す
		sg.mW = mat;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = mat * *Camera::GetViewProjection();
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		//ライトの視点からのカメラの行列
		if (isShadow)
		{
			sg.mWLP = mat * *ShaderShadowMap::GetInstance()->GetViewProjMatrix();
			D3DXMatrixTranspose(&sg.mWLP, &sg.mWLP);
		}

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(TransformMatrixConstantBuffer));
		pDeviceContext->Unmap(m_pTransformMatrixConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pTransformMatrixConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pTransformMatrixConstantBuffer);
}

void ConstantShader::SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, 
											 const Vector4D &diffuse, const Vector4D &specular, const Vector4D &ambient)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(m_pMaterialConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		MaterialConstantBuffer sg;
		sg.diffuse = diffuse;   //ディフューズカラーをシェーダーに渡す
		sg.specular = specular; //スペキュラーをシェーダーに渡す
		sg.ambient = ambient;   //アンビエントををシェーダーに渡す

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(MaterialConstantBuffer));
		pDeviceContext->Unmap(m_pMaterialConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
}

void ConstantShader::SetBornConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBorn*> &pCopyBornArray)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//ボーン情報格納
	if (SUCCEEDED(pDeviceContext->Map(m_pBornConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BornConstantBuffer sg;
		for (int i = 0; i < bornAllNum; i++)
		{
			D3DXMatrixTranspose(&sg.bornMat[i], &pCopyBornArray[i]->bornMat);
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BornConstantBuffer));
		pDeviceContext->Unmap(m_pBornConstantBuffer, 0);
	}

	pDeviceContext->VSSetConstantBuffers(3, 1, &m_pBornConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(3, 1, &m_pBornConstantBuffer);
}

void ConstantShader::Release()
{
	//コンスタントバッファー
	SAFE_RELEASE(m_pCommonInfoConstantBuffer);
	SAFE_RELEASE(m_pTransformMatrixConstantBuffer);
	SAFE_RELEASE(m_pMaterialConstantBuffer);
	SAFE_RELEASE(m_pBornConstantBuffer);
}