#include "LoadXDynamic.h"
#include "..\..\System\Window.h"

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
	m_BoneIndex(0)
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
}

HRESULT LoadXDynamic::LoadXMesh(std::string fileName)
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

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
	std::vector<Vector3D> coordinate;
	std::vector<Vector3D> normal;
	std::vector<Vector2D> uv;
	std::vector<Vector3D> ambient;
	std::vector<Vector4D> diffuse;
	std::vector<Vector3D> specular;
	std::vector<std::string> textureNameArray;
	std::vector<int> materialList;

	int index1, index2, index3;
	int uvNumAll = 0; //UVの数
	int normalNumAll = 0; //法線の数
	int materialListNumAll = 0;
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
			fscanf_s(fp, "%d;", &normalNumAll);

			//法線読み込み
			for (int i = 0; i < normalNumAll; i++)
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
			fscanf_s(fp, "%d;", &uvNumAll);

			for (int i = 0; i < uvNumAll; i++)
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
			fscanf_s(fp, "%d;", &materialListNumAll);

			//マテリアルリスト格納
			int listNum = 0;
			for (int i = 0; i < materialListNumAll; i++)
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

			Vector4D kd(0.0f, 0.0f, 0.0f, 0.0f);
			float sp = 0.0f;
			Vector3D ks(0.0f, 0.0f, 0.0f);
			Vector3D ka(0.0f, 0.0f, 0.0f);
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

	assert(coordinate.size() != 0 && "Xファイルの頂点座標がありません");
	assert(normal.size() != 0 && "Xファイルの法線がありません");
	assert(ambient.size() != 0 && "Xファイルのマテリアルがありません");

	//マテリアルとインデックスバッファー動的作成
	m_MeshInfo.pMaterial = new MaterialInfo[m_MeshInfo.materialNumAll];
	m_MeshInfo.ppIndexBuffer = new ID3D11Buffer*[m_MeshInfo.materialNumAll];
	m_pVertex = new SkinVertexInfo[m_MeshInfo.vertexNumAll];

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
		m_MeshInfo.pMaterial[i].ambient = ambient[i];
		m_MeshInfo.pMaterial[i].diffuse = diffuse[i];
		m_MeshInfo.pMaterial[i].specular = specular[i];

		m_MeshInfo.pMaterial[i].textureName = textureNameArray[i];

		if (textureNameArray[i] == "NoTexture")
		{
			m_MeshInfo.isTexture = false;
		}
		else
		{
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(pDevice, textureNameArray[i].c_str(), NULL, NULL, &m_MeshInfo.pMaterial[i].pTexture, NULL)))
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
				m_MeshInfo.pMaterial[i].numPolygon += 1;
				count += 3;
			}
		}
		if (count == 0) continue;

		//インデックスバッファーを作成
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
	//頂点に割り当て
	if (uv.size() >= 1)
	{
		//uv有りバージョン
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_pVertex[i].pos = coordinate[i];
			m_pVertex[face[i]].normal = normal[normalIndex[i]];
			m_pVertex[i].uv = uv[i];
		}
	}
	else
	{
		//uvなしバージョン
		for (int i = 0; i < m_MeshInfo.vertexNumAll; i++)
		{
			m_pVertex[i].pos = coordinate[i];
			m_pVertex[face[i]].normal = normal[normalIndex[i]];
		}
	}

	fseek(fp, 0, SEEK_SET);
	AddBoneHierarchy(&m_BoneInfo.bone, fp, 0); //ボーン階層構造読み込み

	fseek(fp, 0, SEEK_SET);
	LoadAnimation(fp, m_pVertex); //アニメーションとスキンウェイト読み込み

	//ローカル行列格納
	m_MeshInfo.localMatrix = m_BoneInfo.BoneList[0]->initMat *m_BoneInfo.BoneList[m_BoneInfo.BoneList.size() - 1]->initMat;

	//バーテックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertexInfo) * m_MeshInfo.vertexNumAll;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = m_pVertex;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_MeshInfo.pVertexBuffer)))
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

void LoadXDynamic::CopyBoneTree(CopyBone *pBoneCopy, std::vector<CopyBone*> *pCopyBoneArray, Bone *pBoneOriginal)
{
	//子ボーン
	if (pBoneOriginal->child != nullptr)
	{
		pBoneCopy->child = new CopyBone;
		pCopyBoneArray->emplace_back(pBoneCopy->child);
		CopyBoneTree(pBoneCopy->child, pCopyBoneArray, pBoneOriginal->child);
	}

	//親ボーン
	if (pBoneOriginal->brother != nullptr)
	{
		pBoneCopy->brother = new CopyBone;
		pCopyBoneArray->emplace_back(pBoneCopy->brother);
		CopyBoneTree(pBoneCopy->brother, pCopyBoneArray, pBoneOriginal->brother);
	}
}

void LoadXDynamic::Update(CopyBone *pCopyBone, unsigned int animNum, float *animFrame, bool *pIsAnimEnd)
{
	auto frameAnimNum = m_BoneInfo.AnimationSetFrameNum[(int)animNum].size() - 1;

	//指定のアニメーションフレームを超えたら戻す
	if (*animFrame > (float)frameAnimNum)
	{
		if (*pIsAnimEnd)
		{
			*animFrame = 0.0f;
			*pIsAnimEnd = false;
		}
		else
		{
			*pIsAnimEnd = true;
			return;
		}
	}
	else if (*animFrame < 0.0f)
	{
		if (*pIsAnimEnd)
		{
			*animFrame = (float)frameAnimNum;
			*pIsAnimEnd = false;
		}
		else
		{
			*pIsAnimEnd = true;
			return;
		}
	}

	AnimUpdate(pCopyBone, m_BoneInfo.bone.child, (int)animNum, *animFrame, frameAnimNum);

	//ボーン更新
	Matrix m;
	BoneMatUpdate(pCopyBone, m_BoneInfo.bone.child, m);
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
			std::string boneName = key;
			for (int nameCnt = 0; nameCnt < (int)boneName.size(); nameCnt++)
			{
				if (boneName[nameCnt] == '"') boneName.erase(boneName.begin() + nameCnt);
				if (boneName[nameCnt] == ';') boneName.erase(boneName.begin() + nameCnt);
			}

			//重みリスト数
			int num;
			fscanf_s(fp, "%d;", &num);
			SkinWeightInfo[boneName].listNumAll = num;

			//重みリスト読み込み		
			for (int index = 0, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%d,", &index);
				//fscanf_s(fp, "%s", key, sizeof(key));
				SkinWeightInfo[boneName].weightList.emplace_back(index);
			}
			fscanf_s(fp, "%s", key, sizeof(key));

			//重み値読み込み
			for (float weight = 0.0f, i = 0; i < num; i++)
			{
				fscanf_s(fp, "%f,", &weight);
				SkinWeightInfo[boneName].weight.emplace_back(weight);
			}

			fgets(key, sizeof(key), fp);

			//ボーンオフセット行列読み込み
			float x, y, z, w;
			for (int matCnt = 0; matCnt < 4; matCnt++)
			{
				fgets(key, sizeof(key), fp);
				sscanf_s(key, "%f, %f, %f, %f,", &x, &y, &z, &w);

				SkinWeightInfo[boneName].offsetMat.m[matCnt][0] = x;
				SkinWeightInfo[boneName].offsetMat.m[matCnt][1] = y;
				SkinWeightInfo[boneName].offsetMat.m[matCnt][2] = z;
				SkinWeightInfo[boneName].offsetMat.m[matCnt][3] = w;
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
					std::string boneName = key;
					for (int j = 0; j < (int)boneName.size(); j++)
					{
						if (boneName[j] == '{') boneName.erase(boneName.begin() + j);
						if (boneName[j] == '}') boneName.erase(boneName.begin() + j);
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

						AnimationSet[AnimSetNumAll][boneName].rotation.emplace_back(x, y, z, w);

						//アニメーションセットのフレーム読み込みセットに一つだけ
						if (!isFrameLoad)
						{
							m_BoneInfo.AnimationSetFrameNum[AnimSetNumAll].emplace_back(frameNum);
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

						AnimationSet[AnimSetNumAll][boneName].scale.emplace_back(x, y, z);
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

						AnimationSet[AnimSetNumAll][boneName].position.emplace_back(x, y, z);
					}
				}
			}

			AnimSetNumAll++; //アニメーションセット数
		}
	}

	//アニメーションセットをマトリックスに変換
	Matrix m;
	Matrix S;

	Vector4D rotation;
	Vector3D scale;
	Vector3D position;
	D3DXQUATERNION quaternionOut;

	int cnt = 0;
	for (auto& i : AnimationSet)
	{
		int loopNum = m_BoneInfo.AnimationSetFrameNum[cnt].size();
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

				//平行移動
				m._41 = position.x;
				m._42 = position.y;
				m._43 = position.z;

				m_BoneInfo.AnimationSetMat[cnt][j.first].emplace_back(m);
			}
		}
		cnt++;
	}

	//頂点にボーンの番号とウェイト値を割り当てる
	for (auto& i : SkinWeightInfo)
	{
		int cntBone = 0;
		for (int j = 0; j < i.second.listNumAll; j++)
		{
			//一つの頂点にボーンは4つまで空いているとこに格納
			int index = i.second.weightList[cntBone];
			for (int k = 0; k < 4; k++)
			{
				if (pVB[index].boneIndex[k] == 0)
				{
					//該当するボーンのインデックス番号格納
					int boneNum = 0;
					for (auto& m : m_BoneInfo.BoneList)
					{
						if (m->boneName == i.first)
						{
							boneNum = m->indexId;
							break;
						}
					}
					//ボーンインデックスとウェイト値格納
					pVB[index].boneIndex[k] = boneNum;
					pVB[index].boneWeight[k] = i.second.weight[j];
					break;
				}
			}
			cntBone++;
		}
	}

	//オフセット行列格納
	for (auto& i : m_BoneInfo.BoneList)
	{
		i->offsetMat = SkinWeightInfo[i->boneName].offsetMat;
	}

	//アニメーションセット削除
	for (auto& i : AnimationSet)
	{
		//回転とスケールと平行移動を削除
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

	//スキンウェイト削除
	for (auto& i : SkinWeightInfo)
	{
		i.second.weightList.clear();
		i.second.weightList.shrink_to_fit();
		i.second.weight.clear();
		i.second.weight.shrink_to_fit();
	}
	SkinWeightInfo.clear();
}

void LoadXDynamic::LoadMat(Bone *pBone, FILE *fp)
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

	pBone->initMat = m;
}

void LoadXDynamic::BoneMatUpdate(CopyBone *pCopyBone, Bone *pBone, Matrix &boneMat)
{
	pCopyBone->parentAndChildMat = pCopyBone->worldMat * boneMat;
	pCopyBone->boneMat = pBone->offsetMat * pCopyBone->parentAndChildMat;

	if (pBone->child != nullptr) BoneMatUpdate(pCopyBone->child, pBone->child, pCopyBone->parentAndChildMat);
	if (pBone->brother != nullptr) BoneMatUpdate(pCopyBone->brother, pBone->brother, boneMat);
}

void LoadXDynamic::AnimUpdate(CopyBone *pCopyBone, Bone *pBone, int animNum, float animFrame, unsigned int frameAnimNum)
{
	Matrix m;

	//アニメーションセットのフレーム時間
	auto itFrame = m_BoneInfo.AnimationSetFrameNum[animNum].begin();

	std::vector<Matrix> *pAnimMat = &m_BoneInfo.AnimationSetMat[animNum][pBone->boneName];
	auto itAnimMat = pAnimMat->begin();

	//アニメーション補間
	if (pAnimMat->size() != 0)
	{
		for (unsigned int i = 0; i < frameAnimNum; i++)
		{
			float frameBefore = (float)itFrame[i];
			float frameAfter = (float)itFrame[i + 1];
			if ((animFrame >= frameBefore) && (animFrame <= frameAfter))
			{
				//フレーム時間
				float lengeTime = animFrame - frameBefore;

				float lenge = frameAfter - frameBefore;
				float t = lengeTime / lenge;

				Matrix mb = itAnimMat[i];
				Matrix ma = itAnimMat[i + 1];

				m = mb * (1 - t) + ma * t;

				break;
			}
		}
	}

	pCopyBone->worldMat = m;

	if (pCopyBone->child != nullptr) AnimUpdate(pCopyBone->child, pBone->child, animNum, animFrame, frameAnimNum);
	if (pCopyBone->brother != nullptr) AnimUpdate(pCopyBone->brother, pBone->brother, animNum, animFrame, frameAnimNum);
}

bool LoadXDynamic::AddBoneHierarchy(Bone *pBone, FILE *fp, int hierarchy)
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
				pBone->child = new Bone;
				pBone->child->boneName = key;
				pBone->child->indexId = m_BoneIndex++;
				m_BoneInfo.BoneList.emplace_back(pBone->child);

				//行列読み込み
				filePos = ftell(fp);
				LoadMat(pBone->child, fp);
				int e = fseek(fp, filePos, SEEK_SET);

				hierarchy++;
				AddBoneHierarchy(pBone->child, fp, hierarchy);
			}

			//階層戻り先にきた場合、同一階層追加
			if (m_Back == current)
			{
				m_Back = -1;
				pBone->brother = new Bone;
				pBone->brother->boneName = m_buffer;
				pBone->brother->indexId = m_BoneIndex++;
				m_BoneInfo.BoneList.emplace_back(pBone->brother);

				//行列読み込み
				filePos = ftell(fp);
				LoadMat(pBone->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBone->brother, fp, current);
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
				pBone->brother = new Bone;
				pBone->brother->boneName = key;
				pBone->brother->indexId = m_BoneIndex++;
				m_BoneInfo.BoneList.emplace_back(pBone->brother);

				//行列読み込み
				filePos = ftell(fp);
				LoadMat(pBone->brother, fp);
				fseek(fp, filePos, SEEK_SET);

				AddBoneHierarchy(pBone->brother, fp, current);
			}

			//階層を戻る
			if ((m_Back != -1) && (m_Back < current)) return true;
		}
	}
	return true;
}