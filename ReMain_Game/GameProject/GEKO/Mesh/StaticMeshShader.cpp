#include "StaticMeshShader.h"
#include "..\System\Camera.h"
#include "..\System\DirectionalLight.h"
#include "MeshData.h"

StaticMeshShader::StaticMeshShader()
{
	INIT_NULLPOINTR(m_pConstantBuffer0);
	INIT_NULLPOINTR(m_pConstantBuffer1);
	INIT_NULLPOINTR(m_pConstantBuffer2);
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pVertexLayout);
	INIT_NULLPOINTR(m_pPSClassLinkage);
	INIT_NULLPOINTR(m_pClassInstance);
}

StaticMeshShader::~StaticMeshShader()
{
}

StaticMeshShader *StaticMeshShader::GetInstance()
{
	static StaticMeshShader staticMeshShader;
	return &staticMeshShader;
}

HRESULT StaticMeshShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshTest.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl読み込み失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("バーテックスシェーダー作成失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 3;
	memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		MessageBox(0, TEXT("頂点インプットレイアウト作成失敗"), NULL, MB_OK);
		return FALSE;
	}

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshTest.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl読み込み失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	//動的リンク作成
	pDevice->CreateClassLinkage(&m_pPSClassLinkage);
	pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), m_pPSClassLinkage, &m_pPixelShader);

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0))) return E_FAIL;

	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer1))) return E_FAIL;

	//コンスタントバッファー作成  動的リンク
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer2))) return E_FAIL;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	/*
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
	*/

	return S_OK;
}

void StaticMeshShader::BaseConstantBuffer(const Matrix &mat)
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	//pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX World = mat; //ワールド行列格納

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
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
		pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	// レイアウト設定
	
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void StaticMeshShader::MaterialConstantBuffer(D3DXVECTOR4 &diffuse, D3DXVECTOR4 &specular, D3DXVECTOR4 &ambient)
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer1 sg;
		sg.diffuse = diffuse;   //ディフューズカラーをシェーダーに渡す
		sg.specular = specular; //スペキュラーをシェーダーに渡す
		sg.ambient = ambient;   //アンビエントををシェーダーに渡す

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer1));
		pDeviceContext->Unmap(m_pConstantBuffer1, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
}

void StaticMeshShader::Begin(const char* classInstanceName)
{
	//m_pPSClassLinkage->GetClassInstance(classInstanceName, 0, &m_pClassInstance);
	//m_pPSClassLinkage->CreateClassInstance(classInstanceName, NULL, NULL, NULL, NULL, &m_pClassInstance);

	ID3D11DeviceContext *context = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	// ピクセルシェーダーを動的シェーダーリンクとして設定する
	context->PSSetShader(m_pPixelShader, &m_pClassInstance, 1);

	// 頂点シェーダーに定数バッファを設定する
	//context->VSSetConstantBuffers(2, 1, &m_pConstantBuffer2);

	// ピクセルシェーダーに定数バッファを設定する
	//context->PSSetConstantBuffers(2, 1, &m_pConstantBuffer2);
}

void StaticMeshShader::End()
{
	SAFE_RELEASE(m_pClassInstance);
}

void StaticMeshShader::Release()
{
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBuffer2);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPSClassLinkage);
	SAFE_RELEASE(m_pClassInstance);
}