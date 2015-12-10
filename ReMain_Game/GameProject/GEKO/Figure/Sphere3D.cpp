#include "Sphere3D.h"
#include "..\System\Camera.h"

Sphere3D::Sphere3D()
{
	//シェーダーの初期化
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "Sphere3Dシェーダーの初期化に失敗しました", NULL, MB_OK);
	}

	//頂点作成
	if (FAILED(InitVertex()))
	{
		MessageBoxA(0, "Sphere3D頂点の作成に失敗しました", NULL, MB_OK);
	}
}

Sphere3D::~Sphere3D()
{
}

void Sphere3D::Render(const Matrix &World, const Vector3D &Color)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	//変換行列とカラーをシェーダーに送る
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = *(D3DXMATRIX*)(&World) *(*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		cb.Color = D3DXVECTOR4(Color.x, Color.y, Color.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	Direct3D11::Get().GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//プリミティブをレンダリング
	pDeviceContext->Draw(3*8, 0);
}

HRESULT Sphere3D::InitVertex()
{	//バーテックスバッファー作成
	D3DXVECTOR3 vertices[] =
	{
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),

		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),

		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),

		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),


		D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),

		D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),

		D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),

		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	if (FAILED(Direct3D11::Get().GetID3D11Device()->CreateBuffer(&bd, &InitData, &m_FigureInfo.pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}