#include "LoadXDynamic.h"
#include "..\Mesh\LoadXStatic.h"
#include "..\System\Window.h"
#include <sstream>
#include <chrono>

struct SkinWeightInfo
{
	int listNumAll;
	std::vector<int> weightList;
	std::vector<float> weight;
	D3DXMATRIX offsetMat;
	SkinWeightInfo()
	{
		D3DXMatrixIdentity(&offsetMat);
	}
};

struct AnimationInfo
{
	std::vector<D3DXVECTOR4> rotation;
	std::vector<D3DXVECTOR3> scale;
	std::vector<D3DXVECTOR3> position;
};

LoadXDynamic::LoadXDynamic(std::string fileName) :
	m_Back(-1),
	m_BornIndex(0),
	m_AnimSetNum(0),
	m_FrameAnim(0.0f)
{
	//���b�V���̓ǂݍ���
	if (FAILED(LoadXMesh(fileName)))
	{
		MessageBoxA(0, "�X�L�����b�V���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}

	//�V�F�[�_�[�̏�����
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "�V�F�[�_�[�̏������Ɏ��s���܂���", NULL, MB_OK);
	}
}

LoadXDynamic::~LoadXDynamic()
{
	//�{�[�����X�g�폜
	m_BornInfo.BornList.clear();
	m_BornInfo.BornList.shrink_to_fit();

	//�A�j���[�V�����t���[����
	auto it3 = m_BornInfo.AnimationSetFrameNum.begin();
	for (; it3 != m_BornInfo.AnimationSetFrameNum.end(); it3++)
	{
		it3->second.clear();
		it3->second.shrink_to_fit();
	}
	m_BornInfo.AnimationSetFrameNum.clear();

	//�t���[���K�w�\���폜
	DeleteHierarchy(m_BornInfo.sBorn.child);
}

HRESULT LoadXDynamic::LoadXMesh(std::string fileName)
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::Get().GetID3D11Device();

	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, fileName.c_str(), "rt");
	if (fp == NULL)
	{
		fileName += "�ǂݍ��ݎ��s";
		MessageBoxA(0, fileName.c_str(), "", MB_OK);
		Window::Get()->WindowEnd();
		return FALSE;
	}

	std::vector<int> face;
	std::vector<int> normalIndex;
	std::vector<D3DXVECTOR3> coordinate;
	std::vector<D3DXVECTOR3> normal;
	std::vector<D3DXVECTOR2> uv;
	std::vector<MaterialX> material;
	std::vector<int> materialList;

	int index1, index2, index3;
	float x, y, z;

	char str[400] = { 0 };
	char key[400] = { 0 };

	//���_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s", key, sizeof(key));

		//�e���v���[�g�ł̊ԈႢ�h�~
		if (strcmp(key, "template") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			continue;
		}

		//���_���b�V���ǂݍ���
		if (strcmp(key, "Mesh") == 0)
		{
			//���ߔg���ʑ΍�
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "Mesh�̏��ߔg���ʂ�����������܂���");

			fgets(str, sizeof(str), fp); //�ǂݍ��݃f�[�^���s

			//���_��
			fscanf_s(fp, "%d;", &m_MeshInfo.vertexNumAll);

			//���_���W�ǂݍ���
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				fscanf_s(fp, "%f; %f; %f;,", &x, &y, &z);
				coordinate.emplace_back(x, y, z);
			}

			//�ʂ̐�
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.faceNumAll);

			//�ʂ̃C���f�b�N�X�ԍ��ǂݍ���
			for (int i = 0; i < m_MeshInfo.faceNumAll; i++)
			{
				int faceNum;
				fscanf_s(fp, "%d; %d, %d, %d;,", &faceNum, &index1, &index2, &index3);

				assert(faceNum == 3 && "X�t�@�C����3D���f���͎O�p�`�|���S���ł͂���܂���");

				face.emplace_back(index1); face.emplace_back(index2); face.emplace_back(index3);
			}
		}

		//�@���ǂݍ���
		if (strcmp(key, "MeshNormals") == 0)
		{
			//���ߔg���ʑ΍�
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "�@���̏��ߔg���ʂ�����������܂���");

			//�@���̐�
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.normalNumAll);

			//�@���ǂݍ���
			for (int i = 0; i < m_MeshInfo.normalNumAll; i++)
			{
				fscanf_s(fp, "%f; %f; %f;,", &x, &y, &z);
				normal.emplace_back(x, y, z);
			}

			//�@���̐�
			int NormalNumAll = 0;
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &NormalNumAll);

			//�@���̃C���f�b�N�X�ԍ��ǂݍ���
			int NormalNum = 0;
			for (int i = 0; i < NormalNumAll; i++)
			{
				fscanf_s(fp, "%d; %d, %d, %d;,", &NormalNum, &index1, &index2, &index3);

				assert(NormalNum == 3 && "X�t�@�C����3D���f���͖@�����O�ł͂���܂���");

				normalIndex.emplace_back(index1);
				normalIndex.emplace_back(index2);
				normalIndex.emplace_back(index3);
			}
		}

		//UV���W�ǂݍ���
		if (strcmp(key, "MeshTextureCoords") == 0)
		{
			//���ߔg���ʑ΍�
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "UV���W�̏��ߔg���ʂ�����������܂���");

			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.uvNumAll);

			for (int i = 0; i < m_MeshInfo.uvNumAll; i++)
			{
				fscanf_s(fp, "%f; %f;,", &x, &y);
				uv.emplace_back(x, y);
			}
		}

		//�}�e���A�����X�g�ǂݍ���
		if (strcmp(key, "MeshMaterialList") == 0)
		{
			//���ߔg���ʑ΍�
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "�}�e���A�����X�g���ߔg���ʂ�����������܂���");

			//�}�e���A���̐�
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.materialNumAll);

			//�}�e���A�����X�g�̐�
			fscanf_s(fp, "%d;", &m_MeshInfo.materialListNumAll);

			//�}�e���A�����X�g�i�[
			int listNum = 0;
			for (int i = 0; i < m_MeshInfo.materialListNumAll; i++)
			{
				fscanf_s(fp, "%d,", &listNum);
				materialList.emplace_back(listNum);
			}
		}

		//�}�e���A���ǂݍ���
		if (strcmp(key, "Material") == 0)
		{
			char materialName[400] = { 0 };
			fscanf_s(fp, " %s", &materialName, sizeof(materialName));

			//���ߔg���ʑ΍�
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "�}�e���A���̏��ߔg���ʂ�����������܂���");

			fgets(str, sizeof(str), fp);

			D3DXVECTOR4 kd(0.0f, 0.0f, 0.0f, 0.0f);
			float sp = 0.0f;
			D3DXVECTOR3 ks(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 ka(0.0f, 0.0f, 0.0f);
			std::string textureName;

			//�}�e���A�������ǂݍ���
			fscanf_s(fp, "%f; %f; %f; %f;;", &kd.x, &kd.y, &kd.z, &kd.w);
			fscanf_s(fp, "%f;", &sp);
			fscanf_s(fp, "%f; %f; %f;;", &ks.x, &ks.y, &ks.z);
			fscanf_s(fp, "%f; %f; %f;;", &ka.x, &ka.y, &ka.z);

			//�e�N�X�`���[�ǂݍ���
			fscanf_s(fp, "%s", &str, sizeof(str));
			if (strcmp(str, "TextureFilename") == 0)
			{
				//���ߔg���ʑ΍�
				char leftCurlyBracket;
				fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

				assert(leftCurlyBracket == '{' && "�e�N�X�`���̏��ߔg���ʂ�����������܂���");

				fscanf_s(fp, "%s", &str, sizeof(str));
				textureName = str;

				//�]���ȕ����폜
				for (int i = 0; i < (int)textureName.size(); i++)
				{
					if (textureName[i] == '"')
					{
						textureName.erase(textureName.begin() + i);
					}

					if (textureName[i] == ';')
					{
						textureName.erase(textureName.begin() + i);
					}

					if (textureName[i] == '}')
					{
						textureName.erase(textureName.begin() + i);
					}
				}

				std::string temp = fileName;
				std::string temp2;

				for (int i = 0, j = 0; i < (int)temp.size(); i++)
				{
					if (temp[i] == '\\')
					{
						for (; j < i; j++) temp2 += temp[j];
						temp2 += '\\';
						j++;
					}
				}

				material.emplace_back(kd, ks, ka, temp2 + textureName);
			}
			else
			{
				material.emplace_back(kd, ks, ka, "NoTexture");
			}
		}
	}

	fclose(fp);

	//�}�e���A���ƃC���f�b�N�X�o�b�t�@�[���I�쐬
	m_MeshInfo.m_pMaterial = new SkinMaterialInfo[m_MeshInfo.materialNumAll];
	m_MeshInfo.m_ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];
	m_MeshInfo.pvVertex = new SkinVertexInfo[m_MeshInfo.vertexNumAll];

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�}�e���A���̐����C���f�b�N�X�o�b�t�@�[�쐬
	int count = 0;
	int *faceBuffer = nullptr;
	for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
	{
		faceBuffer = new int[m_MeshInfo.faceNumAll * 3];
		count = 0;

		//�}�e���A�����蓖��
		m_MeshInfo.m_pMaterial[i].Ambient = material[i].Ambient;
		m_MeshInfo.m_pMaterial[i].Diffuse = material[i].Diffuse;
		m_MeshInfo.m_pMaterial[i].Specular = material[i].Specular;

		m_MeshInfo.m_pMaterial[i].TextureName = material[i].TextureName;

		if (material[i].TextureName == "NoTexture")
		{
			m_MeshInfo.m_IsTexture = false;
		}
		else
		{
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_MeshInfo.m_pMaterial[i].TextureName.c_str(), NULL, NULL, &m_MeshInfo.m_pMaterial[i].pTexture, NULL)))
			{
				MessageBoxA(NULL, "�e�N�X�`���[�̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				return E_FAIL;
			}
		}

		//�Ή�����}�e���A���ɖʏ��i�[
		for (int j = 0; j < m_MeshInfo.faceNumAll; j++)
		{
			if (i == materialList[j])
			{
				faceBuffer[count] = face[j * 3];
				faceBuffer[count + 1] = face[j * 3 + 1];
				faceBuffer[count + 2] = face[j * 3 + 2];
				m_MeshInfo.m_pMaterial[i].dwNumFace += 1;
				count += 3;
			}
		}
		if (count == 0) continue;

		//�C���f�b�N�X�o�b�t�@�[���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * m_MeshInfo.m_pMaterial[i].dwNumFace * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = faceBuffer;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.m_ppIndexBuffer[i])))
			return FALSE;

		delete[] faceBuffer;
		faceBuffer = nullptr;
	}

	SAFE_DELETE_ARRAY(faceBuffer);
	//���_�Ɋ��蓖��
	if (uv.size() >= 1)
	{
		//uv�L��o�[�W����
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[face[i]].vNormal = normal[normalIndex[i]];
			m_MeshInfo.pvVertex[i].vTex = uv[i];
		}
	}
	else
	{
		//uv�Ȃ��o�[�W����
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[face[i]].vNormal = normal[normalIndex[i]];
		}
	}

	//�{�[���ǂݍ���
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		std::cerr << "���s" << std::endl;
	}

	AddBoneHierarchy(&m_BornInfo.sBorn, &ifs, 0); //�{�[���K�w�\���ǂݍ���

	LoadAnimation(fileName, m_MeshInfo.pvVertex); //�A�j���[�V�����ƃX�L���E�F�C�g�ǂݍ���

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_MeshInfo.pvVertex;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.m_pVertexBuffer)))
		return FALSE;

	//�폜
	face.clear();
	face.shrink_to_fit();
	normalIndex.clear();
	normalIndex.shrink_to_fit();
	coordinate.clear();
	coordinate.shrink_to_fit();
	normal.clear();
	normal.shrink_to_fit();
	uv.clear();
	uv.shrink_to_fit();
	material.clear();
	material.shrink_to_fit();
	materialList.clear();
	materialList.shrink_to_fit();

	return S_OK;
}

void LoadXDynamic::Relese()
{
	if (m_MeshInfo.m_pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.m_pMaterial[i].pTexture);
			m_MeshInfo.m_ppIndexBuffer[i]->Release();
		}

		SAFE_RELEASE(m_MeshInfo.m_pConstantBufferBone);
		SAFE_DELETE_ARRAY(m_MeshInfo.pvVertex);
		SAFE_DELETE_ARRAY(m_MeshInfo.m_pMaterial);
		SAFE_DELETE_ARRAY(m_MeshInfo.m_ppIndexBuffer);
		SAFE_RELEASE(m_MeshInfo.m_Nothing);
		SAFE_RELEASE(m_MeshInfo.m_pConstantBuffer0);
		SAFE_RELEASE(m_MeshInfo.m_pConstantBuffer1);
		SAFE_RELEASE(m_MeshInfo.m_pVertexShader);
		SAFE_RELEASE(m_MeshInfo.m_pPixelShader);
		SAFE_RELEASE(m_MeshInfo.m_pVertexBuffer);
		SAFE_RELEASE(m_MeshInfo.m_pVertexLayout);
		SAFE_RELEASE(m_MeshInfo.m_pSampleLinear);
	}
}

void LoadXDynamic::ChangeAnimation(unsigned int num)
{
	m_AnimSetNum = num;
	unsigned int AnimSetAllNum = m_BornInfo.AnimationSetFrameNum.size() - 1;
	if (m_AnimSetNum > AnimSetAllNum) m_AnimSetNum = AnimSetAllNum;
	else if (m_AnimSetNum < 0) m_AnimSetNum = 0;
}

void LoadXDynamic::SetRenewalTime(float animSpeed)
{
	m_FrameAnim += Math::VelocityToFrameM(animSpeed);
}

void LoadXDynamic::SetTime(float animTime)
{
	m_FrameAnim = animTime;
}

float LoadXDynamic::GetPlayTime()
{
	return m_FrameAnim;
}

int LoadXDynamic::GetPlayAnimation()
{
	return m_AnimSetNum;
}

int LoadXDynamic::GetBornAllNum()
{
	return m_BornInfo.BornList.size();
}

std::string LoadXDynamic::GetBornName(int bornIndex)
{
	return m_BornInfo.BornList[bornIndex]->BornName;
}

Matrix LoadXDynamic::GetBornWorld(int bornIndex)
{
	return m_BornInfo.BornList[bornIndex]->ParentAndChildMat;
}

Vector3D LoadXDynamic::GetBornPos(int bornIndex)
{
	Matrix m = m_BornInfo.BornList[bornIndex]->ParentAndChildMat;
	return Vector3D(m._41, m._42, m._43);
}

void LoadXDynamic::Update()
{
	//�A�j���[�V�����X�V
	AnimUpdate(m_BornInfo.sBorn.child);

	D3DXMATRIX m;;
	D3DXMatrixIdentity(&m);

	//�{�[���X�V
	BornMatUpdate(m_BornInfo.sBorn.child, m);

	auto it = m_BornInfo.BornList.begin();
	for (int i = 0; it != m_BornInfo.BornList.end(); it++, i++)
	{
		m_MeshInfo.BornBuffer.bornMat[i] = (*it)->bornMat;
	}
}

void LoadXDynamic::BornDebug(eBornDebug eBornDebug)
{
	D3DXMATRIX m;
	std::string str;
	auto it = m_BornInfo.BornList.begin();
	auto itEnd = m_BornInfo.BornList.end();
	for (; it != itEnd; it++)
	{
		switch (eBornDebug)
		{
		case eInitMat:   m = (*it)->initMat; str = "InitMat"; break;
		case eOffsetMat: m = (*it)->offsetMat; str = "OffsetMat"; break;
		case eWorld:     m = (*it)->worldMat; str = "WorldMat"; break;
		case eBornMat:   m = (*it)->bornMat; str = "BornMat"; break;
		default: break;
		}

		//�s��`��
		std::cout << (*it)->BornName;
		std::cout << " BornIndex" << "[" << (*it)->indexId << "]" << " ";
		std::cout << str << "\n";
		printf("%f, %f, %f, %f\n", m._11, m._12, m._13, m._14);
		printf("%f, %f, %f, %f\n", m._21, m._22, m._23, m._24);
		printf("%f, %f, %f, %f\n", m._31, m._32, m._33, m._34);
		printf("%f, %f, %f, %f\n", m._41, m._42, m._43, m._44);
		std::cout << "\n";
	}
}

void LoadXDynamic::AnimationDebug(int animNum)
{
	/*
	int FrameNum = m_BornInfo.AnimationSetFrameNum[animNum].size();
	std::cout << " FrameNum" << FrameNum << "\n";

	auto it = m_BornInfo.AnimationSetMat[animNum].begin();

	for (; it != m_BornInfo.AnimationSetMat[animNum].end(); it++)
	{
		auto it2 = it->second.begin();

		std::cout << it->first << "\n";
		D3DMATRIX m = *it2;
		printf("%f, %f, %f, %f\n", m._11, m._12, m._13, m._14);
		printf("%f, %f, %f, %f\n", m._21, m._22, m._23, m._24);
		printf("%f, %f, %f, %f\n", m._31, m._32, m._33, m._34);
		printf("%f, %f, %f, %f\n", m._41, m._42, m._43, m._44);
	}
	*/
}

void LoadXDynamic::LoadAnimation(std::string fileName, SkinVertexInfo* pVB)
{
	std::map<std::string, SkinWeightInfo> SkinWeightInfo; //�e�{�[���̃X�L���E�F�C�g�l�ƃ��X�g
	std::map<int, std::map<std::string, AnimationInfo>> AnimationSet;
	int AnimSetNumAll = 0;

	std::ifstream ifs(fileName);
	std::string str;
	if (ifs.fail()) std::cerr << "���s" << std::endl;

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "//") for (int i = 0; i < 4; i++) ifs >> str;

		//�e���v���[�g�ł̊ԈႢ�h�~	
		if (str == "template")
		{
			while (str != "}") ifs >> str;
			continue;
		}

		//�E�F�C�g�l�ƃ��X�g�ǂݍ���
		if (str == "SkinWeights")
		{
			//�{�[�����擾
			ifs >> str;
			ifs >> str;

			//�]���ȕ����폜
			std::string bornName = str;
			for (int nameCnt = 0; nameCnt < (int)bornName.size(); nameCnt++)
			{
				if (bornName[nameCnt] == '"') bornName.erase(bornName.begin() + nameCnt);
				if (bornName[nameCnt] == ';') bornName.erase(bornName.begin() + nameCnt);
			}

			//�d�݃��X�g��
			int num; ifs >> num;
			SkinWeightInfo[bornName].listNumAll = num;

			//getline(ifs, str);
			ifs >> str;

			//�d�݃��X�g�ǂݍ���		
			for (int index = 0, i = 0; i < num; i++)
			{
				ifs >> index;
				ifs >> str;
				SkinWeightInfo[bornName].weightList.emplace_back(index);
			}

			//�d�ݒl�ǂݍ���
			for (float weight = 0.0f, i = 0; i < num; i++)
			{
				ifs >> weight;
				ifs >> str;
				SkinWeightInfo[bornName].weight.emplace_back(weight);
			}

			getline(ifs, str);

			//�{�[���I�t�Z�b�g�s��ǂݍ���
			float x, y, z, w;
			for (int matCnt = 0; matCnt < 4; matCnt++)
			{
				getline(ifs, str);
				sscanf_s(str.data(), "%f, %f, %f, %f,", &x, &y, &z, &w);

				SkinWeightInfo[bornName].offsetMat.m[matCnt][0] = x;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][1] = y;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][2] = z;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][3] = w;
			}
		}

		//�A�j���[�V�����Z�b�g��ǂݍ���
		if (str == "AnimationSet")
		{
			int count = 1;
			int frameNum = 0;
			float x, y, z, w;
			x = y = z = w = 0;
			bool is = true;
			bool isFrameLoad = false;

			while (!ifs.eof())
			{
				ifs >> str;
				if (str == "{") count++;
				if (str == "}") count--;

				if (count == 0) break; //�A�j���[�V�����Z�b�g�I��
				if (is)
				{
					is = false;
					count--;
				}

				//�A�j���[�V�����ǂݍ���
				if (str == "Animation")
				{
					ifs >> str;
					count++;

					ifs >> str;

					//�Z���ʔr��
					std::string bornName = str;
					for (int j = 0; j < (int)bornName.size(); j++)
					{
						if (bornName[j] == '{') bornName.erase(bornName.begin() + j);
						if (bornName[j] == '}') bornName.erase(bornName.begin() + j);
					}

					//���s
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					int t = std::stoi(str); //�R�}��
					getline(ifs, str);
					
					int temp;
					//��]�l�擾
					for (int r = 0; r < t; r++)
					{
						sscanf_s(str.data(), "%d;%d; %f, %f, %f, %f;;,", &frameNum, &temp, &w, &x, &y, &z);
						getline(ifs, str);

						AnimationSet[AnimSetNumAll][bornName].rotation.emplace_back(x, y, z, w);

						//�A�j���[�V�����Z�b�g�̃t���[���ǂݍ��݃Z�b�g�Ɉ����
						if (!isFrameLoad)
						{
							m_BornInfo.AnimationSetFrameNum[AnimSetNumAll].emplace_back(frameNum);
						}
					}
					isFrameLoad = true;
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					t = std::stoi(str); //�R�}��
					getline(ifs, str);

					//�X�P�[���l�擾
					for (int s = 0; s < t; s++)
					{
						sscanf_s(str.data(), "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						getline(ifs, str);

						AnimationSet[AnimSetNumAll][bornName].scale.emplace_back(x, y, z);
					}
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					t = std::stoi(str); //�R�}��
					getline(ifs, str);

					//�ʒu�擾
					for (int p = 0; p < t; p++)
					{
						sscanf_s(str.data(), "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						getline(ifs, str);

						AnimationSet[AnimSetNumAll][bornName].position.emplace_back(x, y, z);
					}
				}
			}

			AnimSetNumAll++; //�A�j���[�V�����Z�b�g��
		}
	}

	//�A�j���[�V�����Z�b�g���}�g���b�N�X�ɕϊ�
	D3DXMATRIX m;
	D3DXMATRIX S;
	auto it = AnimationSet.begin();
	auto itEnd = AnimationSet.end();
	for (int i = 0; it != itEnd; it++, i++)
	{
		int loopNum = m_BornInfo.AnimationSetFrameNum[i].size();
		auto it2 = it->second.begin();
		auto it2End = it->second.end();
		for (; it2 != it2End; it2++)
		{
			for (int j = 0; j < loopNum; j++)
			{
				D3DXMatrixIdentity(&m);

				D3DXVECTOR4 rotation = it2->second.rotation[j];
				D3DXVECTOR3 scale = it2->second.scale[j];
				D3DXVECTOR3 position = it2->second.position[j];

				D3DXQUATERNION quaternionOut(rotation.x, rotation.y, rotation.z, -rotation.w);
				D3DXMatrixRotationQuaternion(&m, &quaternionOut);
				D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

				m = m * S;

				//���s�ړ�
				m._41 = position.x;
				m._42 = position.y;
				m._43 = position.z;

				m_BornInfo.AnimationSetMat[i][it2->first].emplace_back(m);
			}
		}
	}

	//���_�Ƀ{�[���̔ԍ��ƃE�F�C�g�l�����蓖�Ă�
	auto it2 = SkinWeightInfo.begin();
	auto it2End = SkinWeightInfo.end();
	for (; it2 != it2End; it2++)
	{
		//�{�[���Ɋ��蓖�ĂĂ鐔�����s
		for (int i = 0; i < it2->second.listNumAll; i++)
		{
			//��̒��_�Ƀ{�[����4�܂ŋ󂢂Ă���Ƃ��Ɋi�[
			int BornIndex = it2->second.weightList[i];
			for (int j = 0; j < 4; j++)
			{
				if (pVB[BornIndex].BoneIndex[j] == 0)
				{
					//�Y������{�[���̃C���f�b�N�X�ԍ��i�[
					int BornNum = 0;
					auto it3 = m_BornInfo.BornList.begin();
					for (; it3 != m_BornInfo.BornList.end(); it3++)
					{
						if ((*it3)->BornName == it2->first)
						{
							BornNum = (*it3)->indexId;
							break;
						}
					}

					//�{�[���C���f�b�N�X�ƃE�F�C�g�l�i�[
					pVB[BornIndex].BoneIndex[j] = BornNum;
					pVB[BornIndex].BoneWeight[j] = it2->second.weight[i];
					break;
				}
			}
		}
	}

	//�{�[���I�t�Z�b�g�s��i�[
	auto itBornList = m_BornInfo.BornList.begin();
	auto itBornListEnd = m_BornInfo.BornList.end();
	for (; itBornList != itBornListEnd; itBornList++)
	{
		(*itBornList)->offsetMat = SkinWeightInfo[(*itBornList)->BornName].offsetMat;
	}

	//�A�j���[�V�����Z�b�g�폜
	auto itAnim = AnimationSet.begin();
	auto itAnimEnd = AnimationSet.end();
	for (; itAnim != itAnimEnd; itAnim++)
	{
		//��]�ƃX�P�[���ƕ��s�ړ����폜
		auto anim = itAnim->second.begin();
		auto animEnd = itAnim->second.end();
		for (; anim != animEnd; anim++)
		{
			anim->second.rotation.clear();
			anim->second.rotation.shrink_to_fit();
			anim->second.scale.clear();
			anim->second.scale.shrink_to_fit();
			anim->second.position.clear();
			anim->second.position.shrink_to_fit();
		}
		itAnim->second.clear();
	}
	AnimationSet.clear();

	//�X�L���E�F�C�g�폜
	auto itWeight = SkinWeightInfo.begin();
	auto itWeightEnd = SkinWeightInfo.end();
	for (; itWeight != itWeightEnd; itWeight++)
	{
		itWeight->second.weightList.clear();
		itWeight->second.weightList.shrink_to_fit();
		itWeight->second.weight.clear();
		itWeight->second.weight.shrink_to_fit();
	}
	SkinWeightInfo.clear();
}

void LoadXDynamic::LoadMat(Born *pBorn, std::ifstream *pIfs)
{
	std::string str;
	getline(*pIfs, str);
	getline(*pIfs, str);
	getline(*pIfs, str);

	D3DXMATRIX m;

	float x, y, z, w;
	for (int matCnt = 0; matCnt < 4; matCnt++)
	{
		sscanf_s(str.data(), "%f, %f, %f, %f,", &x, &y, &z, &w);
		m.m[matCnt][0] = x; m.m[matCnt][1] = y; m.m[matCnt][2] = z; m.m[matCnt][3] = w;
		getline(*pIfs, str);
	}

	pBorn->initMat = m;
}

void LoadXDynamic::BornMatUpdate(Born *pBorn, D3DXMATRIX &bornMat)
{
	pBorn->ParentAndChildMat = pBorn->worldMat * bornMat;
	pBorn->bornMat = pBorn->offsetMat * pBorn->ParentAndChildMat;

	if (pBorn->child != nullptr) BornMatUpdate(pBorn->child, pBorn->ParentAndChildMat);
	if (pBorn->brother != nullptr) BornMatUpdate(pBorn->brother, bornMat);
}

void LoadXDynamic::AnimUpdate(Born *pBorn)
{
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);

	//�A�j���[�V�����Z�b�g�̃t���[������
	std::vector<int> *pFrameNum = &m_BornInfo.AnimationSetFrameNum[m_AnimSetNum];
	auto itFrame = pFrameNum->begin();

	std::vector<D3DXMATRIX> *pAnimMat = &m_BornInfo.AnimationSetMat[m_AnimSetNum][pBorn->BornName];
	auto itAnimMat = pAnimMat->begin();

	//�w��̃A�j���[�V�����t���[���𒴂�����߂�
	if (m_FrameAnim > pFrameNum->size() - 1)
	{
		m_FrameAnim = 0;
	}
	else if (m_FrameAnim < 0.0f)
	{
		m_FrameAnim = (float)pFrameNum->size() - 1;
	}

	//�A�j���[�V�������
	if (pAnimMat->size() != 0)
	{
		unsigned int frameNum = pFrameNum->size() - 1;
		for (unsigned int i = 0; i < frameNum; i++)
		{
			float frameBefore = (float)itFrame[i];
			float frameAfter = (float)itFrame[i + 1];
			if ((m_FrameAnim >= frameBefore) && (m_FrameAnim <= frameAfter))
			{
				//�t���[������
				float lengeTime = m_FrameAnim - frameBefore;

				float lenge = frameAfter - frameBefore;
				float t = lengeTime / lenge;

				D3DXMATRIX mb = itAnimMat[i];
				D3DXMATRIX ma = itAnimMat[i + 1];
				m = mb * (1 - t) + ma * t;

				break;
			}
		}
	}

	pBorn->worldMat = m;

	if (pBorn->child != nullptr) AnimUpdate(pBorn->child);
	if (pBorn->brother != nullptr) AnimUpdate(pBorn->brother);
}

void LoadXDynamic::DeleteHierarchy(Born *pBorn)
{
	if (pBorn->child != nullptr) DeleteHierarchy(pBorn->child);
	if (pBorn->brother != nullptr) DeleteHierarchy(pBorn->brother);
	delete pBorn;
}

bool LoadXDynamic::AddBoneHierarchy(Born *pBorn, std::ifstream *pIfs, int hierarchy)
{
	int begin = 0, end = 0;
	int current = hierarchy; //���݂̊K�w
	std::string str;
	std::ifstream::pos_type beg;

	while (!pIfs->eof())
	{
		*pIfs >> str;
		if (str == "Mesh") break;

		//�e���v���[�g�ł̊ԈႢ�h�~	
		if (str == "template")
		{
			while (str != "}") *pIfs >> str;
			continue;
		}

		if (str == "{") begin++;
		if (str == "}") end++;

		if (str == "Frame")
		{
			*pIfs >> str;

			//�q��ǉ�
			if ((begin == 0) && (end == 0) || (end - begin == -1))
			{
				pBorn->child = new Born;
				pBorn->child->BornName = str;
				pBorn->child->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->child);

				//�s��ǂݍ���
				beg = pIfs->tellg();
				LoadMat(pBorn->child, pIfs);
				pIfs->seekg(beg);

				hierarchy++;
				AddBoneHierarchy(pBorn->child, pIfs, hierarchy);
			}

			//�K�w�߂��ɂ����ꍇ�A����K�w�ǉ�
			if (m_Back == current)
			{
				m_Back = -1;
				pBorn->brother = new Born;
				pBorn->brother->BornName = m_buffer;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//�s��ǂݍ���
				beg = pIfs->tellg();
				LoadMat(pBorn->brother, pIfs);
				pIfs->seekg(beg);

				AddBoneHierarchy(pBorn->brother, pIfs, current);
			}

			//"}"��"{"��葽�����͊K�w��߂�
			if (end - begin > 0){
				m_buffer = str;
				m_Back = current - (end - begin);
				return true;
			}

			//����K�w�ǉ�
			if ((end - begin == 0) && ((begin != 0) && (end != 0)))
			{
				pBorn->brother = new Born;
				pBorn->brother->BornName = str;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//�s��ǂݍ���
				beg = pIfs->tellg();
				LoadMat(pBorn->brother, pIfs);
				pIfs->seekg(beg);

				AddBoneHierarchy(pBorn->brother, pIfs, current);
			}

			//�K�w��߂�
			if ((m_Back != -1) && (m_Back < current)) return true;
		}
	}
	return true;
}