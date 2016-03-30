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

void Sphere3D::Render(const Vector3D &pos, const Vector3D &scale, const Vector3D &color)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	Matrix world;
	world.Scale(scale.x, scale.y, scale.z);
	world._41 = pos.x;
	world._42 = pos.y;
	world._43 = pos.z;

	//�g�p����V�F�[�_�[�̃Z�b�g
	pDeviceContext->VSSetShader(m_FigureInfo.pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_FigureInfo.pPixelShader, NULL, 0);

	//�ϊ��s��ƃJ���[���V�F�[�_�[�ɑ���
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBufferFiqure cb;
	if (SUCCEEDED(pDeviceContext->Map(m_FigureInfo.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		Matrix m = world * (*Camera::GetViewProjection());
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		cb.Color = Vector4D(color.x, color.y, color.z, 1.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		pDeviceContext->Unmap(m_FigureInfo.pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_FigureInfo.pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(Vector4D);
	UINT offset = 0;
	Direct3D11::GetInstance()->GetID3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_FigureInfo.pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_FigureInfo.pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(20 * (20 / 2 + 1), 0);
}

HRESULT Sphere3D::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Sphere3D.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Sphere3D.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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

Vector4D ConvertRTPToXYZ(float r, float fTheta, float fPhi)
{
	Vector4D	vResult;

	vResult.x = r * sinf(fTheta) * cosf(fPhi);
	vResult.y = r * cosf(fTheta);
	vResult.z = r * sinf(fTheta) * sinf(fPhi);
	vResult.w = 1.0f;

	return vResult;
}

HRESULT Sphere3D::InitVertex()
{
	Vector4D		vertices[20 * 20];	// ���_�f�[�^
	float				fTheta1, fTheta2;
	float				fPhi1, fPhi2;
	float				fAngleDelta;
	int verNum = 0;

	fAngleDelta = 2.0f * PI / 20;
	fTheta1 = 0.0f;
	fTheta2 = fTheta1 + fAngleDelta;
	for (int i = 0; i < 20 / 2; i++) 
	{
		fPhi1 = 0.0f;
		fPhi2 = fPhi1 + fAngleDelta;
		for (int j = 0; j < 20; j++) 
		{
			vertices[verNum] = ConvertRTPToXYZ(1.0f, fTheta1, fPhi1);	// ���_1
			vertices[verNum + 1] = ConvertRTPToXYZ(1.0f, fTheta1, fPhi2);	// ���_2
			vertices[verNum + 2] = ConvertRTPToXYZ(1.0f, fTheta2, fPhi1);	// ���_3
			//vertices = ConvertRTPToXYZ(R, fTheta2, fPhi2);	// ���_4
			verNum += 3;

			fPhi1 += fAngleDelta;
			fPhi2 += fAngleDelta;
		}
		fTheta1 += fAngleDelta;
		fTheta2 += fAngleDelta;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices) * 20;
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