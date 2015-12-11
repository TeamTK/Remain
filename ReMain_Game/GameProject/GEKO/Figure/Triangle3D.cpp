#include "Triangle3D.h"
#include "..\System\Camera.h"

Triangle3D::Triangle3D()
{
	//�V�F�[�_�[�̏�����
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "Triangle3D�V�F�[�_�[�̏������Ɏ��s���܂���", NULL, MB_OK);
	}

	//���_�쐬
	if (FAILED(InitVertex()))
	{
		MessageBoxA(0, "Triangle3D���_�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
	}
}

Triangle3D::~Triangle3D()
{
	SAFE_RELEASE(m_pConstantBufferTriangle3D);
}

void Triangle3D::Render(const Vector3D &pos1, const Vector3D &pos2, const Vector3D &pos3, const Vector3D &color)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::Get().GetID3D11DeviceContext();

	//�g�p����V�F�[�_�[�̃Z�b�g
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//�r���[�A�ˉe�s���n��
		D3DXMATRIX m = (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		cb.Color = D3DXVECTOR4(color.x, color.y, color.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//���W���V�F�[�_�[�ɑ���
	if (SUCCEEDED(pDeviceContext->Map(m_pConstantBufferTriangle3D, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBufferTriangle3D cbLine3D;

		cbLine3D.pos[0] = D3DXVECTOR4(pos1.x, pos1.y, pos1.z, 1.0f);
		cbLine3D.pos[1] = D3DXVECTOR4(pos2.x, pos2.y, pos2.z, 1.0f);
		cbLine3D.pos[2] = D3DXVECTOR4(pos3.x, pos3.y, pos3.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cbLine3D), sizeof(cbLine3D));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferTriangle3D);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBufferTriangle3D);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(UINT);
	UINT offset = 0;
	Direct3D11::Get().GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(3, 0);
}

HRESULT Triangle3D::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::Get().GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"GEKO\\HLSL\\Triangle3D.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 1;
	memcpy(&layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(
		&layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_FigureInfo.pVertexLayout)))
	{
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"GEKO\\HLSL\\Triangle3D.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
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

	//�R���X�^���g�o�b�t�@�[�쐬�@���W�n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferTriangle3D);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBufferTriangle3D)))
	{
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return S_OK;
}

HRESULT Triangle3D::InitVertex()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	UINT vertices[] =
	{
		UINT(0), UINT(1), UINT(2)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * 3;
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