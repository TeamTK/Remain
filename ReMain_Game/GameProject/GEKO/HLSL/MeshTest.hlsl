#pragma warning(disable : 3206)

//テクスチャー
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//変換行列やライト
cbuffer global_0:register(b0)
{
	matrix g_mW;		//ワールド行列
	matrix g_mWVP;		//ワールドから射影までの変換行列
	float4 g_vLightDir; //ライトの方向ベクトル
	float4 g_fIntensity;//ディレクショナルライト情報（ライトの明るさ）
	float4 g_vEye;		//カメラ位置
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
	float2 Tex : TEXCOORD3;
};

interface BaseColor
{
	float4 GetColor(VS_OUTPUT input);
};

BaseColor g_BaseColor;

//フォンシェーディング
class PhongShading : BaseColor
{
	float4 g_EyePos; //視点座標

	float4 GetColor(VS_OUTPUT input)
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
		float4 matrial = ambient + diffuse + specular;
		float4 color = g_texColor.Sample(g_samLinear, input.Tex);
		color.rgb *= matrial.rgb * g_fIntensity.rgb * g_fIntensity.w;
		color.a *= g_Diffuse.w; //アルファ値反映

		return color;
	}
};

//バーテックスシェーダー
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//射影変換（ワールド→ビュー→プロジェクション）
	output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる(モデルが回転すれば法線も回転させる）
	output.Normal = mul(Normal, (float3x3)g_mW);

	//ディレクショナルライト
	output.Light = g_vLightDir.xyz;

	//視線ベクトル　ワールド空間上での頂点から視点へ向かうベクトル
	float4 PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld).xyz;
	output.Tex = Tex;
	return output;
}

//ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_BaseColor.GetColor(input);
}