#include "LoadXDynamic.h"
#include "..\Mesh\LoadXStatic.h"
#include "..\System\Window.h"
#include <sstream>

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
	//メッシュの読み込み
	if (FAILED(LoadXMesh(fileName)))
	{
		MessageBoxA(0, "スキンメッシュの読み込みに失敗しました", NULL, MB_OK);
	}

	//シェーダーの初期化
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "シェーダーの初期化に失敗しました", NULL, MB_OK);
	}
}

LoadXDynamic::~LoadXDynamic()
{
	//ボーンリスト削除
	m_BornInfo.BornList.clear();
	m_BornInfo.BornList.shrink_to_fit();

	//アニメーションフレーム数
	auto it3 = m_BornInfo.AnimationSetFrameNum.begin();
	for (; it3 != m_BornInfo.AnimationSetFrameNum.end(); it3++)
	{
		it3->second.clear();
		it3->second.shrink_to_fit();
	}
	m_BornInfo.AnimationSetFrameNum.clear();

	//フレーム階層構造削除
	DeleteHierarchy(m_BornInfo.sBorn.child);
}

HRESULT LoadXDynamic::LoadXMesh(std::string fileName)
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::Get().GetID3D11Device();

	//ファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, fileName.c_str(), "rt");
	if (fp == NULL)
	{
		fileName += "読み込み失敗";
		MessageBoxA(0, fileName.c_str(), "", MB_OK);
		Window::Get()->WindowEnd();
		return FALSE;
	}

	std::vector<int> face;
	std::vector<D3DXVECTOR3> coordinate;
	std::vector<D3DXVECTOR3> normal;
	std::vector<D3DXVECTOR2> uv;
	std::vector<MaterialX> material;
	std::vector<int> materialList;

	int index1, index2, index3;
	float x, y, z;

	char str[400] = { 0 };
	char key[400] = { 0 };

	//頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s", key, sizeof(key));

		//テンプレートでの間違い防止
		if (strcmp(key, "template") == 0)
		{
			fscanf_s(fp, "%s", key, sizeof(key));
			continue;
		}

		//頂点メッシュ読み込み
		if (strcmp(key, "Mesh") == 0)
		{
			//初め波括弧対策
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "Meshの初め波括弧が正しくありません");

			fgets(str, sizeof(str), fp); //読み込みデータ改行

			//頂点数
			fscanf_s(fp, "%d;", &m_MeshInfo.vertexNumAll);

			//頂点座標読み込み
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				fscanf_s(fp, "%f; %f; %f;,", &x, &y, &z);
				coordinate.emplace_back(x, y, z);
			}

			//面の数
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.faceNumAll);

			//面のインデックス番号読み込み
			for (int i = 0; i < m_MeshInfo.faceNumAll; i++)
			{
				int faceNum;
				fscanf_s(fp, "%d; %d, %d, %d;,", &faceNum, &index1, &index2, &index3);

				assert(faceNum == 3 && "Xファイルの3Dモデルは三角形ポリゴンではありません");

				face.emplace_back(index1); face.emplace_back(index2); face.emplace_back(index3);
			}
		}

		//法線読み込み
		if (strcmp(key, "MeshNormals") == 0)
		{
			//初め波括弧対策
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "法線の初め波括弧が正しくありません");

			//法線の数
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.normalNumAll);

			//法線読み込み
			for (int i = 0; i < m_MeshInfo.normalNumAll; i++)
			{
				fscanf_s(fp, "%f; %f; %f;,", &x, &y, &z);
				normal.emplace_back(x, y, z);
			}
		}

		//UV座標読み込み
		if (strcmp(key, "MeshTextureCoords") == 0)
		{
			//初め波括弧対策
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "UV座標の初め波括弧が正しくありません");

			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.uvNumAll);

			for (int i = 0; i < m_MeshInfo.uvNumAll; i++)
			{
				fscanf_s(fp, "%f; %f;,", &x, &y);
				uv.emplace_back(x, y);
			}
		}

		//マテリアルリスト読み込み
		if (strcmp(key, "MeshMaterialList") == 0)
		{
			//初め波括弧対策
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "マテリアルリスト初め波括弧が正しくありません");

			//マテリアルの数
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.materialNumAll);

			//マテリアルリストの数
			fscanf_s(fp, "%d;", &m_MeshInfo.materialListNumAll);

			//マテリアルリスト格納
			int listNum = 0;
			for (int i = 0; i < m_MeshInfo.materialListNumAll; i++)
			{
				fscanf_s(fp, "%d,", &listNum);
				materialList.emplace_back(listNum);
			}
		}

		//マテリアル読み込み
		if (strcmp(key, "Material") == 0)
		{
			char materialName[400] = { 0 };
			fscanf_s(fp, " %s", &materialName, sizeof(materialName));

			//初め波括弧対策
			char leftCurlyBracket;
			fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

			assert(leftCurlyBracket == '{' && "マテリアルの初め波括弧が正しくありません");

			fgets(str, sizeof(str), fp);

			D3DXVECTOR4 kd(0.0f, 0.0f, 0.0f, 0.0f);
			float sp = 0.0f;
			D3DXVECTOR3 ks(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 ka(0.0f, 0.0f, 0.0f);
			std::string textureName;

			//マテリアル成分読み込み
			fscanf_s(fp, "%f; %f; %f; %f;;", &kd.x, &kd.y, &kd.z, &kd.w);
			fscanf_s(fp, "%f;", &sp);
			fscanf_s(fp, "%f; %f; %f;;", &ks.x, &ks.y, &ks.z);
			fscanf_s(fp, "%f; %f; %f;;", &ka.x, &ka.y, &ka.z);

			//テクスチャー読み込み
			fscanf_s(fp, "%s", &str, sizeof(str));
			if (strcmp(str, "TextureFilename") == 0)
			{
				//初め波括弧対策
				char leftCurlyBracket;
				fscanf_s(fp, " %c", &leftCurlyBracket, sizeof(leftCurlyBracket));

				assert(leftCurlyBracket == '{' && "テクスチャの初め波括弧が正しくありません");

				fscanf_s(fp, "%s", &str, sizeof(str));
				textureName = str;

				//余分な文字削除
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

	//マテリアルとインデックスバッファー動的作成
	m_MeshInfo.m_pMaterial = new SkinMaterialInfo[m_MeshInfo.materialNumAll];
	m_MeshInfo.m_ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];
	m_MeshInfo.pvVertex = new SkinVertexInfo[m_MeshInfo.vertexNumAll];

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//マテリアルの数分インデックスバッファー作成
	int count = 0;
	int *faceBuffer = nullptr;
	for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
	{
		faceBuffer = new int[m_MeshInfo.faceNumAll * 3];
		count = 0;

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
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(pDevice, m_MeshInfo.m_pMaterial[i].TextureName.c_str(), NULL, NULL, &m_MeshInfo.m_pMaterial[i].pTexture, NULL)))
			{
				MessageBoxA(NULL, "テクスチャーの読み込みに失敗しました", NULL, MB_OK);
				return E_FAIL;
			}
		}

		//対応するマテリアルに面情報格納
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

		//インデックスバッファーを作成
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
	/*
	//法線の数が違う対策（違うなら面一つで一つの法線を割り当てる）
	if (m_MeshInfo.normalNumAll == m_MeshInfo.vertexNumAll)
	{
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[i].vNormal = normal[i];
			m_MeshInfo.pvVertex[i].vTex = uv[i];
		}
	}
	else
	{
		//座標とUV
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[i].vTex = uv[i];
		}

		//法線
		for (int i = 0; i < m_MeshInfo.normalNumAll; i++)
		{
			m_MeshInfo.pvVertex[i * 3].vNormal = normal[i];
			m_MeshInfo.pvVertex[i * 3 + 1].vNormal = normal[i];
			m_MeshInfo.pvVertex[i * 3 + 2].vNormal = normal[i];
		}
	}
	*/

	//法線の数が違う対策（違うなら面一つで一つの法線を割り当てる）
	if (m_MeshInfo.normalNumAll == m_MeshInfo.vertexNumAll)
	{
		if (uv.size() >= 1)
		{
			//uv有りバージョン
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				m_MeshInfo.pvVertex[i].vPos = coordinate[i];
				m_MeshInfo.pvVertex[i].vNormal = normal[i];
				m_MeshInfo.pvVertex[i].vTex = uv[i];
			}
		}
		else
		{
			//uvなしバージョン
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				m_MeshInfo.pvVertex[i].vPos = coordinate[i];
				m_MeshInfo.pvVertex[i].vNormal = normal[i];
				m_MeshInfo.pvVertex[i].vTex.x = 0.0f;
				m_MeshInfo.pvVertex[i].vTex.y = 0.0f;
			}
		}
	}
	else
	{
		//法線別割り当て
		if (uv.size() >= 1)
		{
			//uv有りバージョン
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				m_MeshInfo.pvVertex[i].vPos = coordinate[i];
				m_MeshInfo.pvVertex[i].vTex = uv[i];
				m_MeshInfo.pvVertex[i].vNormal = normal[i % 3];
			}
		}
		else
		{
			//uvなしバージョン
			for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
			{
				m_MeshInfo.pvVertex[i].vPos = coordinate[i];
				m_MeshInfo.pvVertex[i].vNormal = normal[i / 3];
				m_MeshInfo.pvVertex[i].vTex.x = 0.0f;
				m_MeshInfo.pvVertex[i].vTex.y = 0.0f;
			}
		}
	}

	//ボーン読み込み
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		std::cerr << "失敗" << std::endl;
	}
	AddBoneHierarchy(&m_BornInfo.sBorn, &ifs, 0); //ボーン階層構造読み込み
	LoadAnimation(fileName, m_MeshInfo.pvVertex); //アニメーションとスキンウェイト読み込み

	//バーテックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_MeshInfo.pvVertex;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.m_pVertexBuffer)))
		return FALSE;

	//削除
	face.clear();
	face.shrink_to_fit();
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

void LoadXDynamic::ChangeAnimation(int num)
{
	m_AnimSetNum = num;
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

Vector3D LoadXDynamic::GetScale()
{
	auto born2 = m_BornInfo.BornList.end();
	born2--;
	return Vector3D((*born2)->initMat._11, (*born2)->initMat._22, (*born2)->initMat._33);
}

Vector3D LoadXDynamic::GetRotation()
{
	auto born1 = m_BornInfo.BornList.begin();
	return Vector3D(-atan2f((*born1)->initMat._32, (*born1)->initMat._33), asinf((*born1)->initMat._31) - 3.14f, -atan2f((*born1)->initMat._21, (*born1)->initMat._11));
}

void LoadXDynamic::Update()
{
	//アニメーション更新
	AnimUpdate(m_BornInfo.sBorn.child);

	D3DXMATRIX m;;
	D3DXMatrixIdentity(&m);

	//ボーン更新
	BornMatUpdate(m_BornInfo.sBorn.child, m);

	auto it = m_BornInfo.BornList.begin();
	for (int i = 0; it != m_BornInfo.BornList.end(); it++, i++)
	{
		m_MeshInfo.BornBuffer.bornMat[i] = (*it)->bornMat;
	}
}

void LoadXDynamic::BornDebug(eBornDebug eBornDebug)
{
	auto it = m_BornInfo.BornList.begin();
	for (; it != m_BornInfo.BornList.end(); it++)
	{
		D3DXMATRIX m;
		std::string str;
		switch (eBornDebug)
		{
		case eInitMat:   m = (*it)->initMat; str = "InitMat"; break;
		case eOffsetMat: m = (*it)->offsetMat; str = "OffsetMat"; break;
		case eWorld:     m = (*it)->worldMat; str = "WorldMat"; break;
		case eBornMat:   m = (*it)->bornMat; str = "BornMat"; break;
		default: break;
		}

		//行列描画
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
}

void LoadXDynamic::LoadAnimation(std::string fileName, SkinVertexInfo* pVB)
{
	std::map<std::string, SkinWeightInfo> SkinWeightInfo; //各ボーンのスキンウェイト値とリスト
	std::map<int, std::map<std::string, AnimationInfo>> AnimationSet;
	int AnimSetNumAll = 0;

	std::ifstream ifs(fileName);
	std::string str;
	if (ifs.fail()) std::cerr << "失敗" << std::endl;

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "//") for (int i = 0; i < 4; i++) ifs >> str;

		//テンプレートでの間違い防止	
		if (str == "template")
		{
			while (str != "}") ifs >> str;
			continue;
		}

		//ウェイト値とリスト読み込み
		if (str == "SkinWeights")
		{
			//ボーン名取得
			ifs >> str;
			ifs >> str;

			//余分な文字削除
			std::string bornName = str;
			for (int nameCnt = 0; nameCnt < (int)bornName.size(); nameCnt++)
			{
				if (bornName[nameCnt] == '"') bornName.erase(bornName.begin() + nameCnt);
				if (bornName[nameCnt] == ';') bornName.erase(bornName.begin() + nameCnt);
			}

			//重みリスト数
			int num; ifs >> num;
			SkinWeightInfo[bornName].listNumAll = num;

			//getline(ifs, str);
			ifs >> str;

			//重みリスト読み込み		
			for (int index = 0, i = 0; i < num; i++)
			{
				ifs >> index;
				ifs >> str;
				SkinWeightInfo[bornName].weightList.emplace_back(index);
			}

			//重み値読み込み
			for (float weight = 0.0f, i = 0; i < num; i++)
			{
				ifs >> weight;
				ifs >> str;
				SkinWeightInfo[bornName].weight.emplace_back(weight);
			}

			getline(ifs, str);

			//ボーンオフセット行列読み込み
			float x, y, z, w;
			for (int matCnt = 0; matCnt < 4; matCnt++)
			{
				getline(ifs, str);
				std::istringstream stream(str);
				getline(stream, str, ','); x = std::stof(str);
				getline(stream, str, ','); y = std::stof(str);
				getline(stream, str, ','); z = std::stof(str);
				getline(stream, str, ','); w = std::stof(str);
				SkinWeightInfo[bornName].offsetMat.m[matCnt][0] = x;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][1] = y;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][2] = z;
				SkinWeightInfo[bornName].offsetMat.m[matCnt][3] = w;
			}
		}

		//アニメーションセットを読み込み
		if (str == "AnimationSet")
		{
			int count = 1;
			bool is = true;
			bool isFrameLoad = false;

			while (!ifs.eof())
			{
				ifs >> str;
				if (str == "{") count++;
				if (str == "}") count--;

				if (count == 0) break; //アニメーションセット終了
				if (is)
				{
					is = false;
					count--;
				}

				//アニメーション読み込み
				if (str == "Animation")
				{
					ifs >> str;
					count++;

					ifs >> str;

					//瓜括弧排除
					std::string bornName = str;
					for (int j = 0; j < (int)bornName.size(); j++)
					{
						if (bornName[j] == '{') bornName.erase(bornName.begin() + j);
						if (bornName[j] == '}') bornName.erase(bornName.begin() + j);
					}

					//改行
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					int t = std::stoi(str); //コマ数
					getline(ifs, str);

					//回転値取得
					for (int r = 0; r < t; r++)
					{
						std::istringstream stream(str);
						getline(stream, str, ';'); int frameNum = std::stoi(str);
						getline(stream, str, ';');
						getline(stream, str, ','); float w = std::stof(str);
						getline(stream, str, ','); float x = std::stof(str);
						getline(stream, str, ','); float y = std::stof(str);
						getline(stream, str, ';'); float z = std::stof(str);
						getline(ifs, str);
						stream.clear();

						AnimationSet[AnimSetNumAll][bornName].rotation.emplace_back(x, y, z, w);

						//アニメーションセットのフレーム読み込みセットに一つだけ
						if (!isFrameLoad)
						{
							m_BornInfo.AnimationSetFrameNum[AnimSetNumAll].emplace_back(frameNum);
						}
					}
					isFrameLoad = true;
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					t = std::stoi(str); //コマ数
					getline(ifs, str);

					//スケール値取得
					for (int s = 0; s < t; s++)
					{
						std::istringstream stream(str);
						getline(stream, str, ';'); int frameNum = std::stoi(str);
						getline(stream, str, ';');
						getline(stream, str, ','); float x = std::stof(str);
						getline(stream, str, ','); float y = std::stof(str);
						getline(stream, str, ';'); float z = std::stof(str);
						getline(ifs, str);
						stream.clear();

						AnimationSet[AnimSetNumAll][bornName].scale.emplace_back(x, y, z);
					}
					getline(ifs, str);
					getline(ifs, str);
					getline(ifs, str);

					t = std::stoi(str); //コマ数
					getline(ifs, str);

					//位置取得
					for (int p = 0; p < t; p++)
					{
						std::istringstream stream(str);
						getline(stream, str, ';'); int frameNum = std::stoi(str);
						getline(stream, str, ';');
						getline(stream, str, ','); float x = std::stof(str);
						getline(stream, str, ','); float y = std::stof(str);
						getline(stream, str, ';'); float z = std::stof(str);
						getline(ifs, str);

						AnimationSet[AnimSetNumAll][bornName].position.emplace_back(x, y, z);
					}
				}
			}

			AnimSetNumAll++; //アニメーションセット数
		}
	}

	//アニメーションセットをマトリックスに変換
	D3DXMATRIX m;
	D3DXMATRIX S;
	auto it = AnimationSet.begin();
	for (int i = 0; it != AnimationSet.end(); it++, i++)
	{
		int loopNum = m_BornInfo.AnimationSetFrameNum[i].size();
		auto it2 = it->second.begin();
		for (; it2 != it->second.end(); it2++)
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

				//平行移動
				m._41 = position.x;
				m._42 = position.y;
				m._43 = position.z;

				m_BornInfo.AnimationSetMat[i][it2->first].emplace_back(m);
			}
		}
	}

	//頂点にボーンの番号とウェイト値を割り当てる
	auto it2 = SkinWeightInfo.begin();
	for (; it2 != SkinWeightInfo.end(); it2++)
	{
		//ボーンに割り当ててる数分実行
		for (int i = 0; i < it2->second.listNumAll; i++)
		{
			//一つの頂点にボーンは4つまで空いているとこに格納
			int BornIndex = it2->second.weightList[i];
			for (int j = 0; j < 4; j++)
			{
				if (pVB[BornIndex].BoneIndex[j] == 0)
				{
					//該当するボーンのインデックス番号格納
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

					//ボーンインデックスとウェイト値格納
					pVB[BornIndex].BoneIndex[j] = BornNum;
					pVB[BornIndex].BoneWeight[j] = it2->second.weight[i];
					break;
				}
			}
		}
	}

	//ボーンオフセット行列格納
	auto itBornList = m_BornInfo.BornList.begin();
	for (; itBornList != m_BornInfo.BornList.end(); itBornList++)
	{
		(*itBornList)->offsetMat = SkinWeightInfo[(*itBornList)->BornName].offsetMat;
	}

	//アニメーションセット削除
	auto itAnim = AnimationSet.begin();
	for (; itAnim != AnimationSet.end(); itAnim++)
	{
		//回転とスケールと平行移動を削除
		auto anim = itAnim->second.begin();
		for (; anim != itAnim->second.end(); anim++)
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

	//スキンウェイト削除
	auto itWeight = SkinWeightInfo.begin();
	for (; itWeight != SkinWeightInfo.end(); itWeight++)
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

	std::istringstream stream(str);
	float x, y, z, w;
	for (int matCnt = 0; matCnt < 4; matCnt++)
	{
		std::istringstream stream(str);
		getline(stream, str, ','); x = std::stof(str);
		getline(stream, str, ','); y = std::stof(str);
		getline(stream, str, ','); z = std::stof(str);
		getline(stream, str, ','); w = std::stof(str);
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

	//アニメーションセットのフレーム時間
	std::vector<int> *pFrameNum = &m_BornInfo.AnimationSetFrameNum[m_AnimSetNum];
	auto itFrame = pFrameNum->begin();

	std::vector<D3DXMATRIX> *pAnimMat = &m_BornInfo.AnimationSetMat[m_AnimSetNum][pBorn->BornName];
	auto itAnimMat = pAnimMat->begin();

	//アニメーション補間
	if (pAnimMat->size() != 0)
	{
		for (unsigned int i = 0; i < pFrameNum->size() - 1; i++)
		{
			if ((m_FrameAnim >= (float)itFrame[i]) && (m_FrameAnim <= (float)itFrame[i + 1]))
			{
				//フレーム時間
				float animKeyB = (float)itFrame[i];
				float animKeyF = (float)itFrame[i + 1];
				float lengeTime = m_FrameAnim - animKeyB;

				float lenge = animKeyF - animKeyB;
				float t = lengeTime / lenge;

				D3DXMATRIX mb = itAnimMat[i];
				D3DXMATRIX mf = itAnimMat[i + 1];
				m = mb * (1 - t) + mf * t;

				break;
			}
		}
	}

	if ((m_FrameAnim < 0.0f) || (m_FrameAnim > pFrameNum->size() - 1))
	{
		m_FrameAnim = 0;
	}
	else
	{
		pBorn->worldMat = m;
	}

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
	int current = hierarchy; //現在の階層
	std::string str;
	std::ifstream::pos_type beg;

	while (!pIfs->eof())
	{
		*pIfs >> str;

		//テンプレートでの間違い防止	
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

			//子を追加
			if ((begin == 0) && (end == 0) || (end - begin == -1))
			{
				pBorn->child = new Born;
				pBorn->child->BornName = str;
				pBorn->child->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->child);

				//行列読み込み
				beg = pIfs->tellg();
				LoadMat(pBorn->child, pIfs);
				pIfs->seekg(beg);

				hierarchy++;
				AddBoneHierarchy(pBorn->child, pIfs, hierarchy);
			}

			//階層戻り先にきた場合、同一階層追加
			if (m_Back == current)
			{
				m_Back = -1;
				pBorn->brother = new Born;
				pBorn->brother->BornName = m_buffer;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//行列読み込み
				beg = pIfs->tellg();
				LoadMat(pBorn->brother, pIfs);
				pIfs->seekg(beg);

				AddBoneHierarchy(pBorn->brother, pIfs, current);
			}

			//"}"が"{"より多い時は階層を戻る
			if (end - begin > 0){
				m_buffer = str;
				m_Back = current - (end - begin);
				return true;
			}

			//同一階層追加
			if ((end - begin == 0) && ((begin != 0) && (end != 0)))
			{
				pBorn->brother = new Born;
				pBorn->brother->BornName = str;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//行列読み込み
				beg = pIfs->tellg();
				LoadMat(pBorn->brother, pIfs);
				pIfs->seekg(beg);

				AddBoneHierarchy(pBorn->brother, pIfs, current);
			}

			//階層を戻る
			if ((m_Back != -1) && (m_Back < current)) return true;
		}
	}
	return true;
}