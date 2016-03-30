#include "../CommonMeshInfo.hlsl"

#define DEFERRED_LIGHT_MAX 100

//後方レンダリング用点光源
cbuffer Global_DeferredLight : register(b4)
{
	float4 g_DLihgtPos[DEFERRED_LIGHT_MAX];  //点光源の位置
	float4 g_DRange[DEFERRED_LIGHT_MAX];     //点光源の半径
	float g_DLihgtNum;						 //点光源の数
};

/******************************************/
//頂点情報出力用
/******************************************/

//頂点シェーダー出力構造体
struct VS_OUTPUT_DEFERRED
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float2 uv : TEXCOORD2;
};

//頂点シェーダー出力構造体(影あり)
struct VS_OUTPUT_DEFERRED_SHADOWMAP
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
	float4 lightTexCoord : TEXCOORD2;
	float2 uv : TEXCOORD3;
};

/******************************************/
//ピクセルシェーダー情報出力用
/******************************************/

//ピクセルシェーダー出力構造体（MRT）
struct PS_OUTPUT_DEFERRED
{
	float4 albedo : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

/******************************************/
//頂点シェーダー用関数
/******************************************/

//頂点を変換
VS_OUTPUT_DEFERRED GetVertexOutPut_Deferred(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;

	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;

	return output;
}

//頂点を変換（影）
VS_OUTPUT_DEFERRED_SHADOWMAP GetVertexShadowMapOutPut_Deferred(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_DEFERRED_SHADOWMAP output = (VS_OUTPUT_DEFERRED_SHADOWMAP)0;

	output.pos = mul(pos, g_mWVP);

	output.pos = mul(pos, g_mWVP);
	output.normal = mul(normal, (float3x3)g_mW);
	output.worldPos = mul(pos, g_mW);
	output.uv = uv;
	output.lightTexCoord = mul(pos, g_mWLP);

	return output;
}

/******************************************/
//ピクセルシェーダー用関数
/******************************************/

//ピクセルシェーダーでのライティング計算
PS_OUTPUT_DEFERRED GetPixel_Deferred(VS_OUTPUT_DEFERRED input)
{
	PS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;

	output.albedo = GetTexture(input.uv);
	output.normal = float4(input.normal, 1.0f);
	output.position = float4(input.worldPos, 1.0f);

	return output;
}

//ピクセルシェーダーでのライティング計算(影用)
PS_OUTPUT_DEFERRED GetPixelShadowMap_Deferred(VS_OUTPUT_DEFERRED_SHADOWMAP input)
{
	PS_OUTPUT_DEFERRED output = (VS_OUTPUT_DEFERRED)0;
	
	return output;
}

//後方レンダリング用ポイントライト計算
float3 GetDeferredPointLight(float3 pos, float3 normal, float3 eyeVector)
{
	float3 color;     //最終結果色
	float3 lightDir;  //頂点から点光源への方向
	float3 lightVec;  //点光源のベクトル
	float distance;   //頂点から点光源への距離
	float percentage; //点光源の半径の割合
	float result;     //計算結果
	float attenution; //減衰

	//複数の点光源計算
	for (int i = 0; i < g_DLihgtNum; i++)
	{
		lightVec = g_DLihgtPos[i].xyz - pos;
		lightDir = normalize(lightVec);

		//環境光　項
		float4 ambient = g_Ambient;

		//拡散反射光　項
		float NL = Lambert(normal, lightDir);
		float4 diffuse = g_Diffuse * HalfLambert(NL);

		//鏡面反射光　項
		float4 specular = g_Specular * BlinnPhong(normal, lightDir, eyeVector);

		//減衰計算
		distance = length(lightVec);
		percentage = distance / g_DRange[i].w;
		percentage = percentage * percentage * percentage * percentage;
		result = saturate(1.0f - percentage);
		result = result * result;
		attenution = result / (1.0f + (distance * distance));

		color += ((ambient + diffuse + specular).rgb * attenution * g_DLihgtPos[i].w) * g_DRange[i].rgb;
	}

	return color;
}