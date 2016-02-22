#include "DynamicMeshShader.h"
#include "..\..\System\Camera.h"
#include "..\..\System\DirectionalLight.h"
#include "..\..\Mesh\MeshData.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"

DynamicMeshShader::DynamicMeshShader()
{
	INIT_NULLPOINTR(m_pConstantBuffer0);
	INIT_NULLPOINTR(m_pMaterialConstantBuffer);
	INIT_NULLPOINTR(m_pBornConstantBuffer);
	//INIT_NULLPOINTR(m_pConstantBuffer2);
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pVertexShader_NoTexture);
	INIT_NULLPOINTR(m_pVertexShader_ShadowMap);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pPixelShader_NoTexture);
	INIT_NULLPOINTR(m_pPixelShader_ShadowMap);
	INIT_NULLPOINTR(m_pVertexLayout);
	//INIT_NULLPOINTR(m_pPSClassLinkage);
	//INIT_NULLPOINTR(m_pClassInstance);
}

DynamicMeshShader::~DynamicMeshShader()
{
	Release();
}

DynamicMeshShader *DynamicMeshShader::GetInstance()
{
	static DynamicMeshShader DynamicMeshShader;
	return &DynamicMeshShader;
}

bool DynamicMeshShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//頂点シェーダー初期化
	if (!InitVertexShader(pDevice))
	{
		MessageBox(0, TEXT("頂点シェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//テクスチャーなし頂点シェーダー初期化
	if (!InitVertexShader_NoTexture(pDevice))
	{
		MessageBox(0, TEXT("テクスチャーなし頂点シェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	/*
	//頂点シェーダー初期化
	if (!InitVertexShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("シャドウマップ頂点シェーダー作成失敗"), NULL, MB_OK);
		return false;
	}
*/
	//ピクセルシェーダー初期化
	if (!InitPixelShader(pDevice))
	{
		MessageBox(0, TEXT("ピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//テクスチャーなしピクセルシェーダー初期化
	if (!InitPixelShader_NoTexture(pDevice))
	{
		MessageBox(0, TEXT("テクスチャーなしピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	/*
	//テクスチャーなしピクセルシェーダー初期化
	if (!InitPixelShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("シャドウマップピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}
	*/
	//動的リンク作成
//	pDevice->CreateClassLinkage(&m_pPSClassLinkage);
//	pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), m_pPSClassLinkage, &m_pPixelShader);

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0))) return false;

	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pMaterialConstantBuffer))) return false;

	//コンスタントバッファーボーン用作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(BornConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pBornConstantBuffer))) return false;

	/*
	//コンスタントバッファー作成  動的リンク
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer2))) return E_FAIL;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	*/

	return true;
}

void DynamicMeshShader::SetBaseConstantBuffer(ID3D11DeviceContext *pDeviceContext, const Matrix &mat, bool isShadow)
{
	D3DXMATRIX World = mat; //ワールド行列格納

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SkinConstantBuffer0 sg;

		//ワールド行列を渡す
		sg.mW = World;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = World * *Camera::GetViewProjection();
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		/*
		//ライトの視点からのカメラの行列
		if (isShadow)
		{
			D3DXMATRIX v = *ShaderShadowMap::GetInstance()->GetViewMatrix();
			D3DXMATRIX p = *ShaderShadowMap::GetInstance()->GetProjMatrix();

			sg.mWLP = World * v * p;
			D3DXMatrixTranspose(&sg.mWLP, &sg.mWLP);
		}
		*/

		//ライトの方向を渡す
		D3DXVec4Normalize(&sg.vLightDir, DirectionalLight::GetDirection());
		sg.fIntensity = DirectionalLight::GetLightColor()->Change();

		//視点位置を渡す
		D3DXVECTOR3 EyePos(Camera::GetEyePositionD3D());
		sg.vEye = D3DXVECTOR4(EyePos.x, EyePos.y, EyePos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer0));
		pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
}

void DynamicMeshShader::SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, D3DXVECTOR4 &diffuse, D3DXVECTOR4 &specular, D3DXVECTOR4 &ambient)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(m_pMaterialConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SkinConstantBuffer1 sg;
		sg.vDiffuse = diffuse;   //ディフューズカラーをシェーダーに渡す
		sg.vSpecular = specular; //スペキュラーをシェーダーに渡す
		sg.vAmbient = ambient;   //アンビエントををシェーダーに渡す

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SkinConstantBuffer1));
		pDeviceContext->Unmap(m_pMaterialConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pMaterialConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pMaterialConstantBuffer);
}

void DynamicMeshShader::SetBornConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBorn*> &pCopyBornArray)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//ボーン情報格納
	if (SUCCEEDED(pDeviceContext->Map(m_pBornConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BornConstantBuffer sg;
		for (int i = 0; i < bornAllNum; i++)
		{
			D3DXMATRIX m = pCopyBornArray[i]->bornMat;
			D3DXMatrixTranspose(&m, &m);
			sg.bornMat[i] = m;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BornConstantBuffer));
		pDeviceContext->Unmap(m_pBornConstantBuffer, 0);
	}

	pDeviceContext->VSSetConstantBuffers(2, 1, &m_pBornConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(2, 1, &m_pBornConstantBuffer);
}

void DynamicMeshShader::SetVertexShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//テクスチャあり
	if (isTexture)
	{
		//影あり
		if (isShadow)
		{
			pDeviceContext->VSSetShader(m_pVertexShader_ShadowMap, NULL, 0);
		}
		else
		{
			pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		}
	}
	else
	{
		pDeviceContext->VSSetShader(m_pVertexShader_NoTexture, NULL, 0);
	}
}

void DynamicMeshShader::SetPixelShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//テクスチャあり
	if (isTexture)
	{
		//影あり
		if (isShadow)
		{
			pDeviceContext->PSSetShader(m_pPixelShader_ShadowMap, NULL, 0);
			ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		}
		else
		{
			pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		}
	}
	else
	{
		pDeviceContext->PSSetShader(m_pPixelShader_NoTexture, NULL, 0);
	}
}

void DynamicMeshShader::SetInputLayout(ID3D11DeviceContext *pDeviceContext, bool isTexture)
{
	if (isTexture)
	{
		pDeviceContext->IASetInputLayout(m_pVertexLayout); 
	}
	else
	{
		pDeviceContext->IASetInputLayout(m_pVertexLayout_NoTexture);
	}

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*
void DynamicMeshShader::Begin(const char* classInstanceName)
{
	//m_pPSClassLinkage->GetClassInstance(classInstanceName, 0, &m_pClassInstance);
	//m_pPSClassLinkage->CreateClassInstance(classInstanceName, NULL, NULL, NULL, NULL, &m_pClassInstance);

	//ID3D11DeviceContext *context = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	// ピクセルシェーダーを動的シェーダーリンクとして設定する
	//context->PSSetShader(m_pPixelShader, &m_pClassInstance, 1);

	// 頂点シェーダーに定数バッファを設定する
	//context->VSSetConstantBuffers(2, 1, &m_pConstantBuffer2);

	// ピクセルシェーダーに定数バッファを設定する
	//context->PSSetConstantBuffers(2, 1, &m_pConstantBuffer2);
}

void DynamicMeshShader::End()
{
//	SAFE_RELEASE(m_pClassInstance);
}
*/

void DynamicMeshShader::Release()
{
	//コンスタントバッファー
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pMaterialConstantBuffer);
	SAFE_RELEASE(m_pBornConstantBuffer);
	//SAFE_RELEASE(m_pConstantBuffer2);

	//シェーダー
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexShader_NoTexture);
	SAFE_RELEASE(m_pVertexShader_ShadowMap);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pPixelShader_NoTexture);
	SAFE_RELEASE(m_pPixelShader_ShadowMap);

	//頂点レイアウト
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexLayout_NoTexture);
	
	//SAFE_RELEASE(m_pPSClassLinkage);
	//SAFE_RELEASE(m_pClassInstance);
}