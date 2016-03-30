#include "Image.h"
#include "..\System\Window.h"
#include "Math.h"
#include <iostream>
#include <assert.h>

Image::Image() :
	m_IsDrawRegister(false),
	m_SizeW(0),
	m_SizeH(0),
	m_Angle(0),
	m_CenterX(0),
	m_CenterY(0)
{
	m_RenderingInfo.priorityGroup = 0;
	m_RenderingInfo.priority = 0;
	m_RenderingInfo.func = std::bind(&Image::Draw, this);
}

Image::Image(const std::string &assetName, unsigned int priorityGroup, unsigned int priority) :
	m_IsDrawRegister(true),
	m_SizeW(0),
	m_SizeH(0),
	m_Angle(0),
	m_CenterX(0),
	m_CenterY(0)
{
	m_pImageData = ImageAsset::GetImage(assetName);
	ImageInfo *data = m_pImageData->GetImageInfo();

	//�����_�����O�Ǘ��ɓo�^
	m_RenderingInfo.priorityGroup = priorityGroup;
	m_RenderingInfo.priority = priority;
	m_RenderingInfo.func = std::bind(&Image::Draw, this);
	RenderingManager::GetInstance()->AddImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);

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

Image::~Image()
{
	if (m_IsDrawRegister)
	{
		RenderingManager::GetInstance()->ClearImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
	m_pImageData = nullptr;
}

int Image::GetWidth() const
{
	return m_SizeW;
}

int Image::GetHeight() const
{
	return m_SizeH;
}

void Image::SetDrawRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsDrawRegister != isRegister&& "�`�悪���łɓo�^����Ă��܂�");

	if (isRegister)
	{
		//�����_�����O�Ǘ��ɓo�^
		m_RenderingInfo.priorityGroup = priorityGroup;
		m_RenderingInfo.priority = priority;
		RenderingManager::GetInstance()->AddImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
	else
	{
		RenderingManager::GetInstance()->ClearImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
	m_IsDrawRegister = isRegister;
}

void Image::SetAsset(const std::string &assetName)
{
	m_pImageData = ImageAsset::GetImage(assetName);
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

void Image::SetDrawPriority(unsigned int priorityGroup, unsigned int priority)
{
	assert(m_IsDrawRegister && "�`�悪�o�^����Ă��܂���");

	if (m_RenderingInfo.priorityGroup == priorityGroup)
	{
		m_RenderingInfo.priority = priority;
		RenderingManager::GetInstance()->SortImage(m_RenderingInfo.priorityGroup);
	}
	else
	{
		RenderingManager::GetInstance()->ClearImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
		m_RenderingInfo.priorityGroup = priorityGroup;
		m_RenderingInfo.priority = priority;
		RenderingManager::GetInstance()->AddImage(m_RenderingInfo.priorityGroup, &m_RenderingInfo);
	}
}

void Image::SetAlpha(int alpha)
{
	m_RGBA.sAlpha = (float)alpha * RGB;
}

void Image::SetRGB(int red, int green, int blue)
{
	m_RGBA.sRed   = (float)red   * RGB;
	m_RGBA.sGreen = (float)green * RGB;
	m_RGBA.sBlue  = (float)blue  * RGB;
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

void Image::SetDrawArea(int leftX, int leftY, int rightX, int rightY)
{
	m_UvSize.top = leftY;
	m_UvSize.left = leftX;
	m_UvSize.right = rightX;
	m_UvSize.bottom = rightY;
}

void Image::SetPosition(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void Image::SetPosition(const Vector2D &position)
{
	m_Pos = position;
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
		Vector3D((float)-(centerX), (float)-(centerY), 0.0f), Vector2D(uvLeft, uvTop), //���_1
		Vector3D((float)m_SizeW - centerX, (float)-(centerY), 0.0f), Vector2D(uvRight, uvTop), //���_2
		Vector3D((float)-(centerX), (float)m_SizeH - centerY, 0.0f), Vector2D(uvLeft, uvBotton), //���_3
		Vector3D((float)m_SizeW - centerX, (float)m_SizeH - centerY, 0.0f), Vector2D(uvRight, uvBotton), //���_4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	SAFE_RELEASE(data->pVertexBuffer);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	if (FAILED(Direct3D11::GetInstance()->GetID3D11Device()->CreateBuffer(&bd, &InitData, &data->pVertexBuffer)))
	{
		MessageBox(0, TEXT("�o�[�e�b�N�X�o�b�t�@�[�쐬���s"), NULL, MB_OK);
	}
}

void Image::Draw()
{
	Direct3D11 *pDiret11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDiret11->GetID3D11DeviceContext();
	ImageInfo *data = m_pImageData->GetImageInfo();

	pDiret11->ChangeBlendState2D();
	pDiret11->DepthOff();

	//�s�N�Z���l����UV���W�ɕϊ�
	float uvLeft = m_UvSize.left / (float)data->Width;
	float uvRight = m_UvSize.right / (float)data->Width;
	float uvTop = m_UvSize.top / (float)data->Height;
	float uvBotton = m_UvSize.bottom / (float)data->Height;

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	Vertex vertices[] =
	{
		Vector3D((float)-(m_CenterX), (float)-(m_CenterY), 0.0f), Vector2D(uvLeft, uvTop), //���_1
		Vector3D((float)m_SizeW - m_CenterX, (float)-(m_CenterY), 0.0f), Vector2D(uvRight, uvTop), //���_2
		Vector3D((float)-(m_CenterX), (float)m_SizeH - m_CenterY, 0.0f), Vector2D(uvLeft, uvBotton), //���_3
		Vector3D((float)m_SizeW - m_CenterX, (float)m_SizeH - m_CenterY, 0.0f), Vector2D(uvRight, uvBotton), //���_4
	};

	//���_�X�V
	pDeviceContext->UpdateSubresource(data->pVertexBuffer, 0, nullptr, vertices, 0, 0);

	Matrix World;

	//Z��]
	Matrix RZ;
	RZ.RotationRadianZ(Math::ChangeToRadian((float)m_Angle));

	World._11 = 1.0f * RZ._11;
	World._12 = 1.0f * RZ._12;
	World._13 = 1.0f * RZ._13;
	World._14 = 0.0f;

	World._21 = 1.0f * RZ._21;
	World._22 = 1.0f * RZ._22;
	World._23 = 1.0f * RZ._23;
	World._24 = 0.0f;

	World._31 = 1.0f * RZ._31;
	World._32 = 1.0f * RZ._32;
	World._33 = 1.0f * RZ._33;
	World._34 = 0.0f;

	World._41 = m_Pos.x;
	World._42 = m_Pos.y;
	World._43 = 0.0f;
	World._44 = 1.0f;

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
		const WindowSize *OriginSize = Window::Get()->GetWindowOriginSize();

		cb.ViewPortWidth = (float)OriginSize->width;
		cb.ViewPortHeight = (float)OriginSize->height;

		//RGBA��n��
		cb.Color = Vector4D(m_RGBA.sRed, m_RGBA.sGreen, m_RGBA.sBlue, m_RGBA.sAlpha);

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
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetInputLayout(data->pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &data->pSampler);
	pDeviceContext->PSSetShaderResources(0, 1, &data->pTexture);

	pDeviceContext->Draw(4, 0); //�v���~�e�B�u�������_�����O

	pDiret11->ChangeBlendState3D();
	pDiret11->DepthOn();
}