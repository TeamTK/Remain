#include "StaticMeshShader.h"
#include "..\..\System\Camera.h"
#include "..\..\System\DirectionalLight.h"
#include "..\..\Mesh\MeshData.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"

StaticMeshShader::StaticMeshShader()
{
	INIT_NULLPOINTR(m_pConstantBuffer0);
	INIT_NULLPOINTR(m_pMaterialConstantBuffer);
	//INIT_NULLPOINTR(m_pConstantBuffer2);
	INIT_NULLPOINTR(m_pVertexShader);
	INIT_NULLPOINTR(m_pVertexShader_NoTexture);
	INIT_NULLPOINTR(m_pVertexShader_ShadowMap);
	INIT_NULLPOINTR(m_pPixelShader);
	INIT_NULLPOINTR(m_pPixelShader_NoTexture);
	INIT_NULLPOINTR(m_pPixelShader_ShadowMap);
	INIT_NULLPOINTR(m_pVertexLayout);
	//INIT_NULLPOINTR(m_pPSClassLinkage);
	//INIT_NULLPOINTR(m_pClassInstance);
}

StaticMeshShader::~StaticMeshShader()
{
	Release();
}

StaticMeshShader *StaticMeshShader::GetInstance()
{
	static StaticMeshShader staticMeshShader;
	return &staticMeshShader;
}

bool StaticMeshShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//���_�V�F�[�_�[������
	if (!InitVertexShader(pDevice))
	{
		MessageBox(0, TEXT("���_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�e�N�X�`���[�Ȃ����_�V�F�[�_�[������
	if (!InitVertexShader_NoTexture(pDevice))
	{
		MessageBox(0, TEXT("�e�N�X�`���[�Ȃ����_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//���_�V�F�[�_�[������
	if (!InitVertexShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("�V���h�E�}�b�v���_�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�s�N�Z���V�F�[�_�[������
	if (!InitPixelShader(pDevice))
	{
		MessageBox(0, TEXT("�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[������
	if (!InitPixelShader_NoTexture(pDevice))
	{
		MessageBox(0, TEXT("�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//�e�N�X�`���[�Ȃ��s�N�Z���V�F�[�_�[������
	if (!InitPixelShader_ShadowMap(pDevice))
	{
		MessageBox(0, TEXT("�V���h�E�}�b�v�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return false;
	}

	//���I�����N�쐬
//	pDevice->CreateClassLinkage(&m_pPSClassLinkage);
//	pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), m_pPSClassLinkage, &m_pPixelShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0))) return false;

	//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pMaterialConstantBuffer))) return false;

	/*
	//�R���X�^���g�o�b�t�@�[�쐬  ���I�����N
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer2))) return E_FAIL;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	*/

	return true;
}

void StaticMeshShader::BaseConstantBuffer(ID3D11DeviceContext *pDeviceContext, const Matrix &mat, bool isShadow)
{
	D3DXMATRIX World = mat; //���[���h�s��i�[

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 sg;

		//���[���h�s���n��
		sg.mW = World;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//���[���h�A�J�����A�ˉe�s���n��
		sg.mWVP = World * (*Camera::GetView()) * (*Camera::GetProjection());
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		//���C�g�̎��_����̃J�����̍s��
		if (isShadow)
		{
			D3DXMATRIX v = *ShaderShadowMap::GetInstance()->GetViewMatrix();
			D3DXMATRIX p = *ShaderShadowMap::GetInstance()->GetProjMatrix();

			sg.mWLP = World * v * p;
			D3DXMatrixTranspose(&sg.mWLP, &sg.mWLP);
		}

		//���C�g�̕�����n��
		D3DXVec4Normalize(&sg.LightDir, DirectionalLight::GetDirection());
		sg.fIntensity = DirectionalLight::GetLightColor()->Change();

		//���_�ʒu��n��
		D3DXVECTOR3 EyePos(Camera::GetEyePositionD3D());
		sg.eyePos = D3DXVECTOR4(EyePos.x, EyePos.y, EyePos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer0));
		pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
}

void StaticMeshShader::MaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, D3DXVECTOR4 &diffuse, D3DXVECTOR4 &specular, D3DXVECTOR4 &ambient)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(m_pMaterialConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer1 sg;
		sg.diffuse = diffuse;   //�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
		sg.specular = specular; //�X�y�L�����[���V�F�[�_�[�ɓn��
		sg.ambient = ambient;   //�A���r�G���g�����V�F�[�_�[�ɓn��

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer1));
		pDeviceContext->Unmap(m_pMaterialConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pMaterialConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pMaterialConstantBuffer);
}

void StaticMeshShader::SetVertexShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//�e�N�X�`������
	if (isTexture)
	{
		//�e����
		if (isShadow)
		{
			pDeviceContext->VSSetShader(m_pVertexShader_ShadowMap, NULL, 0);
		}
		else
		{
			pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		}
	}
	else
	{
		pDeviceContext->VSSetShader(m_pVertexShader_NoTexture, NULL, 0);
	}
}

void StaticMeshShader::SetPixelShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow)
{
	//�e�N�X�`������
	if (isTexture)
	{
		//�e����
		if (isShadow)
		{
			pDeviceContext->PSSetShader(m_pPixelShader_ShadowMap, NULL, 0);
			ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		}
		else
		{
			pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		}
	}
	else
	{
		pDeviceContext->PSSetShader(m_pPixelShader_NoTexture, NULL, 0);
	}
}

void StaticMeshShader::SetInputLayout(ID3D11DeviceContext *pDeviceContext, bool isTexture)
{
	if (isTexture)
	{
		pDeviceContext->IASetInputLayout(m_pVertexLayout); 
	}
	else
	{
		pDeviceContext->IASetInputLayout(m_pVertexLayout_NoTexture);
	}

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*
void StaticMeshShader::Begin(const char* classInstanceName)
{
	//m_pPSClassLinkage->GetClassInstance(classInstanceName, 0, &m_pClassInstance);
	//m_pPSClassLinkage->CreateClassInstance(classInstanceName, NULL, NULL, NULL, NULL, &m_pClassInstance);

	//ID3D11DeviceContext *context = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	// �s�N�Z���V�F�[�_�[�𓮓I�V�F�[�_�[�����N�Ƃ��Đݒ肷��
	//context->PSSetShader(m_pPixelShader, &m_pClassInstance, 1);

	// ���_�V�F�[�_�[�ɒ萔�o�b�t�@��ݒ肷��
	//context->VSSetConstantBuffers(2, 1, &m_pConstantBuffer2);

	// �s�N�Z���V�F�[�_�[�ɒ萔�o�b�t�@��ݒ肷��
	//context->PSSetConstantBuffers(2, 1, &m_pConstantBuffer2);
}

void StaticMeshShader::End()
{
//	SAFE_RELEASE(m_pClassInstance);
}
*/

void StaticMeshShader::Release()
{
	//�R���X�^���g�o�b�t�@�[
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pMaterialConstantBuffer);
	//SAFE_RELEASE(m_pConstantBuffer2);

	//�V�F�[�_�[
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexShader_NoTexture);
	SAFE_RELEASE(m_pVertexShader_ShadowMap);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pPixelShader_NoTexture);
	SAFE_RELEASE(m_pPixelShader_ShadowMap);

	//���_���C�A�E�g
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexLayout_NoTexture);
	
	//SAFE_RELEASE(m_pPSClassLinkage);
	//SAFE_RELEASE(m_pClassInstance);
}