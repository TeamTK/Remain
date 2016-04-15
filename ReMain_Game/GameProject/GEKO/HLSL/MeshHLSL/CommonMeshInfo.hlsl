#pragma warning(disable : 3206)

#ifndef _COMMON_MESH_INFO_
#define _COMMON_MESH_INFO_

/******************************************/
//テクスチャリソース
/******************************************/

Texture2D g_texColor : register(t0); //モデルのテクスチャ
Texture2D g_ShadowMapTexture : register(t1); //深度テクスチャ
Texture2D g_Texture : register(t2); //マルチテクスチャ
SamplerState g_SamLinear : register(s0); //モデルのサンプラー
SamplerState g_ShadowMapSamLinear : register(s1); //深度のサンプラー

/******************************************/
//定数バッファー
/******************************************/

//全てのメッシュ共通情報
cbuffer Global_CommonInfo : register(b0)
{
	float4 g_LightDir;  //ライトの方向ベクトル
	float4 g_Intensity; //ディレクショナルライト情報（ライトの明るさ）
	float4 g_Eye;		//カメラ位置
	float4 g_FogColor;  //フォグの色
	float g_FogDensity; //フォグの密度
};

//変換行列
cbuffer Global_TransformMatrix : register(b1)
{
	matrix g_mW;	//ワールド行列
	matrix g_mWVP;	//ワールドから射影までの変換行列
	matrix g_mWLP;	//ワールド × ライトビュー × ライト射影
};

//マテリアル
cbuffer Global_Material : register(b2)
{
	float4 g_Ambient;  //アンビエント光
	float4 g_Diffuse;  //拡散反射(色）
	float4 g_Specular; //鏡面反射
};

/******************************************/
//頂点情報出力用
/******************************************/

/******************************************/
//頂点シェーダー用関数
/******************************************/

/******************************************/
//ピクセルシェーダー用関数
/******************************************/

//ランバート反射
float Lambert(float3 normal, float3 light)
{
	return max(dot(normal, light), 0.0f);
}

//ハーフランバート反射
float HalfLambert(float NL)
{
	float Half = NL * 0.5f + 0.5f;
	return Half * Half;
}

//フォン反射
float4 Phong(float NL, float3 n, float3 l, float3 v)
{
	float3 reflect = normalize(2 * NL * n - l);
	return pow(max(dot(reflect, v), 0.0f), 4);
}

//ブリンフォン反射
float4 BlinnPhong(float3 n, float3 l, float3 v)
{
	float3 h = normalize(v + l);
	return pow(max(dot(n, h), 0.0f), 4);
}

//テクスチャ取得（マルチ対応）
float4 GetTexture(float2 uv)
{
	float4 TexColor1 = g_texColor.Sample(g_SamLinear, uv);
	float4 TexColor2 = g_Texture.Sample(g_SamLinear, uv);
	return TexColor1 * (1 - TexColor2.a) + TexColor2 * TexColor2.a;
}

//シャドウマップ判断
float4 ShadowMap_PS(float alpha, float4 lightTexCoord, float4 color)
{
	//ライトからみたZ値
	//float ZValue = (lightTexCoord.z / lightTexCoord.w + 1.0f) * 0.5f * alpha;
	float ZValue = lightTexCoord.z / lightTexCoord.w * alpha;

	//深度テクスチャを参照するUVを算出
	float2 TexCoord;
	TexCoord.x = (lightTexCoord.x / lightTexCoord.w * 0.5f) + 0.5f;
	TexCoord.y = (-lightTexCoord.y / lightTexCoord.w * 0.5f) + 0.5f;

	float sm = g_ShadowMapTexture.Sample(g_ShadowMapSamLinear, TexCoord).r;
	if (sm <= 0.0f) return color;

	if (ZValue + 0.005f > sm)
	{
		color.rgb *= 0.5f;
	}

	return color;
}

//フォグを計算
float4 Fog(float4 color, float4 fogColor, float distance, float density)
{
	float e = 2.71828;
	float d = distance * density;
	float f = pow(e, -pow(d, 4));
	f = saturate(f);
	float3 c = f * color.rgb + (1 - f) * fogColor.rgb;

	return float4(c, color.a);
}

#endif