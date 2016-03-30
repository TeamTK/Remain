#include "ConstantShader.h"
#include "Light\DirectionalLight.h"
#include "Light\PointLightManager.h"
#include "..\System\Camera.h"
#include "..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\Mesh\MeshInfo.h"

ConstantShader::ConstantShader()
{
	INIT_NULLPOINTR(m_pCommonInfoConstantBuffer);
	INIT_NULLPOINTR(m_pTransformMatrixConstantBuffer);
	INIT_NULLPOINTR(m_pMaterialConstantBuffer);
	INIT_NULLPOINTR(m_pBoneConstantBuffer);
	INIT_NULLPOINTR(m_pForwardConstantBuffer);
	INIT_NULLPOINTR(m_pDeferredConstantBuffer);
}

ConstantShader::~ConstantShader()
{
	Release();
}

ConstantShader *ConstantShader::GetInstance()
{
	static ConstantShader constantShader;
	return &constantShader;
}

bool ConstantShader::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	D3D11_BUFFER_DESC cb;

	//�R���X�^���g�o�b�t�@�[�쐬  ���ʏ��n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CommonInfoConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pCommonInfoConstantBuffer))) return false;

	//�ϊ��s��n���p
	cb.ByteWidth = sizeof(TransformMatrixConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pTransformMatrixConstantBuffer))) return false;

	//�}�e���A���n���p
	cb.ByteWidth = sizeof(MaterialConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pMaterialConstantBuffer))) return false;

	//�{�[���p  
	cb.ByteWidth = sizeof(BoneConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pBoneConstantBuffer))) return false;

	//�O�������_�����O�p���C�g
	cb.ByteWidth = sizeof(ForwardLightConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pForwardConstantBuffer))) return false;

	//��������_�����O�p���C�g  
	cb.ByteWidth = sizeof(DeferredLightConstantBuffer);
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pDeferredConstantBuffer))) return false;

	return true;
}

void ConstantShader::SetCommonInfoConstantBuffer()
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pCommonInfoConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		CommonInfoConstantBuffer sg;

		//���C�g�̕�����n��
		D3DXVec4Normalize(&sg.lightDir, DirectionalLight::GetDirection());
		sg.intensity = *DirectionalLight::GetLightColor();

		//���_�ʒu��n��
		Vector3D EyePos(Camera::GetEyePosition());
		sg.eyePos = Vector4D(EyePos.x, EyePos.y, EyePos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CommonInfoConstantBuffer));
		pDeviceContext->Unmap(m_pCommonInfoConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pCommonInfoConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &m_pCommonInfoConstantBuffer);
}

void ConstantShader::SetTransformMatrixConstantBuffer(ID3D11DeviceContext *pDeviceContext, Matrix &mat, bool isShadow)
{
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(
		m_pTransformMatrixConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		TransformMatrixConstantBuffer sg;

		//���[���h�s���n��
		sg.mW = mat;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);

		//���[���h�A�J�����A�ˉe�s���n��
		sg.mWVP = mat * *Camera::GetViewProjection();
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);

		//���C�g�̎��_����̃J�����̍s��
		if (isShadow)
		{
			sg.mWLP = mat * *ShaderShadowMap::GetInstance()->GetViewProjMatrix();
			D3DXMatrixTranspose(&sg.mWLP, &sg.mWLP);
		}

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(TransformMatrixConstantBuffer));
		pDeviceContext->Unmap(m_pTransformMatrixConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(1, 1, &m_pTransformMatrixConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pTransformMatrixConstantBuffer);
}

void ConstantShader::SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, 
											 const Vector4D &diffuse, const Vector4D &specular, const Vector4D &ambient)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(pDeviceContext->Map(m_pMaterialConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		MaterialConstantBuffer sg;
		sg.diffuse = diffuse;   //�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
		sg.specular = specular; //�X�y�L�����[���V�F�[�_�[�ɓn��
		sg.ambient = ambient;   //�A���r�G���g�����V�F�[�_�[�ɓn��

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(MaterialConstantBuffer));
		pDeviceContext->Unmap(m_pMaterialConstantBuffer, 0);
	}
	pDeviceContext->VSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialConstantBuffer);
}

void ConstantShader::SetBoneConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBone*> &pCopyBornArray)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//�{�[�����i�[
	if (SUCCEEDED(pDeviceContext->Map(m_pBoneConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		BoneConstantBuffer sg;
		for (int i = 0; i < bornAllNum; i++)
		{
			D3DXMatrixTranspose(&sg.boneMat[i], &pCopyBornArray[i]->boneMat);
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(BoneConstantBuffer));
		pDeviceContext->Unmap(m_pBoneConstantBuffer, 0);
	}

	pDeviceContext->VSSetConstantBuffers(3, 1, &m_pBoneConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(3, 1, &m_pBoneConstantBuffer);
}

void ConstantShader::SetForwardLightConstantBuffer(ID3D11DeviceContext *pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	auto *plight = PointLightManager::GetInstance();
	int lightNum = plight->GetForwardLightNum();
	Vector3D vec;
	Vector3D color;
	const PointLight* pPointLight = nullptr;

	//�|�C���g���C�g�i�[
	if (SUCCEEDED(pDeviceContext->Map(m_pForwardConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ForwardLightConstantBuffer sg;

		//���C�g���Ȃ��ꍇ�i�[���Ȃ�
		if (lightNum != 0)
		{
			//��������Ă���^�C�g�̐��i�[
			for (int i = 0; i < lightNum; i++)
			{
				pPointLight = plight->GetForwardLight(i);
				vec = pPointLight->GetPosition();
				color = pPointLight->GetColor();
				sg.lihgtPos[i] = Vector4D(vec.x, vec.y, vec.z, pPointLight->GetIntensity());
				sg.range[i] = Vector4D(color.x, color.y, color.z, pPointLight->GetRange());
			}
		}

		sg.lightNum = (float)lightNum; //���C�g�̍ő���i�[

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ForwardLightConstantBuffer));
		pDeviceContext->Unmap(m_pForwardConstantBuffer, 0);
	}

	pDeviceContext->VSSetConstantBuffers(4, 1, &m_pForwardConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(4, 1, &m_pForwardConstantBuffer);
}

void ConstantShader::SetDeferredLightConstantBuffer(ID3D11DeviceContext *pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	auto *plight = PointLightManager::GetInstance();
	int lightNum = plight->GetForwardLightNum();
	Vector3D vec;
	Vector3D color;
	const PointLight* pPointLight = nullptr;

	//�|�C���g���C�g�i�[
	if (SUCCEEDED(pDeviceContext->Map(m_pDeferredConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		DeferredLightConstantBuffer sg;

		//���C�g���Ȃ��ꍇ�i�[���Ȃ�
		if (lightNum != 0)
		{
			//��������Ă���^�C�g�̐��i�[
			for (int i = 0; i < lightNum; i++)
			{
				pPointLight = plight->GetDeferreLight(i);
				vec = pPointLight->GetPosition();
				sg.lihgtPos[i] = Vector4D(vec.x, vec.y, vec.z, pPointLight->GetIntensity());
				sg.range[i] = Vector4D(color.x, color.y, color.z, pPointLight->GetRange());
			}
		}

		sg.lightNum = (float)lightNum; //���C�g�̍ő���i�[

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(DeferredLightConstantBuffer));
		pDeviceContext->Unmap(m_pDeferredConstantBuffer, 0);
	}

	pDeviceContext->VSSetConstantBuffers(4, 1, &m_pDeferredConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(4, 1, &m_pDeferredConstantBuffer);
}

void ConstantShader::Release()
{
	//�萔�o�b�t�@�[
	SAFE_RELEASE(m_pCommonInfoConstantBuffer);
	SAFE_RELEASE(m_pTransformMatrixConstantBuffer);
	SAFE_RELEASE(m_pMaterialConstantBuffer);
	SAFE_RELEASE(m_pBoneConstantBuffer);
	SAFE_RELEASE(m_pForwardConstantBuffer);
	SAFE_RELEASE(m_pDeferredConstantBuffer);
}