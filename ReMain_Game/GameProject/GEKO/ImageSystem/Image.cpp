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

	//�s�N�Z���l����UV���W�ɕϊ�
	float uvLeft = m_UvSize.left / (float)data->Width;
	float uvRight = m_UvSize.right / (float)data->Width;
	float uvTop = m_UvSize.top / (float)data->Height;
	float uvBotton = m_UvSize.bottom / (float)data->Height;

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	Vertex vertices[] =
	{
		D3DXVECTOR3((FLOAT)-(centerX), (FLOAT)-(centerY), 0.0f), D3DXVECTOR2(uvLeft, uvTop), //���_1
		D3DXVECTOR3((FLOAT)m_SizeW - centerX, (FLOAT)-(centerY), 0.0f), D3DXVECTOR2(uvRight, uvTop), //���_2
		D3DXVECTOR3((FLOAT)-(centerX), (FLOAT)m_SizeH - centerY, 0.0f), D3DXVECTOR2(uvLeft, uvBotton), //���_3
		D3DXVECTOR3((FLOAT)m_SizeW - centerX, (FLOAT)m_SizeH - centerY, 0.0f), D3DXVECTOR2(uvRight, uvBotton), //���_4
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
		MessageBox(0, TEXT("�o�[�e�b�N�X�o�b�t�@�[�쐬���s"), NULL, MB_OK);
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

	//UV���W�T�C�Y
	m_UvSize.top = static_cast<LONG>(0.0f);
	m_UvSize.left = static_cast<LONG>(0.0f);
	m_UvSize.right = static_cast<LONG>(data->Width);
	m_UvSize.bottom = static_cast<LONG>(data->Height);

	//�ύX����T�C�Y��������
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

	//Z��]
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

	//�g�p����V�F�[�_�[�̃Z�b�g
	pDeviceContext->VSSetShader(data->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(data->pPixelShader, NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(pDeviceContext->Map(data->pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��
		cb.mW = World;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
		//cb.ViewPortWidth = (float)Window::Get()->GetWindowWidth();
		//cb.ViewPortHeight = (float)Window::Get()->GetWindowHeight();

		//�r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
		WindowSize *OriginSize = Window::Get()->GetWindowOriginSize();
		cb.ViewPortWidth = (float)OriginSize->sWidth;
		cb.ViewPortHeight = (float)OriginSize->sHeight;

		//RGBA��n��
		cb.Color = D3DXVECTOR4(m_RGBA.sRed, m_RGBA.sGreen, m_RGBA.sBlue, m_RGBA.sAlpha);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(data->pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &data->pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &data->pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(data->pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &data->pSampler);
	pDeviceContext->PSSetShaderResources(0, 1, &data->pTexture);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(4, 0);
}