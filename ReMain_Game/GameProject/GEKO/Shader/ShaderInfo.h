#ifndef _SHADER_INFO_
#define _SHADER_INFO_

#include "..\System\Direct3D11.h"

#define MAX_BONES 255
#define FORWARD_LIGHT_MAX 10
#define DEFERRED_LIGHT_MAX 100
#define SHADER_TYPE_TOTAL 2
#define SHADER_TYPE_TOTAL_VS 2
#define SHADER_TYPE_TOTAL_PS 4

//全てのメッシュに共通
struct CommonInfoConstantBuffer
{
	Vector4D lightDir;	//ライト方向
	Vector4D intensity;	//ライトの明るさと色
	Vector4D eyePos;	//カメラ位置
	Vector4D fogColor;  //フォグの色
	ALIGN16 float fogDensity; //フォグの密度
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
struct BoneConstantBuffer
{
	Matrix boneMat[MAX_BONES];
	BoneConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&boneMat[i]);
		}
	}
};

//シェーダーに渡す用（前方レンダリング用ライト）
struct ForwardLightConstantBuffer
{
	Vector4D lihgtPos[FORWARD_LIGHT_MAX];
	Vector4D range[FORWARD_LIGHT_MAX];
	ALIGN16 float lightNum;
};

//シェーダーに渡す用（後方レンダリング用ライト）
struct DeferredLightConstantBuffer
{
	Vector4D lihgtPos[DEFERRED_LIGHT_MAX];
	Vector4D range[DEFERRED_LIGHT_MAX];
	ALIGN16 float lightNum;
};

enum class ShaderType
{
	eNormal = 0,
	eShadow = 1,
};

enum class ShaderTypeVS
{
	eNormal = 0,
	eNormal_Shadow = 1,
};

enum class ShaderTypePS
{
	eNormal = 0,
	eNormal_Shadow = 1,
	eNormal_Fog = 2,
	eShadow_Fog = 3,
};

#endif