#ifndef _CONSTANT_SHADER_H_
#define _CONSTANT_SHADER_H_

#include "..\System\Direct3D11.h"
#include "..\AnimationMesh\SkinMeshData.h"

//全てのメッシュに共通
struct CommonInfoConstantBuffer
{
	Vector4D lightDir;	//ライト方向
	Vector4D intensity;	//ライトの明るさ
	Vector4D eyePos;	//カメラ位置
};

//シェーダーに渡す用(変換行列など)
struct TransformMatrixConstantBuffer
{
	Matrix mW;	 //ワールド行列
	Matrix mWVP; //ワールドから射影までの変換行列
	Matrix mWLP; //ライトから見た変換行列
};

//シェーダーに渡す用（マテリアル）
struct MaterialConstantBuffer
{
	Vector4D ambient;	//アンビエント光
	Vector4D diffuse;	//ディフューズ色
	Vector4D specular;	//鏡面反射
};

//シェーダーに渡す用（ボーン）
struct BornConstantBuffer
{
	Matrix bornMat[MAX_BONES];
	BornConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&bornMat[i]);
		}
	}
};

class ConstantShader
{
public:
	~ConstantShader();
	static ConstantShader* GetInstance();
	bool Init();
	void SetCommonInfoConstantBuffer();
	void SetTransformMatrixConstantBuffer(ID3D11DeviceContext *pDeviceContext, Matrix &mat, bool isShadow);
	void SetMaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, 
								   const Vector4D &diffuse, const Vector4D &specular, const Vector4D &ambient);
	void SetBornConstantBuffer(ID3D11DeviceContext *pDeviceContext, int bornAllNum, std::vector<CopyBorn*> &pCopyBornArray);
	void Release();

private:
	ConstantShader();

private:
	ID3D11Buffer* m_pCommonInfoConstantBuffer;		 //コンスタントバッファー（共通情報）
	ID3D11Buffer* m_pTransformMatrixConstantBuffer;  //コンスタントバッファー（変換行列）
	ID3D11Buffer* m_pMaterialConstantBuffer;		 //コンスタントバッファー（マテリアル）
	ID3D11Buffer* m_pBornConstantBuffer;			 //コンスタントバッファー（ボーン）
};

#endif