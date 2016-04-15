#include "../CommonMeshInfo.hlsl"

#ifndef _FORWARD_MESH_INFO_
#define _FORWARD_MESH_INFO_

#define FORWARD_LIGHT_MAX 10

/******************************************/
//定数バッファー
/******************************************/

//前方レンダリング用点光源
cbuffer Global_ForwardLight : register(b4)
{
	float4 g_FLihgtPos[FORWARD_LIGHT_MAX];  //点光源の位置と明るさ
	float4 g_FRange[FORWARD_LIGHT_MAX];     //点光源の半径と色
	float g_FLihgtNum;						//点光源の数
};

/******************************************/
//頂点情報出力用
/******************************************/

//頂点シェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
	float3 worldPos : TEXCOORD4;
};

//頂点シェーダー出力構造体(影あり)
struct VS_OUTPUT_SHADOWMAP
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float3 worldPos : TEXCOORD3;
	float2 uv : TEXCOORD4;
	float4 LightTexCoord : TEXCOORD5;
};

/******************************************/
//頂点シェーダー用関数
/******************************************/

//頂点を変換
VS_OUTPUT GetVertexOutPut(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = -g_LightDir.xyz;

	//視線ベクトル
	float4 posWorld = mul(pos, g_mW);
	output.EyeVector = normalize(g_Eye - posWorld).xyz;

	output.uv = uv;

	output.worldPos = posWorld;

	return output;
}

//頂点を変換（影）
VS_OUTPUT_SHADOWMAP GetVertexShadowMapOutPut(float4 pos, float3 normal, float2 uv)
{
	VS_OUTPUT_SHADOWMAP output = (VS_OUTPUT_SHADOWMAP)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = -g_LightDir.xyz;

	//視線ベクトル
	float4 posWorld = mul(pos, g_mW);
	output.EyeVector = normalize(g_Eye - posWorld).xyz;

	output.worldPos = posWorld;

	output.uv = uv; //uv格納

	output.LightTexCoord = mul(pos, g_mWLP);

	return output;
}

/******************************************/
//ピクセルシェーダー用関数
/******************************************/

/*
//マテリアルの合計
float4 MatrialColor(float4 ambient, float4 diffuse, float4 specular, float2 uv)
{
	float4 matrial = ambient + diffuse + specular;
	float4 color = GetTexture(uv);

	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //アルファ値反映
	return color;
}
*/

//前方レンダリング用ポイントライト計算
float3 GetForwardPointLight(float3 pos, float3 normal, float3 eyeVector)
{
	float3 color;     //最終結果色
	float3 lightDir;  //頂点から点光源への方向
	float3 lightVec;  //点光源のベクトル
	float distance;   //頂点から点光源への距離
	float percentage; //点光源の半径の割合
	float result;     //計算結果
	float attenution; //減衰

	//複数の点光源計算
	for (int i = 0; i < g_FLihgtNum; i++)
	{
		lightVec = g_FLihgtPos[i].xyz - pos;
		lightDir = normalize(lightVec);

		//環境光　項
		float4 ambient = g_Ambient;

		//拡散反射光　項
		float NL = Lambert(normal, lightDir);
		float4 diffuse = g_Diffuse * HalfLambert(NL);;

		//鏡面反射光　項
		float4 specular = g_Specular * BlinnPhong(normal, lightDir, eyeVector);

		//減衰計算
		distance = length(lightVec);
		percentage = distance / g_FRange[i].w;
		percentage = percentage * percentage * percentage * percentage;
		result = saturate(1.0f - percentage);
		result = result * result;
		attenution = result / (1.0f + (distance * distance));

		color += ((ambient + diffuse + specular).rgb * attenution * g_FLihgtPos[i].w) * g_FRange[i].rgb;
	}

	return color;
}

//ピクセルシェーダーでのライティング計算
float4 GetPixel(VS_OUTPUT input)
{
	/*
	float3 leng = g_Eye - input.worldPos;

	if (length(leng) < 5)
	{
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	*/
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//平行光源でのマテリアル合成
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//平行光源と点光源を計算
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	color.a *= g_Diffuse.w; //アルファ値反映

	return color;
}

//ピクセルシェーダーでのライティングと影計算
float4 GetPixelShadowMap(VS_OUTPUT_SHADOWMAP input)
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//平行光源でのマテリアル合成
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//平行光源と点光源を計算
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	//影付加
	color = ShadowMap_PS(color.a, input.LightTexCoord, color);

	return color;
}

//ピクセルシェーダーでのライティング計算と霧
float4 GetPixelFog(VS_OUTPUT input)
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//平行光源でのマテリアル合成
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//平行光源と点光源を計算
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	color.a *= g_Diffuse.w; //アルファ値反映

	return Fog(color, g_FogColor, length(g_Eye - input.worldPos), g_FogDensity);
}

//ピクセルシェーダーでのライティングと影計算と霧
float4 GetPixelShadowMap_Fog(VS_OUTPUT_SHADOWMAP input)
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//平行光源でのマテリアル合成
	float4 matrial = ambient + diffuse + specular;

	float4 color = GetTexture(input.uv);

	//平行光源と点光源を計算
	float3 lightColor = GetForwardPointLight(input.worldPos, input.Normal, input.EyeVector);
	lightColor += matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.rgb *= lightColor;

	//影付加
	color = ShadowMap_PS(color.a, input.LightTexCoord, color);

	return Fog(color, g_FogColor, length(g_Eye - input.worldPos), g_FogDensity);
}

#endif