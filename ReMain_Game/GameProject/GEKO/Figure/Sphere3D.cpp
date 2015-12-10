#include "Sphere3D.h"
#include "..\System\Camera.h"

Sphere3D::Sphere3D()
{
	//�V�F�[�_�[�̏�����
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "Sphere3D�V�F�[�_�[�̏������Ɏ��s���܂���", NULL, MB_OK);
	}

	//���_�쐬
	if (FAILED(InitVertex()))
	{
		MessageBoxA(0, "Sphere3D���_�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
	}
}

Sphere3D::~Sphere3D()
{
}

void Sphere3D::Render(const Matrix &World, const Vector3D &Color)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//�g�p����V�F�[�_�[�̃Z�b�g
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	//�ϊ��s��ƃJ���[���V�F�[�_�[�ɑ���
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = *(D3DXMATRIX*)(&World) *(*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		cb.Color = D3DXVECTOR4(Color.x, Color.y, Color.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	Direct3D11::Get().GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(3*8, 0);
}

HRESULT Sphere3D::InitVertex()
{	//�o�[�e�b�N�X�o�b�t�@�[�쐬
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