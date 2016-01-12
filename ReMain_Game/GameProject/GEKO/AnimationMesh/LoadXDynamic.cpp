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
	m_BornIndex(0)
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

			//法線の数
			int NormalNumAll = 0;
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &NormalNumAll);

			//法線のインデックス番号読み込み
			int NormalNum = 0;
			for (int i = 0; i < NormalNumAll; i++)
			{
				fscanf_s(fp, "%d; %d, %d, %d;,", &NormalNum, &index1, &index2, &index3);

				assert(NormalNum == 3 && "Xファイルの3Dモデルは法線が三つではありません");

				normalIndex.emplace_back(index1);
				normalIndex.emplace_back(index2);
				normalIndex.emplace_back(index3);
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

	assert(coordinate.size() != 0 && "Xファイルの頂点座標がありません");
	assert(normal.size() != 0 && "Xファイルの法線がありません");
	assert(material.size() != 0 && "Xファイルのマテリアルがありません");

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

		//マテリアル割り当て
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
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(pDevice, m_MeshInfo.m_pMaterial[i].TextureName.c_str(), NULL, NULL, &m_MeshInfo.m_pMaterial[i].pTexture, NULL)))
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
	//頂点に割り当て
	if (uv.size() >= 1)
	{
		//uv有りバージョン
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[face[i]].vNormal = normal[normalIndex[i]];
			m_MeshInfo.pvVertex[i].vTex = uv[i];
		}
	}
	else
	{
		//uvなしバージョン
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_MeshInfo.pvVertex[i].vPos = coordinate[i];
			m_MeshInfo.pvVertex[face[i]].vNormal = normal[normalIndex[i]];
		}
	}

	fseek(fp, 0, SEEK_SET);
	AddBoneHierarchy(&m_BornInfo.sBorn, fp, 0); //ボーン階層構造読み込み

	fseek(fp, 0, SEEK_SET);
	LoadAnimation(fp, m_MeshInfo.pvVertex); //アニメーションとスキンウェイト読み込み

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

void LoadXDynamic::CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal)
{
	//子ボーン
	if (pBornOriginal->child != nullptr)
	{
		pBornCopy->child = new CopyBorn;
		pCopyBornArray->emplace_back(pBornCopy->child);
		CopyBornTree(pBornCopy->child, pCopyBornArray, pBornOriginal->child);
	}

	//親ボーン
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

	//ボーン更新
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	BornMatUpdate(pCopyBorn, m_BornInfo.sBorn.child, m);
}

void LoadXDynamic::BornDebug(eBornDebug eBornDebug)
{
	/*
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
	*/
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

void LoadXDynamic::LoadAnimation(FILE* fp, SkinVertexInfo* pVB)
{
	std::map<std::string, SkinWeightInfo> SkinWeightInfo; //各ボーンのスキンウェイト値とリスト
	std::map<int, std::map<std::string, AnimationInfo>> AnimationSet;
	int AnimSetNumAll = 0;

	char key[256];

	while (!feof(fp))
	{

		fscanf_s(fp, "%s", key, sizeof(key));

		if (strcmp(key, "//") == 0) for (int i = 0; i < 4; i++) fscanf_s(fp, "%s", key, sizeof(key));

		//テンプレートでの間違い防止	
		if (strcmp(key, "template") == 0)
		{
			while (strcmp(key, "}") != 0) fscanf_s(fp, "%s", key, sizeof(key));
			continue;
		}

		//ウェイト値とリスト読み込み
		if (strcmp(key, "SkinWeights") == 0)
		{
			//ボーン名取得
			fscanf_s(fp, "%s", key, sizeof(key));
			fscanf_s(fp, "%s", key, sizeof(key));

			//余分な文字削除
			std::string bornName = key;
			for (int nameCnt = 0; nameCnt < (int)bornName.size(); nameCnt++)
			{
				if (bornName[nameCnt] == '"') bornName.erase(bornName.begin() + nameCnt);
				if (bornName[nameCnt] == ';') bornName.erase(bornName.begin() + nameCnt);
			}

			//重みリスト数
			int num;
			fscanf_s(fp, "%d;", &num);
			SkinWeightInfo[bornName].listNumAll = num;

			//重みリスト読み込み		
			for (int index = 0, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%d,", &index);
				//fscanf_s(fp, "%s", key, sizeof(key));
				SkinWeightInfo[bornName].weightList.emplace_back(index);
			}
			fscanf_s(fp, "%s", key, sizeof(key));

			//重み値読み込み
			for (float weight = 0.0f, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%f,", &weight);
				SkinWeightInfo[bornName].weight.emplace_back(weight);
			}

			fgets(key, sizeof(key), fp);

			//ボーンオフセット行列読み込み
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

		//アニメーションセットを読み込み
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

				if (count == 0) break; //アニメーションセット終了
				if (is)
				{
					is = false;
					count--;
				}

				//アニメーション読み込み
				if (strcmp(key, "Animation") == 0)
				{
					fscanf_s(fp, "%s", key, sizeof(key));
					count++;
					fscanf_s(fp, "%s", key, sizeof(key));

					//瓜括弧排除
					std::string bornName = key;
					for (int j = 0; j < (int)bornName.size(); j++)
					{
						if (bornName[j] == '{') bornName.erase(bornName.begin() + j);
						if (bornName[j] == '}') bornName.erase(bornName.begin() + j);
					}

					//改行
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					int t = std::stoi(key); //コマ数
					fgets(key, sizeof(key), fp);
					
					int temp;
					//回転値取得
					for (int r = 0; r < t; r++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f, %f;;,", &frameNum, &temp, &w, &x, &y, &z);
						fgets(key, sizeof(key), fp);

						AnimationSet[AnimSetNumAll][bornName].rotation.emplace_back(x, y, z, w);

						//アニメーションセットのフレーム読み込みセットに一つだけ
						if (!isFrameLoad)
						{
							m_BornInfo.AnimationSetFrameNum[AnimSetNumAll].emplace_back(frameNum);
						}
					}
					isFrameLoad = true;
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					t = std::stoi(key); //コマ数
					fgets(key, sizeof(key), fp);

					//スケール値取得
					for (int s = 0; s < t; s++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						fgets(key, sizeof(key), fp);

						AnimationSet[AnimSetNumAll][bornName].scale.emplace_back(x, y, z);
					}
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);
					fgets(key, sizeof(key), fp);

					t = std::stoi(key); //コマ数
					fgets(key, sizeof(key), fp);

					//位置取得
					for (int p = 0; p < t; p++)
					{
						sscanf_s(key, "%d;%d; %f, %f, %f;;,", &frameNum, &temp, &x, &y, &z);
						fgets(key, sizeof(key), fp);

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
	auto it2End = SkinWeightInfo.end();
	for (; it2 != it2End; it2++)
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
	auto itBornListEnd = m_BornInfo.BornList.end();
	for (; itBornList != itBornListEnd; itBornList++)
	{
		(*itBornList)->offsetMat = SkinWeightInfo[(*itBornList)->BornName].offsetMat;
	}

	//アニメーションセット削除
	auto itAnim = AnimationSet.begin();
	auto itAnimEnd = AnimationSet.end();
	for (; itAnim != itAnimEnd; itAnim++)
	{
		//回転とスケールと平行移動を削除
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

	//スキンウェイト削除
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

void LoadXDynamic::LoadMat(Born *pBorn, FILE *fp)
{
	char key[256];
	fgets(key, sizeof(key), fp);
	fgets(key, sizeof(key), fp);
	fgets(key, sizeof(key), fp);

	D3DXMATRIX m;

	float x, y, z, w;
	for (int matCnt = 0; matCnt < 4; matCnt++)
	{
		sscanf_s(key, "%f, %f, %f, %f,", &x, &y, &z, &w);
		m.m[matCnt][0] = x; m.m[matCnt][1] = y; m.m[matCnt][2] = z; m.m[matCnt][3] = w;
		fgets(key, sizeof(key), fp);
	}

	pBorn->initMat = m;
}

void LoadXDynamic::BornMatUpdate(CopyBorn *pCopyBorn, Born *pBorn, D3DXMATRIX &bornMat)
{
	pCopyBorn->ParentAndChildMat = pCopyBorn->worldMat * bornMat;
	pCopyBorn->bornMat = pBorn->offsetMat * pCopyBorn->ParentAndChildMat;

	if (pBorn->child != nullptr) BornMatUpdate(pCopyBorn->child, pBorn->child, pCopyBorn->ParentAndChildMat);
	if (pBorn->brother != nullptr) BornMatUpdate(pCopyBorn->brother, pBorn->brother, bornMat);
}

void LoadXDynamic::AnimUpdate(CopyBorn *pCopyBorn, Born *pBorn)
{
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);

	auto frameAnimNum = m_BornInfo.AnimationSetFrameNum[pCopyBorn->animNum].size() - 1;

	//アニメーションセットのフレーム時間
	auto itFrame = m_BornInfo.AnimationSetFrameNum[pCopyBorn->animNum].begin();

	std::vector<D3DXMATRIX> *pAnimMat = &m_BornInfo.AnimationSetMat[pCopyBorn->animNum][pBorn->BornName];
	auto itAnimMat = pAnimMat->begin();

	//指定のアニメーションフレームを超えたら戻す
	if (pCopyBorn->animFrame > frameAnimNum)
	{
		pCopyBorn->animFrame = 0.0f;
	}
	else if (pCopyBorn->animFrame < 0.0f)
	{
		pCopyBorn->animFrame = (float)frameAnimNum;
	}

	//アニメーション補間
	if (pAnimMat->size() != 0)
	{
		unsigned int frameNum = frameAnimNum;
		for (unsigned int i = 0; i < frameNum; i++)
		{
			float frameBefore = (float)itFrame[i];
			float frameAfter = (float)itFrame[i + 1];
			if ((pCopyBorn->animFrame >= frameBefore) && (pCopyBorn->animFrame <= frameAfter))
			{
				//フレーム時間
				float lengeTime = pCopyBorn->animFrame - frameBefore;

				float lenge = frameAfter - frameBefore;
				float t = lengeTime / lenge;

				D3DXMATRIX mb = itAnimMat[i];
				D3DXMATRIX ma = itAnimMat[i + 1];
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
	int current = hierarchy; //現在の階層
	int filePos = 0;
	char key[256];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s", key, sizeof(key));
		if (strcmp(key, "Mesh") == 0) break;

		//テンプレートでの間違い防止	
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

			//子を追加
			if ((begin == 0) && (end == 0) || (end - begin == -1))
			{
				pBorn->child = new Born;
				pBorn->child->BornName = key;
				pBorn->child->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->child);

				//行列読み込み
				filePos = ftell(fp);
				LoadMat(pBorn->child, fp);
				int e = fseek(fp, filePos, SEEK_SET);

				hierarchy++;
				AddBoneHierarchy(pBorn->child, fp, hierarchy);
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
				filePos = ftell(fp);
				LoadMat(pBorn->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBorn->brother, fp, current);
			}

			//"}"が"{"より多い時は階層を戻る
			if (end - begin > 0){
				m_buffer = key;
				m_Back = current - (end - begin);
				return true;
			}

			//同一階層追加
			if ((end - begin == 0) && ((begin != 0) && (end != 0)))
			{
				pBorn->brother = new Born;
				pBorn->brother->BornName = key;
				pBorn->brother->indexId = m_BornIndex++;
				m_BornInfo.BornList.emplace_back(pBorn->brother);

				//行列読み込み
				filePos = ftell(fp);
				LoadMat(pBorn->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBorn->brother, fp, current);
			}

			//階層を戻る
			if ((m_Back != -1) && (m_Back < current)) return true;
		}
	}
	return true;
}