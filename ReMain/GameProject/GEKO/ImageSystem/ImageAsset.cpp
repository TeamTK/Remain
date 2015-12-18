#include "ImageAsset.h"
#include "..\System\Window.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

ImageData::ImageData(std::string fileName)
{
	INIT_NULLPOINTR(m_ImageInfo.pSampler);
	INIT_NULLPOINTR(m_ImageInfo.pTexture);
	INIT_NULLPOINTR(m_ImageInfo.pConstantBuffer);
	INIT_NULLPOINTR(m_ImageInfo.pVertexShader);
	INIT_NULLPOINTR(m_ImageInfo.pPixelShader);
	INIT_NULLPOINTR(m_ImageInfo.pVertexBuffer);
	INIT_NULLPOINTR(m_ImageInfo.pVertexLayout);

	ID3D11Device *pDevice;
	pDevice = Direct3D11::Get().GetID3D11Device();

	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"GEKO\\HLSL\\Image.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"Sprite2D.hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_ImageInfo.pVertexShader)))
	{
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
	}

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_ImageInfo.pVertexLayout)))
	{
		MessageBox(0, L"���_�C���v�b�g���C�A�E�g�쐬���s", NULL, MB_OK);
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"GEKO\\HLSL\\Image.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"Sprite2D.hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_ImageInfo.pPixelShader)))
	{
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
	}

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_ImageInfo.pConstantBuffer)))
	{
		MessageBox(0, L"�R���X�^���g�o�b�t�@�[�쐬���s", NULL, MB_OK);
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_ImageInfo.pSampler);

	//�e�N�X�`���[�ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, fileName.c_str(), NULL, NULL, &m_ImageInfo.pTexture, NULL)))
	{
		MessageBoxA(0, "Image�N���X�̃e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
	}

	ID3D11Resource *pResource;
	m_ImageInfo.pTexture->GetResource(&pResource);

	D3D11_RESOURCE_DIMENSION type;
	pResource->GetType(&type);

	switch (type)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		ID3D11Texture2D *pTexture = (ID3D11Texture2D*)(pResource);

		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);
		m_ImageInfo.Width = desc.Width;
		m_ImageInfo.Height = desc.Height;

		break;
	}

	default:
		MessageBoxA(0, "D3D11_RESOURCE_DIMENSION_TEXTURE2D�ł͂���܂���", "", MB_OK);
		break;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	SAFE_RELEASE(pResource);
}

ImageData::~ImageData()
{
}

ImageInfo* ImageData::GetImageInfo()
{
	return &m_ImageInfo;
}

void ImageData::Relese()
{
	SAFE_RELEASE(m_ImageInfo.pSampler);
	SAFE_RELEASE(m_ImageInfo.pTexture);
	SAFE_RELEASE(m_ImageInfo.pConstantBuffer);
	SAFE_RELEASE(m_ImageInfo.pVertexShader);
	SAFE_RELEASE(m_ImageInfo.pPixelShader);
	SAFE_RELEASE(m_ImageInfo.pVertexBuffer);
	SAFE_RELEASE(m_ImageInfo.pVertexLayout);
}

class ImageAsset::ImagePimpl
{
public:
	std::map<std::string, ImageData*> map;
};

ImageAsset::ImageAsset()
{
	m_pImagePimpl = new ImagePimpl;
}

ImageAsset::~ImageAsset()
{
	delete m_pImagePimpl;
}

ImageAsset *ImageAsset::GetInstance()
{
	static ImageAsset imageAsset;
	return &imageAsset;
}

ImageData *ImageAsset::GetImage(std::string name)
{
	ImagePimpl *imagePimpl = GetInstance()->m_pImagePimpl;

	//�w��̃T�E���h�f�[�^���Ȃ��ꍇ�x�����o���E�C���h�E��~
	if (imagePimpl->map.find(name) == imagePimpl->map.end())
	{
		GetInstance()->AllClear();
		std::string str = name;
		str += "��ImageAsset�ɂ͂���܂���";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return imagePimpl->map[name];
}

void ImageAsset::LoadAsset(std::string filmeName, std::string name)
{
	ImagePimpl *imagePimpl = GetInstance()->m_pImagePimpl;

	//���łɂ���ꍇ�͌x�����o���E�B���h�E�I��
	if (imagePimpl->map.find(name) != imagePimpl->map.end())
	{
		GetInstance()->AllClear();
		std::string str = name;
		str += "�̖��O�͑��݂��܂�";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	imagePimpl->map[name] = new ImageData(filmeName);
}

void ImageAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "�ǂݍ��݂Ɏ��s���܂���";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "Image")
		{
			ifs >> str; std::string texfileName = str;
			ifs >> str; std::string Name = str;
			ImageAsset::LoadAsset(texfileName, Name.c_str());
		}
	}
}

void ImageAsset::PartClear(std::string name)
{
	ImagePimpl *imagePimpl = GetInstance()->m_pImagePimpl;

	auto it = imagePimpl->map.find(name);
	if (it != imagePimpl->map.end())
	{
		std::cout << it->first << "�͍폜����܂���" << "\n";
		it->second->Relese();
		delete it->second;
		imagePimpl->map.erase(it);
	}
}

void ImageAsset::AllClear()
{
	ImagePimpl *imagePimpl = GetInstance()->m_pImagePimpl;

	//ImageData�̃f�[�^�������[�X
	auto it = imagePimpl->map.begin();
	for (; it != imagePimpl->map.end(); it++)
	{
		std::cout << it->first << "�͍폜����܂���\n";
		it->second->Relese();
		delete it->second;
	}

	//�S�Ă̗v�f���폜
	imagePimpl->map.clear();

	OutputDebugString(L"ImageData��S�č폜���܂���\n");
}

void ImageAsset::DebugDraw()
{
	ImagePimpl *imagePimpl = GetInstance()->m_pImagePimpl;

	//���݂���摜�f�[�^��\��
	auto it = imagePimpl->map.begin();
	for (; it != imagePimpl->map.end(); it++)
	{
		std::cout << it->first << "\n";
	}
}