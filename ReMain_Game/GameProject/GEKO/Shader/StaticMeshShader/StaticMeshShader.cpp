#include "StaticMeshShader.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"

StaticMeshShader::StaticMeshShader()
{
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pVertexShader_NoTexture);
	INIT_NULLPOINTR(m_pVertexShader_ShadowMap);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pPixelShader_NoTexture);
	INIT_NULLPOINTR(m_pPixelShader_ShadowMap);
	INIT_NULLPOINTR(m_pVertexLayout);
}

StaticMeshShader::~StaticMeshShader()
{
	Release();
}

StaticMeshShader *StaticMeshShader::GetInstance()
{
	static StaticMeshShader staticMeshShader;
	return &staticMeshShader;
}

bool StaticMeshShader::Init()
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

	//頂点シェーダー初期化
	if (!InitVertexShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("シャドウマップ頂点シェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

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

	//テクスチャーなしピクセルシェーダー初期化
	if (!InitPixelShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("シャドウマップピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	return true;
}

void StaticMeshShader::SetShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//テクスチャありなしで切り替え
	if (isTexture)
	{
		//影ありなしで切り替え
		if (isShadow)
		{
			//影あり
			pDeviceContext->VSSetShader(m_pVertexShader_ShadowMap, NULL, 0);
			pDeviceContext->PSSetShader(m_pPixelShader_ShadowMap, NULL, 0);

			ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		}
		else
		{
			//影なし
			pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
			pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		}
		pDeviceContext->IASetInputLayout(m_pVertexLayout);
	}
	else
	{
		//テクスチャなし
		pDeviceContext->VSSetShader(m_pVertexShader_NoTexture, NULL, 0);
		pDeviceContext->PSSetShader(m_pPixelShader_NoTexture, NULL, 0);

		pDeviceContext->IASetInputLayout(m_pVertexLayout_NoTexture);
	}

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void StaticMeshShader::Release()
{	
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
}