#pragma warning(disable : 3206)

//定義
#define MAX_BONE_MATRICES 255

//グローバル
Texture2D g_texColor: register(t0);
Texture2D g_Texture: register(t2);
SamplerState g_samLinear : register(s0);

//グローバル
cbuffer global_0 : register(b0)
{
	matrix g_mW;		//ワールド行列
	matrix g_mWVP;		//ワールドから射影までの変換行列
	float4 g_vLightDir; //ライトの方向ベクトル
	float4 g_Intensity;//ディレクショナルライト情報（ライトの明るさ）
	float4 g_vEye;		//カメラ位置
};

cbuffer global_1 : register(b1)
{
	float4 g_Ambient; //アンビエント光
	float4 g_Diffuse; //拡散反射(色）
	float4 g_Specular; //鏡面反射
};

cbuffer global_bones : register(b2)//ボーンのポーズ行列が入る
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 uv : TEXCOORD3;
	//uint4  Bones : BONE_INDEX;//ボーンのインデックス
	//float4 Weights : BONE_WEIGHT;//ボーンの重み
};

//スキニング後の頂点・法線が入る
struct Skin
{
	float4 Pos;
	float3 Normal;
};

//頂点をスキニング（ボーンにより移動）する。サブ関数（バーテックスシェーダーで使用）
Skin SkinVert(float4 Pos, float4 Normal, uint4  Bones, float4 Weights)
{
	Skin Output = (Skin)0;

	matrix matA = Weights.x * g_mConstBoneWorld[Bones.x];
	matrix matB = Weights.y * g_mConstBoneWorld[Bones.y];
	matrix matC = Weights.z * g_mConstBoneWorld[Bones.z];
	matrix matD = Weights.w * g_mConstBoneWorld[Bones.w];
	matrix mat = matA + matB + matC + matD;

	Output.Pos = mul(Pos, mat);
	Output.Normal = mul(Normal, (float3x3)mat);

	return Output;
}

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
	float4 color;
	float4 TexColor1 = g_texColor.Sample(g_samLinear, uv);
	float4 TexColor2 = g_Texture.Sample(g_samLinear, uv);;

	color = TexColor1 * (1 - TexColor2.a) + TexColor2 * TexColor2.a;

	color.rgb *= matrial.rgb * g_Intensity.rgb * g_Intensity.w;
	color.a *= g_Diffuse.w; //アルファ値反映
	return color;
}

//バーテックスシェーダー
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 tex : TEXCOORD, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	Skin vSkinned = SkinVert(Pos, Normal, Bones, Weights);

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(vSkinned.Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = normalize(mul(vSkinned.Normal, (float3x3)g_mW));

	//ディレクショナルライト
	output.Light = -g_vLightDir;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_vEye);

	output.uv = tex; //テクスチャ格納
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

//バーテックスシェーダー
VS_OUTPUT VS_NoTeX(float4 Pos : POSITION, float4 Normal : NORMAL, uint4  Bones : BONE_INDEX, float4 Weights : BONE_WEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	Skin vSkinned = SkinVert(Pos, Normal, Bones, Weights);

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(vSkinned.Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = normalize(mul(vSkinned.Normal, (float3x3)g_mW));

	//ディレクショナルライト
	output.Light = -g_vLightDir;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float3 PosWorld = mul(output.Pos, g_mW);
	output.EyeVector = normalize(PosWorld - g_vEye);

	return output;
}

//ピクセルシェーダー
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//環境光　項
	float4 ambient = g_Ambient;

	//拡散反射光　項
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse = g_Diffuse * NL;

	//鏡面反射光　項
	float3 reflect = normalize(2 * NL * input.Normal - input.Light);
	float4 specular = pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//フォンモデル最終色　３つの項の合計
	float4 color = ambient + diffuse + specular;

	color.rgb *= g_Intensity.rgb; //ライトの色反映
	color *= g_Intensity.w; //ライト強度を反映
	color.a = g_Diffuse.w; //アルファ値反映

	return color;
}