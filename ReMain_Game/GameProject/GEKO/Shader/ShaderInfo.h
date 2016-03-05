#ifndef _SHADER_INFO_
#define _SHADER_INFO_

#include "..\System\Direct3D11.h"

#define MAX_BONES 255

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

#endif