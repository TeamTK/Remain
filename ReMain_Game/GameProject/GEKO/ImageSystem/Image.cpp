#include "Image.h"
#include "..\System\Window.h"
#include "Math.h"
#include <iostream>

Image::Image() :
	m_SizeW(0),
	m_SizeH(0),
	m_Angle(0),
	m_CenterX(0),
	m_CenterY(0)
{
}

Image::~Image()
{
	m_pImageData = nullptr;
}

void Image::InitModel(int centerX, int centerY)
{
	ImageInfo *data = m_pImageData->GetImageInfo();

	//ピクセル値からUV座標に変換
	float uvLeft = m_UvSize.left / (float)data->Width;
	float uvRight = m_UvSize.right / (float)data->Width;
	float uvTop = m_UvSize.top / (float)data->Height;
	float uvBotton = m_UvSize.bottom / (float)data->Height;

	//バーテックスバッファー作成
	Vertex vertices[] =
	{
		D3DXVECTOR3((FLOAT)-(centerX), (FLOAT)-(centerY), 0.0f), D3DXVECTOR2(uvLeft, uvTop), //頂点1
		D3DXVECTOR3((FLOAT)m_SizeW - centerX, (FLOAT)-(centerY), 0.0f), D3DXVECTOR2(uvRight, uvTop), //頂点2
		D3DXVECTOR3((FLOAT)-(centerX), (FLOAT)m_SizeH - centerY, 0.0f), D3DXVECTOR2(uvLeft, uvBotton), //頂点3
		D3DXVECTOR3((FLOAT)m_SizeW - centerX, (FLOAT)m_SizeH - centerY, 0.0f), D3DXVECTOR2(uvRight, uvBotton), //頂点4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	SAFE_RELEASE(m_pImageData->GetImageInfo()->pVertexBuffer);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	if (FAILED(Direct3D11::Get().GetID3D11Device()->CreateBuffer(&bd, &InitData, &data->pVertexBuffer)))
	{
		MessageBox(0, TEXT("バーテックスバッファー作成失敗"), NULL, MB_OK);
	}
}

int Image::GetWidth()
{
	return m_SizeW;
}

int Image::GetHeight()
{
	return m_SizeH;
}

void Image::SetAsset(const char *name)
{
	m_pImageData = ImageAsset::GetImage(name);

	ImageInfo *data = m_pImageData->GetImageInfo();

	m_RGBA = { 1.0f, 1.0f, 1.0f, 1.0f };

	//UV座標サイズ
	m_UvSize.top = static_cast<LONG>(0.0f);
	m_UvSize.left = static_cast<LONG>(0.0f);
	m_UvSize.right = static_cast<LONG>(data->Width);
	m_UvSize.bottom = static_cast<LONG>(data->Height);

	//変更するサイズを初期化
	m_SizeW = data->Width;
	m_SizeH = data->Height;

	m_CenterX = 0;
	m_CenterY = 0;

	InitModel(0, 0);
}

void Image::SetAlpha(int alpha)
{
	m_RGBA.sAlpha = (float)alpha / 255.0f;
}

void Image::SetRGB(int red, int green, int blue)
{
	m_RGBA.sRed   = (float)red   / 255.0f;
	m_RGBA.sGreen = (float)green / 255.0f;
	m_RGBA.sBlue  = (float)blue  / 255.0f;
}

void Image::SetAngle(int angle)
{
	m_Angle = angle;
}

void Image::SetSize(int w, int h)
{
	m_SizeW = w;
	m_SizeH = h;
}

void Image::SetCenter(int x, int y)
{
	m_CenterX = x;
	m_CenterY = y;
}

void Image::SetDrawPos(int x, int y, int width, int height)
{
	m_UvSize.top = y;
	m_UvSize.left = x;
	m_UvSize.right = width;
	m_UvSize.bottom = height;
}

void Image::Draw(int x, int y)
{
	InitModel(m_CenterX, m_CenterY);

	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	ImageInfo *data = m_pImageData->GetImageInfo();

	D3DXMATRIX World;

	//Z回転
	Matrix RZ;
	RZ.RotationRadianZ(Math::ChangeToRadian((float)m_Angle));

	Matrix temp;

	temp._11 = 1.0f * RZ._11;
	temp._12 = 1.0f * RZ._12;
	temp._13 = 1.0f * RZ._13;
	temp._14 = 0.0f;

	temp._21 = 1.0f * RZ._21;
	temp._22 = 1.0f * RZ._22;
	temp._23 = 1.0f * RZ._23;
	temp._24 = 0.0f;

	temp._31 = 1.0f * RZ._31;
	temp._32 = 1.0f * RZ._32;
	temp._33 = 1.0f * RZ._33;
	temp._34 = 0.0f;

	temp._41 = static_cast<float>(x);
	temp._42 = static_cast<float>(y);
	temp._43 = 0.0f;
	temp._44 = 1.0f;

	World = temp;

	//使用するシェーダーのセット
	pDeviceContext->VSSetShader(data->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(data->pPixelShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(data->pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す
		cb.mW = World;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ビューポートサイズを渡す（クライアント領域の横と縦）
		//cb.ViewPortWidth = (float)Window::Get()->GetWindowWidth();
		//cb.ViewPortHeight = (float)Window::Get()->GetWindowHeight();

		//ビューポートサイズを渡す（クライアント領域の横と縦）
		WindowSize *OriginSize = Window::Get()->GetWindowOriginSize();
		cb.ViewPortWidth = (float)OriginSize->sWidth;
		cb.ViewPortHeight = (float)OriginSize->sHeight;

		//RGBAを渡す
		cb.Color = D3DXVECTOR4(m_RGBA.sRed, m_RGBA.sGreen, m_RGBA.sBlue, m_RGBA.sAlpha);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(data->pConstantBuffer, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(data->pVertexLayout);
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &data->pSampler);
	pDeviceContext->PSSetShaderResources(0, 1, &data->pTexture);
	//プリミティブをレンダリング
	pDeviceContext->Draw(4, 0);
}