#include "DeferredMeshInfo.hlsl"

//デファード関係
Texture2D g_Albedo : register(t3); //アルペド
Texture2D g_Normal : register(t4); //法線
Texture2D g_Position : register(t5); //位置

SamplerState g_SamLinear_Deferred : register(s3); //モデルのサンプラー

//頂点シェーダー出力構造体
struct VS_OUTPUT_DEFERRED_PASS2
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float2 uv : TEXCOORD2;
	float4 eyePos : TEXCOORD3;
};

VS_OUTPUT_DEFERRED_PASS2 VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT_DEFERRED_PASS2 output = (VS_OUTPUT_DEFERRED_PASS2)0;

	output.Pos = pos;
	output.Normal = normal;
	output.Light = -g_LightDir.xyz;
	output.uv = uv;
	output.eyePos = g_Eye;

	return output;
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT_DEFERRED_PASS2 input) : SV_Target
{
	//テクスチャから取り出す
	float4 albedo = g_Albedo.Sample(g_SamLinear_Deferred, input.uv);
	float3 normal = DecodeNormal(g_Normal.Sample(g_SamLinear_Deferred, input.uv).xyz);
	float3 position = g_Position.Sample(g_SamLinear_Deferred, input.uv).xyz;

	float4 ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);
	ambient *= float4(g_Intensity.rgb, 1.0f) * g_Intensity.w;

	float NL = Lambert(normal, input.Light);
	float4 diffuse = albedo * HalfLambert(NL);
	diffuse.a = albedo.a;

	float4 specular = BlinnPhong(normal, input.Light, input.eyePos - position);
	specular.a = 0.0f;

	//マテリアル合成
	float4 matrial = diffuse + specular;
	float4 color = matrial + float4(GetDeferredPointLight(position, normal, input.eyePos - position, albedo), 0.0f);
	color.rgb *= g_Intensity.w;
	//float4 color = matrial;
	//float4 color;
	//color = ambient + diffuse + specular;
	//平行光源と点光源を計算
	//float3 lightColor = GetDeferredPointLight(position, normal, input.EyeVector, albedo);
	//lightColor += matrial.rgb;
	//color.rgb *= 

	//color.a *= g_Diffuse.w; //アルファ値反映


	return diffuse;
}
