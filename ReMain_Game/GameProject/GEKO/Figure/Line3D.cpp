#include "Line3D.h"
#include "..\System\Camera.h"

Line3D::Line3D()
{
	//シェーダーの初期化
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "Line3Dシェーダーの初期化に失敗しました", NULL, MB_OK);
	}

	//頂点作成
	if (FAILED(InitVertex()))
	{
		MessageBoxA(0, "Line3D頂点の作成に失敗しました", NULL, MB_OK);
	}
}

Line3D::~Line3D()
{
	SAFE_RELEASE(m_pConstantBufferLine3D);
}

void Line3D::Render(const Vector3D &Spos, const Vector3D &Epos, const Vector3D &Color)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	//変換行列とカラーをシェーダーに送る
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		cb.Color = D3DXVECTOR4(Color.x, Color.y, Color.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//座標をシェーダーに送る
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBufferLine3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBufferLine3D cbLine3D;

		cbLine3D.pos[0] = D3DXVECTOR4(Spos.x, Spos.y, Spos.z, 1.0f);
		cbLine3D.pos[1] = D3DXVECTOR4(Epos.x, Epos.y, Epos.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cbLine3D), sizeof(cbLine3D));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferLine3D);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBufferLine3D);

	//バーテックスバッファーをセット
	UINT stride = sizeof(UINT);
	UINT offset = 0;
	Direct3D11::GetInstance()->GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//プリミティブをレンダリング
	pDeviceContext->Draw(2, 0);
}

HRESULT Line3D::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Line3D.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl読み込み失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("バーテックスシェーダー作成失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 1;
	memcpy(&layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		&layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_FigureInfo.pVertexLayout)))
	{
		return FALSE;
	}

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Line3D.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl読み込み失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("ピクセルシェーダー作成失敗"), NULL, MB_OK);
		return E_FAIL;
	}

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferFiqure);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_FigureInfo.pConstantBuffer)))
	{
		return E_FAIL;
	}

	//コンスタントバッファー作成　座標渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferLine3D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBufferLine3D)))
	{
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return S_OK;
}

HRESULT Line3D::InitVertex()
{
	//バーテックスバッファー作成
	UINT vertices[] =
	{
		UINT(0),
		UINT(1),
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	if (FAILED(Direct3D11::GetInstance()->GetID3D11Device()->CreateBuffer(&bd, &InitData, &m_FigureInfo.pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}