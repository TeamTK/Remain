#include "LoadXStatic.h"
#include "..\..\System\Window.h"

LoadXStatic::LoadXStatic(std::string fileName)
{
	//���b�V���̓ǂݍ���
	if (FAILED(LoadXMesh(fileName)))
	{
		MessageBoxA(0, "���b�V���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}

	//�V�F�[�_�[�̏�����
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "�V�F�[�_�[�̏������Ɏ��s���܂���", NULL, MB_OK);
	}
}

LoadXStatic::~LoadXStatic()
{
}

HRESULT LoadXStatic::LoadXMesh(std::string fileName)
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

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
	std::vector<Vector3D> ambient;
	std::vector<Vector4D> diffuse;
	std::vector<Vector3D> specular;
	std::vector<std::string> textureNameArray;
	std::vector<int> materialList;

	int index1, index2, index3;
	int frameNum = 0;
	int uvNumAll = 0; //UV�̐�
	int normalNumAll = 0; //�@���̐�
	int materialListNumAll = 0;
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

		//���[�J���s�񏊓�
		if (strcmp(key, "Frame") == 0)
		{
			frameNum++;
			assert(frameNum <= 2  && "�A�j���[�V������񂪂���܂�StaticMesh�ł͓ǂݍ��߂܂���");
			Matrix m;
			fgets(str, sizeof(str), fp);
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._11, &m._12, &m._13, &m._14);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._21, &m._22, &m._23, &m._24);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._31, &m._32, &m._33, &m._34);
			fscanf_s(fp, "%f, %f, %f, %f;;", &m._41, &m._42, &m._43, &m._44);

			if (frameNum == 2) m_MeshInfo.localMatrix *= m;
			else m_MeshInfo.localMatrix = m;
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
			fscanf_s(fp, "%d;", &normalNumAll);

			//�@���ǂݍ���
			for (int i = 0; i < normalNumAll; i++)
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
			fscanf_s(fp, "%d;", &uvNumAll);

			for (int i = 0; i < uvNumAll; i++)
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
			fscanf_s(fp, "%d;", &materialListNumAll);

			//�}�e���A�����X�g�i�[
			int listNum = 0;
			for (int i = 0; i < materialListNumAll; i++)
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

				//�e�N�X�`���[�ǂݍ��݃f�B���N�g���ɂ���
				for (int i = 0, j = 0; i < (int)temp.size(); i++)
				{
					if (temp[i] == '\\')
					{
						for (; j < i; j++) temp2 += temp[j];
						temp2 += '\\';
						j++;
					}
				}

				ambient.emplace_back(ka);
				diffuse.emplace_back(kd);
				specular.emplace_back(ks);
				textureNameArray.emplace_back(temp2 + textureName);
			}
			else
			{
				ambient.emplace_back(ka);
				diffuse.emplace_back(kd);
				specular.emplace_back(ks);
				textureNameArray.emplace_back("NoTexture");
			}
		}
	}

	assert(coordinate.size() != 0 && "X�t�@�C���̒��_���W������܂���");
	assert(normal.size() != 0 && "X�t�@�C���̖@��������܂���");
	assert(ambient.size() != 0 && "X�t�@�C���̃}�e���A��������܂���");

	//�}�e���A���ƃC���f�b�N�X�o�b�t�@�[���I�쐬
	m_MeshInfo.pMaterial = new MaterialInfo[m_MeshInfo.materialNumAll];		//�}�e���A��
	m_MeshInfo.ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];	//�C���f�b�N�X
	m_pVertex = new VertexInfo[m_MeshInfo.vertexNumAll];
	m_MeshInfo.pIndex = new IndexInfo[m_MeshInfo.faceNumAll];

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	int indexCnt = 0;

	//�}�e���A���̐����C���f�b�N�X�o�b�t�@�[�쐬
	int count = 0;
	int *pFaceBuffer = nullptr;
	for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
	{
		pFaceBuffer = new int[m_MeshInfo.faceNumAll * 3];
		count = 0;

		//�}�e���A�����蓖��
		m_MeshInfo.pMaterial[i].ambient = ambient[i];
		m_MeshInfo.pMaterial[i].diffuse = diffuse[i];
		m_MeshInfo.pMaterial[i].specular = specular[i];

		m_MeshInfo.pMaterial[i].textureName = textureNameArray[i];

		//�e�N�X�`���[�ǂݍ���
		if (textureNameArray[i] == "NoTexture")
		{
			m_MeshInfo.isTexture = false;
		}
		else
		{
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(pDevice, textureNameArray[i].c_str(), NULL, NULL, &m_MeshInfo.pMaterial[i].pTexture, NULL)))
			{
				Relese();
				MessageBoxA(NULL, TEXT("�e�N�X�`���[�̓ǂݍ��݂Ɏ��s���܂���"), NULL, MB_OK);
				return E_FAIL;
			}
		}

		//�Ή�����}�e���A���ɖʏ��i�[
		for (int j = 0; j < m_MeshInfo.faceNumAll; j++)
		{
			if (i == materialList[j])
			{
				//���J�p�C���f�b�N�X
				m_MeshInfo.pIndex[indexCnt].vertexIndex[0] = face[j * 3];
				m_MeshInfo.pIndex[indexCnt].vertexIndex[1] = face[j * 3 + 1];
				m_MeshInfo.pIndex[indexCnt].vertexIndex[2] = face[j * 3 + 2];
				m_MeshInfo.pIndex[indexCnt].materialIndex = i;
				indexCnt++;

				//�����p�C���f�b�N�X
				pFaceBuffer[count] = face[j * 3];
				pFaceBuffer[count + 1] = face[j * 3 + 1];
				pFaceBuffer[count + 2] = face[j * 3 + 2];
				m_MeshInfo.pMaterial[i].numPolygon += 1;
				count += 3;
			}
		}
		if (count == 0) continue;
		count = 0;

		//�C���f�b�N�X�o�b�t�@�[���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * m_MeshInfo.pMaterial[i].numPolygon * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pFaceBuffer;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.ppIndexBuffer[i])))
			return FALSE;

		delete [] pFaceBuffer;
		pFaceBuffer = nullptr;
	}

	SAFE_DELETE_ARRAY(pFaceBuffer);

	//���_�Ɋ��蓖��
	if (uv.size() >= 1)
	{
		//uv�L��o�[�W����
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_pVertex[i].pos = coordinate[i];
			m_pVertex[face[i]].normal = normal[normalIndex[i]];
			m_pVertex[i].uv = uv[i];
		}
	}
	else
	{
		//uv�Ȃ��o�[�W����
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_pVertex[i].pos = coordinate[i];
			m_pVertex[i].normal = normal[normalIndex[i]];
		}
	}

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_pVertex;
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
	 ambient.clear();
	 ambient.shrink_to_fit();
	 diffuse.clear();
	 diffuse.shrink_to_fit();
	 specular.clear();
	 specular.shrink_to_fit();
	 textureNameArray.clear();
	 textureNameArray.clear();
	 materialList.clear();
	 materialList.shrink_to_fit();

	 fclose(fp);

	return S_OK;
}

void LoadXStatic::Relese()
{
	if (m_MeshInfo.pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.pMaterial[i].pTexture);
			m_MeshInfo.ppIndexBuffer[i]->Release();
		}
		
		SAFE_DELETE_ARRAY(m_MeshInfo.pIndex);
		SAFE_DELETE_ARRAY(m_pVertex);
		SAFE_DELETE_ARRAY(m_MeshInfo.pMaterial);
		SAFE_DELETE_ARRAY(m_MeshInfo.ppIndexBuffer);
		SAFE_RELEASE(m_MeshInfo.pSampleLinear);
	}
}