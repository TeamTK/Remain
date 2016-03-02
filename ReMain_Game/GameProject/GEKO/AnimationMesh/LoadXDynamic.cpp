#include "LoadXDynamic.h"
#include "..\Mesh\LoadXStatic.h"
#include "..\System\Window.h"

struct SkinWeightInfo
{
	int listNumAll;
	std::vector<int> weightList;
	std::vector<float> weight;
	Matrix offsetMat;
};

struct AnimationInfo
{
	std::vector<Vector4D> rotation;
	std::vector<Vector3D> scale;
	std::vector<Vector3D> position;
};

LoadXDynamic::LoadXDynamic(std::string fileName) :
	m_Back(-1),
	m_BornIndex(0)
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
	for (auto& i : m_BornInfo.AnimationSetFrameNum)
	{
		i.second.clear();
		i.second.shrink_to_fit();
	}
	m_BornInfo.AnimationSetFrameNum.clear();

	//�t���[���K�w�\���폜
	DeleteHierarchy(m_BornInfo.sBorn.child);
}

HRESULT LoadXDynamic::LoadXMesh(std::string fileName)
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

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
	std::vector<Vector3D> coordinate;
	std::vector<Vector3D> normal;
	std::vector<Vector2D> uv;
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

			Vector4D kd(0.0f, 0.0f, 0.0f, 0.0f);
			float sp = 0.0f;
			Vector3D ks(0.0f, 0.0f, 0.0f);
			Vector3D ka(0.0f, 0.0f, 0.0f);
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

	assert(coordinate.size() != 0 && "X�t�@�C���̒��_���W������܂���");
	assert(normal.size() != 0 && "X�t�@�C���̖@��������܂���");
	assert(material.size() != 0 && "X�t�@�C���̃}�e���A��������܂���");

	//�}�e���A���ƃC���f�b�N�X�o�b�t�@�[���I�쐬
	m_MeshInfo.pMaterial = new SkinMaterialInfo[m_MeshInfo.materialNumAll];
	m_MeshInfo.ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];
	m_MeshInfo.pVertex = new SkinVertexInfo[m_MeshInfo.vertexNumAll];

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
		m_MeshInfo.pMaterial[i].ambient = material[i].ambient;
		m_MeshInfo.pMaterial[i].diffuse = material[i].diffuse;
		m_MeshInfo.pMaterial[i].specular = material[i].specular;

		m_MeshInfo.pMaterial[i].textureName = material[i].textureName;

		if (material[i].textureName == "NoTexture")
		{
			m_MeshInfo.isTexture = false;
		}
		else
		{
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(pDevice, material[i].textureName.c_str(), NULL, NULL, &m_MeshInfo.pMaterial[i].pTexture, NULL)))
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
				m_MeshInfo.pMaterial[i].numPolygon += 1;
				count += 3;
			}
		}
		if (count == 0) continue;

		//�C���f�b�N�X�o�b�t�@�[���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * m_MeshInfo.pMaterial[i].numPolygon * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = faceBuffer;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.ppIndexBuffer[i])))
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
			m_MeshInfo.pVertex[i].pos = coordinate[i];
			m_MeshInfo.pVertex[face[i]].normal = normal[normalIndex[i]];
			m_MeshInfo.pVertex[i].uv = uv[i];
		}
	}
	else
	{
		//uv�Ȃ��o�[�W����
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pVertex[i].pos = coordinate[i];
			m_MeshInfo.pVertex[face[i]].normal = normal[normalIndex[i]];
		}
	}

	fseek(fp, 0, SEEK_SET);
	AddBoneHierarchy(&m_BornInfo.sBorn, fp, 0); //�{�[���K�w�\���ǂݍ���

	fseek(fp, 0, SEEK_SET);
	LoadAnimation(fp, m_MeshInfo.pVertex); //�A�j���[�V�����ƃX�L���E�F�C�g�ǂݍ���

	//���[�J���s��i�[
	//m_MeshInfo.m_LocalMat = m_BornInfo.BornList[0]->initMat *m_BornInfo.BornList[m_BornInfo.BornList.size() - 1]->initMat;

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_MeshInfo.pVertex;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.pVertexBuffer)))
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

	fclose(fp);

	return S_OK;
}

void LoadXDynamic::Relese()
{
	if (m_MeshInfo.pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.pMaterial[i].pTexture);
			m_MeshInfo.ppIndexBuffer[i]->Release();
		}

		SAFE_DELETE_ARRAY(m_MeshInfo.pVertex);
		SAFE_DELETE_ARRAY(m_MeshInfo.pMaterial);
		SAFE_DELETE_ARRAY(m_MeshInfo.ppIndexBuffer);
		SAFE_RELEASE(m_MeshInfo.pVertexBuffer);
		SAFE_RELEASE(m_MeshInfo.pSampleLinear);
	}
}

void LoadXDynamic::CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal)
{
	//�q�{�[��
	if (pBornOriginal->child != nullptr)
	{
		pBornCopy->child = new CopyBorn;
		pCopyBornArray->emplace_back(pBornCopy->child);
		CopyBornTree(pBornCopy->child, pCopyBornArray, pBornOriginal->child);
	}

	//�e�{�[��
	if (pBornOriginal->brother != nullptr)
	{
		pBornCopy->brother = new CopyBorn;
		pCopyBornArray->emplace_back(pBornCopy->brother);
		CopyBornTree(pBornCopy->brother, pCopyBornArray, pBornOriginal->brother);
	}
}

void LoadXDynamic::Update(CopyBorn *pCopyBorn)
{
	AnimUpdate(pCopyBorn, m_BornInfo.sBorn.child);

	//�{�[���X�V
	Matrix m;
	BornMatUpdate(pCopyBorn, m_BornInfo.sBorn.child, m);
}

void LoadXDynamic::LoadAnimation(FILE* fp, SkinVertexInfo* pVB)
{
	std::map<std::string, SkinWeightInfo> SkinWeightInfo; //�e�{�[���̃X�L���E�F�C�g�l�ƃ��X�g
	std::map<int, std::map<std::string, AnimationInfo>> AnimationSet;
	int AnimSetNumAll = 0;

	char key[256];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s", key, sizeof(key));

		if (strcmp(key, "//") == 0) for (int i = 0; i < 4; i++) fscanf_s(fp, "%s", key, sizeof(key));

		//�e���v���[�g�ł̊ԈႢ�h�~	
		if (strcmp(key, "template") == 0)
		{
			while (strcmp(key, "}") != 0) fscanf_s(fp, "%s", key, sizeof(key));
			continue;
		}

		//�E�F�C�g�l�ƃ��X�g�ǂݍ���
		if (strcmp(key, "SkinWeights") == 0)
		{
			//�{�[�����擾
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));

			//�]���ȕ����폜
			std::string bornName = key;
			for (int nameCnt = 0; nameCnt < (int)bornName.size(); nameCnt++)
			{
				if (bornName[nameCnt] == '"') bornName.erase(bornName.begin() + nameCnt);
				if (bornName[nameCnt] == ';') bornName.erase(bornName.begin() + nameCnt);
			}

			//�d�݃��X�g��
			int num;
			fscanf_s(fp, "%d;", &num);
			SkinWeightInfo[bornName].listNumAll = num;

			//�d�݃��X�g�ǂݍ���		
			for (int index = 0, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%d,", &index);
				//fscanf_s(fp, "%s", key, sizeof(key));
				SkinWeightInfo[bornName].weightList.emplace_back(index);
			}
			fscanf_s(fp, "%s", key, sizeof(key));

			//�d�ݒl�ǂݍ���
			for (float weight = 0.0f, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%f,", &weight);
				SkinWeightInfo[bornName].weight.emplace_back(weight);
			}

			fgets(key, sizeof(key), fp);

			//�{�[���I�t�Z�b�g�s��ǂݍ���
			float x, y, z, w;
			for (int matCnt = 0; matCnt < 4; matCnt++)
			{
				fgets(key, sizeof(key), fp);
				sscanf_s(key, "%f, %f, %f, %f,", &x, &y, &z, &w);

				SkinWeightInfo[bornName].offsetMat.m[matCnt][0] = x;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][1] = y;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][2] = z;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][3] = w;
			}
		}

		//�A�j���[�V�����Z�b�g��ǂݍ���
		if (strcmp(key, "AnimationSet") == 0)
		{
			int count = 1;
			int frameNum = 0;

			float x, y, z, w;
			x = y = z = w = 0;

			bool is = true;
			bool isFrameLoad = false;

			while (!feof(fp))
			{
				fscanf_s(fp, "%s", key, sizeof(key));
				if (strcmp(key, "{") == 0) count++;
				if (strcmp(key, "}") == 0) count--;

				if (count == 0) break; //�A�j���[�V�����Z�b�g�I��
				if (is)
				{
					is = false;
					count--;
				}

				//�A�j���[�V�����ǂݍ���
				if (strcmp(key, "Animation") == 0)
				{
					fscanf_s(fp, "%s", key, sizeof(key));
					count++;
					fscanf_s(fp, "%s", key, sizeof(key));

					//�Z���ʔr��
					std::string bornName = key;
					for (int j = 0; j < (int)bornName.size(); j++)
					{
						if (bornName[j] == '{') bornName.erase(bornName.begin() + j);
						if (bornName[j] == '}') bornName.erase(bornName.begin() + j);
					}

					//���s
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					int t = std::stoi(key); //�R�}��
					fgets(key, sizeof(key), fp);
					
					int temp;
					//��]�l�擾
					for (int r = 0; r < t; r++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f, %f;;,", &frameNum, &temp, &w, &x, &y, &z);
						fgets(key, sizeof(key), fp);

						AnimationSet[AnimSetNumAll][bornName].rotation.emplace_back(x, y, z, w);

						//�A�j���[�V�����Z�b�g�̃t���[���ǂݍ��݃Z�b�g�Ɉ����
						if (!isFrameLoad)
						{
							m_BornInfo.AnimationSetFrameNum[AnimSetNumAll].emplace_back(frameNum);
						}
					}
					isFrameLoad = true;
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					t = std::stoi(key); //�R�}��
					fgets(key, sizeof(key), fp);

					//�X�P�[���l�擾
					for (int s = 0; s < t; s++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						fgets(key, sizeof(key), fp);

						AnimationSet[AnimSetNumAll][bornName].scale.emplace_back(x, y, z);
					}
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					t = std::stoi(key); //�R�}��
					fgets(key, sizeof(key), fp);

					//�ʒu�擾
					for (int p = 0; p < t; p++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						fgets(key, sizeof(key), fp);

						AnimationSet[AnimSetNumAll][bornName].position.emplace_back(x, y, z);
					}
				}
			}

			AnimSetNumAll++; //�A�j���[�V�����Z�b�g��
		}
	}

	//�A�j���[�V�����Z�b�g���}�g���b�N�X�ɕϊ�
	Matrix m;
	Matrix S;

	Vector4D rotation;
	Vector3D scale;
	Vector3D position;
	D3DXQUATERNION quaternionOut;

	int cnt = 0;
	for (auto& i : AnimationSet)
	{
		int loopNum = m_BornInfo.AnimationSetFrameNum[cnt].size();
		for (auto& j : i.second)
		{
			for (int k = 0; k < loopNum; k++)
			{
				rotation = j.second.rotation[k];
				scale = j.second.scale[k];
				position = j.second.position[k];

				quaternionOut.x = rotation.x;
				quaternionOut.y = rotation.y;
				quaternionOut.z = rotation.z;
				quaternionOut.w = -rotation.w;

				D3DXMatrixRotationQuaternion(&m, &quaternionOut);
				D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
				
				m = m * S;

				//���s�ړ�
				m._41 = position.x;
				m._42 = position.y;
				m._43 = position.z;

				m_BornInfo.AnimationSetMat[cnt][j.first].emplace_back(m);
			}
		}
		cnt++;
	}

	//���_�Ƀ{�[���̔ԍ��ƃE�F�C�g�l�����蓖�Ă�
	for (auto& i : SkinWeightInfo)
	{
		int cntBorn = 0;
		for (int j = 0; j < i.second.listNumAll; j++)
		{
			//��̒��_�Ƀ{�[����4�܂ŋ󂢂Ă���Ƃ��Ɋi�[
			int index = i.second.weightList[cntBorn];
			for (int k = 0; k < 4; k++)
			{
				if (pVB[index].boneIndex[k] == 0)
				{
					//�Y������{�[���̃C���f�b�N�X�ԍ��i�[
					int bornNum = 0;
					for (auto& m : m_BornInfo.BornList)
					{
						if (m->BornName == i.first)
						{
							bornNum = m->indexId;
							break;
						}
					}
					//�{�[���C���f�b�N�X�ƃE�F�C�g�l�i�[
					pVB[index].boneIndex[k] = bornNum;
					pVB[index].boneWeight[k] = i.second.weight[j];
					break;
				}
			}
			cntBorn++;
		}
	}

	//�I�t�Z�b�g�s��i�[
	for (auto& i : m_BornInfo.BornList)
	{
		i->offsetMat = SkinWeightInfo[i->BornName].offsetMat;
	}

	//�A�j���[�V�����Z�b�g�폜
	for (auto& i : AnimationSet)
	{
		//��]�ƃX�P�[���ƕ��s�ړ����폜
		for (auto& j : i.second)
		{
			j.second.rotation.clear();
			j.second.rotation.shrink_to_fit();
			j.second.scale.clear();
			j.second.scale.shrink_to_fit();
			j.second.position.clear();
			j.second.position.shrink_to_fit();
		}
		i.second.clear();
	}
	AnimationSet.clear();

	//�X�L���E�F�C�g�폜
	for (auto& i : SkinWeightInfo)
	{
		i.second.weightList.clear();
		i.second.weightList.shrink_to_fit();
		i.second.weight.clear();
		i.second.weight.shrink_to_fit();
	}
	SkinWeightInfo.clear();
}

void LoadXDynamic::LoadMat(Born *pBorn, FILE *fp)
{
	char key[256];
	fgets(key, sizeof(key), fp);
	fgets(key, sizeof(key), fp);
	fgets(key, sizeof(key), fp);

	Matrix m;

	float x, y, z, w;
	for (int matCnt = 0; matCnt < 4; matCnt++)
	{
		sscanf_s(key, "%f, %f, %f, %f,", &x, &y, &z, &w);
		m.m[matCnt][0] = x; m.m[matCnt][1] = y; m.m[matCnt][2] = z; m.m[matCnt][3] = w;
		fgets(key, sizeof(key), fp);
	}

	pBorn->initMat = m;
}

void LoadXDynamic::BornMatUpdate(CopyBorn *pCopyBorn, Born *pBorn, Matrix &bornMat)
{
	pCopyBorn->ParentAndChildMat = pCopyBorn->worldMat * bornMat;
	pCopyBorn->bornMat = pBorn->offsetMat * pCopyBorn->ParentAndChildMat;

	if (pBorn->child != nullptr) BornMatUpdate(pCopyBorn->child, pBorn->child, pCopyBorn->ParentAndChildMat);
	if (pBorn->brother != nullptr) BornMatUpdate(pCopyBorn->brother, pBorn->brother, bornMat);
}

void LoadXDynamic::AnimUpdate(CopyBorn *pCopyBorn, Born *pBorn)
{
	Matrix m;

	auto frameAnimNum = m_BornInfo.AnimationSetFrameNum[pCopyBorn->animNum].size() - 1;

	//�A�j���[�V�����Z�b�g�̃t���[������
	auto itFrame = m_BornInfo.AnimationSetFrameNum[pCopyBorn->animNum].begin();

	std::vector<Matrix> *pAnimMat = &m_BornInfo.AnimationSetMat[pCopyBorn->animNum][pBorn->BornName];
	auto itAnimMat = pAnimMat->begin();

	//�w��̃A�j���[�V�����t���[���𒴂�����߂�
	if (pCopyBorn->animFrame > (float)frameAnimNum)
	{
		pCopyBorn->animFrame = 0.0f;
	}
	else if (pCopyBorn->animFrame < 0.0f)
	{
		pCopyBorn->animFrame = (float)frameAnimNum;
	}

	//�A�j���[�V�������
	if (pAnimMat->size() != 0)
	{
		unsigned int frameNum = frameAnimNum;
		for (unsigned int i = 0; i < frameNum; i++)
		{
			float frameBefore = (float)itFrame[i];
			float frameAfter = (float)itFrame[i + 1];
			if ((pCopyBorn->animFrame >= frameBefore) && (pCopyBorn->animFrame <= frameAfter))
			{
				//�t���[������
				float lengeTime = pCopyBorn->animFrame - frameBefore;

				float lenge = frameAfter - frameBefore;
				float t = lengeTime / lenge;

				Matrix mb = itAnimMat[i];
				Matrix ma = itAnimMat[i + 1];

				m = mb * (1 - t) + ma * t;

				break;
			}
		}
	}

	pCopyBorn->worldMat = m;

	if (pCopyBorn->child != nullptr) AnimUpdate(pCopyBorn->child, pBorn->child);
	if (pCopyBorn->brother != nullptr) AnimUpdate(pCopyBorn->brother, pBorn->brother);
}

void LoadXDynamic::DeleteHierarchy(Born *pBorn)
{
	if (pBorn->child != nullptr) DeleteHierarchy(pBorn->child);
	if (pBorn->brother != nullptr) DeleteHierarchy(pBorn->brother);
	delete pBorn;
}

bool LoadXDynamic::AddBoneHierarchy(Born *pBorn, FILE *fp, int hierarchy)
{
	int begin = 0, end = 0;
	int current = hierarchy; //���݂̊K�w
	int filePos = 0;
	char key[256];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s", key, sizeof(key));
		if (strcmp(key, "Mesh") == 0) break;

		//�e���v���[�g�ł̊ԈႢ�h�~	
		if (strcmp(key, "template") == 0)
		{
			while (strcmp(key, "}") != 0) fscanf_s(fp, "%s", key, sizeof(key));
			continue;
		}

		if (strcmp(key, "{") == 0) begin++;
		if (strcmp(key, "}") == 0) end++;

		if (strcmp(key, "Frame") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));

			//�q��ǉ�
			if ((begin == 0) && (end == 0) || (end - begin == -1))
			{
				pBorn->child = new Born;
				pBorn->child->BornName = key;
				pBorn->child->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->child);

				//�s��ǂݍ���
				filePos = ftell(fp);
				LoadMat(pBorn->child, fp);
				int e = fseek(fp, filePos, SEEK_SET);

				hierarchy++;
				AddBoneHierarchy(pBorn->child, fp, hierarchy);
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
				filePos = ftell(fp);
				LoadMat(pBorn->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBorn->brother, fp, current);
			}

			//"}"��"{"��葽�����͊K�w��߂�
			if (end - begin > 0){
				m_buffer = key;
				m_Back = current - (end - begin);
				return true;
			}

			//����K�w�ǉ�
			if ((end - begin == 0) && ((begin != 0) && (end != 0)))
			{
				pBorn->brother = new Born;
				pBorn->brother->BornName = key;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//�s��ǂݍ���
				filePos = ftell(fp);
				LoadMat(pBorn->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBorn->brother, fp, current);
			}

			//�K�w��߂�
			if ((m_Back != -1) && (m_Back < current)) return true;
		}
	}
	return true;
}