#include "Billboard.h"
#include "..\ImageSystem\Image.h"
#include "..\System\Camera.h"

Billboard::Billboard()
{
	//�V�F�[�_�[�̏�����
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "Billboard(�V�F�[�_�[�̏������Ɏ��s���܂���", NULL, MB_OK);
	}

	//���_�쐬
	if (FAILED(InitVertex()))
	{
		MessageBoxA(0, "Billboard(���_�̃V�F�[�_�[�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
	}
}

Billboard::~Billboard()
{
}

void Billboard::Render(const Vector3D &pos, const Vector3D &scale, const std::string &name)
{
	ImageData *pImage = ImageAsset::GetImage(name);
	ImageInfo *p = pImage->GetImageInfo();

	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//�g�p����V�F�[�_�[�̃Z�b�g
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	//���s�ړ�
	world._41 = pos.x;
	world._42 = pos.y;
	world._43 = pos.z;

	//�g��k��
	world._11 = scale.x;
	world._22 = scale.y;
	world._33 = scale.z;

	D3DXMATRIX CancelRotation = (*Camera::GetView());
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0;
	D3DXMatrixInverse(&CancelRotation, NULL, &CancelRotation);
	world = CancelRotation * world;

	//�ϊ��s��ƃJ���[���V�F�[�_�[�ɑ���
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = world * (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		cb.Color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(BillboradVertex);
	UINT offset = 0;
	Direct3D11::GetInstance()->GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &p->pSampler);
	pDeviceContext->PSSetShaderResources(0, 1, &p->pTexture);

	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(4, 0);
}

HRESULT Billboard::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�o�[�e�b�N�X�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	//D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(),
										  pCompiledShader->GetBufferSize(), &m_FigureInfo.pVertexLayout)))
	{
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Billboard.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_FigureInfo.pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
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

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return S_OK;
}

HRESULT Billboard::InitVertex()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	BillboradVertex vertices[] =
	{
		D3DXVECTOR3(-1.0, -1.0,0),D3DXVECTOR2(0,1), //���_1,
		D3DXVECTOR3(-1.0, 1.0,0), D3DXVECTOR2(0,0), //���_2
		D3DXVECTOR3(1.0, -1.0,0), D3DXVECTOR2(1,1), //���_3
		D3DXVECTOR3(1.0, 1.0,0),  D3DXVECTOR2(1,0), //���_4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BillboradVertex) * 4;
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