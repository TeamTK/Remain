#include "LoadXStatic.h"
#include "..\System\Window.h"

LoadXStatic::LoadXStatic(std::string fileName)
{
	//メッシュの読み込み
	if (FAILED(LoadXMesh(fileName)))
	{
		MessageBoxA(0, "メッシュの読み込みに失敗しました", NULL, MB_OK);
	}

	//シェーダーの初期化
	if (FAILED(InitShader()))
	{
		MessageBoxA(0, "シェーダーの初期化に失敗しました", NULL, MB_OK);
	}
}

LoadXStatic::~LoadXStatic()
{
}

HRESULT LoadXStatic::LoadXMesh(std::string fileName)
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
	/////////////////////////////////////////////////////////////////////////////////変更
	std::vector<int> normalIndex;
	/////////////////////////////////////////////////////////////////////////////////変更
	std::vector<D3DXVECTOR3> coordinate;
	std::vector<D3DXVECTOR3> normal;
	std::vector<D3DXVECTOR2> uv;
	std::vector<MaterialX> material;
	std::vector<int> materialList;

	int index1, index2, index3;
	int frameNum = 0;
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

		//ローカル行列所得
		if (strcmp(key, "Frame") == 0)
		{
			frameNum++;
			assert(frameNum <= 2  && "アニメーション情報がありますStaticMeshでは読み込めません");
			Matrix m;
			fgets(str, sizeof(str), fp);
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._11, &m._12, &m._13, &m._14);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._21, &m._22, &m._23, &m._24);
			fscanf_s(fp, "%f, %f, %f, %f,", &m._31, &m._32, &m._33, &m._34);
			fscanf_s(fp, "%f, %f, %f, %f;;", &m._41, &m._42, &m._43, &m._44);

			if (frameNum == 2) m_MeshInfo.m_LocalMat *= m;
			else m_MeshInfo.m_LocalMat = m;
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

			/////////////////////////////////////////////////////////////////////////////////変更
			assert(leftCurlyBracket == '{' && "法線の初め波括弧が正しくありません");
			/////////////////////////////////////////////////////////////////////////////////変更

			//法線の数
			fgets(str, sizeof(str), fp);
			fscanf_s(fp, "%d;", &m_MeshInfo.normalNumAll);

			//法線読み込み
			for (int i = 0; i < m_MeshInfo.normalNumAll; i++)
			{
				fscanf_s(fp, "%f; %f; %f;,", &x, &y, &z);
				normal.emplace_back(x, y, z);
			}

			/////////////////////////////////////////////////////////////////////////////////変更
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
			/////////////////////////////////////////////////////////////////////////////////変更
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

				//テクスチャー読み込みディレクトリにする
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
	m_MeshInfo.m_pMaterial = new MaterialInfo[m_MeshInfo.materialNumAll];
	m_MeshInfo.m_ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];
	m_MeshInfo.pvVertex = new VertexInfo[m_MeshInfo.vertexNumAll];

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

		delete [] faceBuffer;
		faceBuffer = nullptr;
	}

	SAFE_DELETE_ARRAY(faceBuffer);

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
				/////////////////////////////////////////////////////////////////////////////////変更
				//m_MeshInfo.pvVertex[i].vNormal = normal[i % 3];
				/////////////////////////////////////////////////////////////////////////////////変更
				m_MeshInfo.pvVertex[i].vTex.x = 0.0f;
				m_MeshInfo.pvVertex[i].vTex.y = 0.0f;
			}
			/////////////////////////////////////////////////////////////////////////////////変更
			for (int i = 0; i < face.size(); i++)
			{
				m_MeshInfo.pvVertex[face[i]].vNormal = normal[normalIndex[i]];
			}
			/////////////////////////////////////////////////////////////////////////////////変更

		}
	}

	//バーテックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_MeshInfo.pvVertex;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.m_pVertexBuffer)))
		return FALSE;

	 face.clear();
	 face.shrink_to_fit();
	 /////////////////////////////////////////////////////////////////////////////////変更
	 normalIndex.clear();
	 normalIndex.shrink_to_fit();
	 /////////////////////////////////////////////////////////////////////////////////変更
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

void LoadXStatic::Relese()
{
	if (m_MeshInfo.m_pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.m_pMaterial[i].pTexture);
			m_MeshInfo.m_ppIndexBuffer[i]->Release();
		}

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