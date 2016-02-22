#pragma warning(disable : 3206)

//テクスチャー
Texture2D g_texColor : register(t0); //モデルのテクスチャ
Texture2D g_ShadowMapTexture : register(t1); //深度テクスチャ
Texture2D g_Texture : register(t2); //深度テクスチャ
SamplerState g_samLinear : register(s0); //モデルのサンプラー
SamplerState g_ShadowMapSamLinear : register(s1); //深度のサンプラー

//変換行列やライト
cbuffer global_0:register(b0)
{
	matrix g_mW;		//ワールド行列
	matrix g_mWVP;		//ワールドから射影までの変換行列
	matrix g_mWLP;
	float4 g_LightDir; //ライトの方向ベクトル
	float4 g_Intensity;//ディレクショナルライト情報（ライトの明るさ）
	float4 g_Eye;		//カメラ位置
};

//マテリアル
cbuffer global_1:register(b1)
{
	float4 g_Ambient; //アンビエント光
	float4 g_Diffuse; //拡散反射(色）
	float4 g_Specular; //鏡面反射
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT_SHADOWMAP
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
	float4 LightTexCoord : TEXCOORD4;
};

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

//マテリアルの合計
float4 MatrialColor(float4 ambient, float4 diffuse, float4 specular, float2 uv)
{
	float4 matrial = ambient + diffuse + specular;
	float4 color = g_texColor.Sample(g_samLinear, uv) + g_Texture.Sample(g_samLinear, uv);
	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //アルファ値反映
	return color;
}

//バーテックスシェーダー
VS_OUTPUT VS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = -g_LightDir.xyz;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float4 PosWorld = mul(pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_Eye).xyz;
	output.uv = uv;
	return output;
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//モデル最終色　３つの項の合計
	return MatrialColor(ambient, diffuse, specular, input.uv);
}

/******************************************/
//影描画用
/******************************************/

//シャドウマップ判断
float4 ShadowMap_PS(float4 lightTexCoord, float4 color)
{
	//ライトからみたZ値
	float ZValue = lightTexCoord.z / lightTexCoord.w;

	//深度テクスチャを参照するUVを算出
	float2 TexCoord;
	TexCoord.x = (lightTexCoord.x / lightTexCoord.w + 1.0f) * 0.5f;
	TexCoord.y = (-lightTexCoord.y / lightTexCoord.w + 1.0f) * 0.5f;

	float sm = g_ShadowMapTexture.Sample(g_ShadowMapSamLinear, TexCoord).r;
	if (sm <= 0.0f) return color;

	if (ZValue + 0.005f > sm)
	{
		color.rgb *= 0.5f;
	}

	/*
	if ((TexCoord.x < 0 || TexCoord.x > 1 || TexCoord.y < 0 || TexCoord.y > 1))
	{

	}
	else
	{
		float sm = g_ShadowMapTexture.Sample(g_ShadowMapSamLinear, TexCoord).r;
		if (sm <= 0.0f) return color;

		if (ZValue < sm + 0.005f)
		{
			color.rgb *= 0.5f;
		}
	}
	*/
	return color;
}

VS_OUTPUT_SHADOWMAP VS_ShadowMap(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	VS_OUTPUT_SHADOWMAP output = (VS_OUTPUT_SHADOWMAP)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = -g_LightDir.xyz;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float4 PosWorld = mul(pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_Eye).xyz;

	output.uv = uv; //uv格納

	output.LightTexCoord = mul(pos, g_mWLP);

	return output;
}

//ピクセルシェーダー
float4 PS_ShadowMap(VS_OUTPUT_SHADOWMAP input) : SV_Target
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = Lambert(input.Normal, input.Light);
	float4 diffuse = g_Diffuse * HalfLambert(NL);

	//鏡面反射光　項
	float4 specular = g_Specular * BlinnPhong(input.Normal, input.Light, input.EyeVector);

	//モデル最終色　３つの項の合計
	float4 color = MatrialColor(ambient, diffuse, specular, input.uv);
	color = ShadowMap_PS(input.LightTexCoord, color);

	return color;
}
